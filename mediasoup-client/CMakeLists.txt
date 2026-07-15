cmake_minimum_required(VERSION 3.5)

project(mediasoupclient_so LANGUAGES CXX)

# Set version number.
set(mediasoup_client_so_VERSION_MAJOR 3)
set(mediasoup_client_so_VERSION_MINOR 0)
set(mediasoup_client_so_VERSION_PATCH 8)

# C++ standard requirements (C++17 minimum for WebRTC M137 STL features).
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 16KB page size alignment for Android 15+ (Google Play requirement).
# See: https://developer.android.com/guide/practices/page-sizes
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,max-page-size=16384")

set(
	SOURCE_DIR
	${mediasoupclient_so_SOURCE_DIR}/src/main/jni/
)

set(
	SOURCE_FILES
	${SOURCE_DIR}/jni_generator_helper.cpp
	${SOURCE_DIR}/jni_onload.cpp
	${SOURCE_DIR}/scoped_java_ref.cpp
	${SOURCE_DIR}/java_types.cpp
	${SOURCE_DIR}/consumer_jni.cpp
	${SOURCE_DIR}/data_consumer_jni.cpp
	${SOURCE_DIR}/data_producer_jni.cpp
	${SOURCE_DIR}/device_jni.cpp
	${SOURCE_DIR}/logger_jni.cpp
	${SOURCE_DIR}/mediasoup_client_jni.cpp
	${SOURCE_DIR}/peerconnection_jni.cpp
	${SOURCE_DIR}/producer_jni.cpp
	${SOURCE_DIR}/transport_jni.cpp
)

# TODO(haiyang wu): add condition
list(APPEND SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/androidTest/jni/parameters_jni.cpp)
list(APPEND SOURCE_FILES ${PROJECT_SOURCE_DIR}/deps/libmediasoupclient/test/src/fakeParameters.cpp)

# Create target.
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# WebRTC M137's legacy StatsCollector crosses from the signaling thread to the
# worker thread and used to re-read the live transceiver receiver list there.
# Replace only that archive member with the vendored source backport, which
# snapshots receiver proxies before crossing threads.
if(ANDROID)
	set(WEBRTC_ORIGINAL_ARCHIVE
		"${LIBWEBRTC_BINARY_PATH}/${ANDROID_ABI}/libwebrtc${CMAKE_STATIC_LIBRARY_SUFFIX}")
	set(WEBRTC_PATCHED_ARCHIVE
		"${CMAKE_CURRENT_BINARY_DIR}/libwebrtc-without-stats-collector${CMAKE_STATIC_LIBRARY_SUFFIX}")

	add_custom_command(
		OUTPUT "${WEBRTC_PATCHED_ARCHIVE}"
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different
			"${WEBRTC_ORIGINAL_ARCHIVE}" "${WEBRTC_PATCHED_ARCHIVE}"
		COMMAND "${CMAKE_AR}" d "${WEBRTC_PATCHED_ARCHIVE}" stats_collector.o
		DEPENDS "${WEBRTC_ORIGINAL_ARCHIVE}"
		COMMENT "Removing the vulnerable WebRTC legacy stats collector object"
		VERBATIM
	)
	add_custom_target(webrtc_archive_without_stats_collector
		DEPENDS "${WEBRTC_PATCHED_ARCHIVE}")

	add_library(webrtc_stats_collector_patch OBJECT
		"${PROJECT_SOURCE_DIR}/deps/webrtc/src/pc/stats_collector.cc")
	set_target_properties(webrtc_stats_collector_patch PROPERTIES
		CXX_STANDARD 14
		CXX_STANDARD_REQUIRED ON
		POSITION_INDEPENDENT_CODE ON
	)
	target_compile_definitions(webrtc_stats_collector_patch PRIVATE
		WEBRTC_ANDROID
		WEBRTC_POSIX
	)
	target_include_directories(webrtc_stats_collector_patch SYSTEM PRIVATE
		"${LIBWEBRTC_INCLUDE_PATH}"
		"${LIBWEBRTC_INCLUDE_PATH}/third_party/abseil-cpp"
	)
	target_sources(${PROJECT_NAME} PRIVATE
		$<TARGET_OBJECTS:webrtc_stats_collector_patch>)
	add_dependencies(${PROJECT_NAME} webrtc_archive_without_stats_collector)

	set(LIBWEBRTC_LIBRARY_PATH "${WEBRTC_PATCHED_ARCHIVE}"
		CACHE FILEPATH "Patched Android libwebrtc archive" FORCE)
endif()

if(${MEDIASOUPCLIENT_LOG_TRACE})
	target_compile_definitions(${PROJECT_NAME}
		PRIVATE MSC_LOG_TRACE=1
	)
endif()

if(${MEDIASOUPCLIENT_LOG_DEV})
	target_compile_definitions(${PROJECT_NAME}
		PRIVATE MSC_LOG_DEV=1
	)
endif()

# Source Dependencies. override LIBWEBRTC_BINARY_PATH
# TODO(HaiyangWu) add libwebrtc.a without change CMakeLists in libmediasoupclient
# set(LIBWEBRTC_BINARY_PATH ${LIBWEBRTC_BINARY_ANDROID_PATH}/${ANDROID_ABI} CACHE STRING "libwebrtc binary path" FORCE)
add_subdirectory("deps/libmediasoupclient")

# Add some compile flags to our source files.
set_source_files_properties(${SOURCE_FILES}
	PROPERTIES COMPILE_FLAGS -Wall -Wextra -Wpedantic)

include_directories(SYSTEM "${LIBWEBRTC_INCLUDE_PATH}/third_party/abseil-cpp")

# Private (implementation) header files.
target_include_directories(${PROJECT_NAME} PUBLIC
	"${SOURCE_DIR}/"
	"${SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/deps/libmediasoupclient/include"
	"${PROJECT_SOURCE_DIR}/deps/libmediasoupclient/test/include"
	"${PROJECT_SOURCE_DIR}/deps/libmediasoupclient/deps/libsdptransform/include"
	"${LIBWEBRTC_INCLUDE_PATH}"
	"${LIBWEBRTC_INCLUDE_PATH}/third_party/abseil-cpp"
	"${LIBWEBRTC_INCLUDE_PATH}/sdk/android/native_api"
	"${LIBWEBRTC_INCLUDE_PATH}/sdk/android/native_api/jni"
	"${LIBWEBRTC_INCLUDE_PATH}/sdk/android/src/jni"
)

# Compile definitions for libwebrtc.
target_compile_definitions(${PROJECT_NAME} PUBLIC
	$<$<NOT:$<PLATFORM_ID:Windows>>:WEBRTC_POSIX>
	$<$<PLATFORM_ID:Windows>:WEBRTC_WIN>
	$<$<PLATFORM_ID:Windows>:NOMINMAX>
	$<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN>
	$<$<PLATFORM_ID:Darwin>:WEBRTC_MAC>
)

# Public (interface) dependencies.
target_link_libraries(${PROJECT_NAME} PUBLIC
	android
	log
	OpenSLES
	mediasoupclient
)
