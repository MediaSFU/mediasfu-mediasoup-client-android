#include "Logger.hpp"
#include <jni.h>
#include <sdk/android/native_api/jni/java_types.h>

#include "generated_mediasoupclient_jni/jni/Logger_jni.h"

#define TAG "mediasoupclient-jni"

namespace mediasoupclient
{
class LogHandlerInterfaceJNI : public Logger::LogHandlerInterface
{
public:
	LogHandlerInterfaceJNI(JNIEnv* env, const JavaParamRef<jobject>& j_handler_interface)
	{
		// WebRTC M137: Initialize ScopedJavaGlobalRef with move semantics
		j_handler_interface_ = j_handler_interface;
		auto tag_local = webrtc::NativeToJavaString(env, TAG);
		jni_tag_ = tag_local;
		assert(!j_handler_interface_.is_null());
	}

	virtual ~LogHandlerInterfaceJNI() = default;

public:
	void OnLog(Logger::LogLevel level, char* payload, size_t len) override
	{
		std::string message(payload, len);
		auto env       = webrtc::AttachCurrentThreadIfNeeded();
		auto j_level   = Java_LogLevel_getLogLevel(env, static_cast<int>(level));
		auto j_message = webrtc::NativeToJavaString(env, message);
		// WebRTC M137: Pass ScopedJavaGlobalRef directly to callbacks
		Java_LogHandlerInterface_OnLog(env, j_handler_interface_, j_level, jni_tag_, j_message);
	}

private:
	webrtc::ScopedJavaGlobalRef<jobject> j_handler_interface_;
	webrtc::ScopedJavaGlobalRef<jstring> jni_tag_;
};

static void JNI_Logger_SetLogLevel(JNIEnv* env, jint j_level)
{
	Logger::SetLogLevel(static_cast<Logger::LogLevel>(j_level));
}

static jlong JNI_Logger_SetHandler(JNIEnv* env, const JavaParamRef<jobject>& j_handler)
{
	auto* handler = new LogHandlerInterfaceJNI(env, j_handler);
	Logger::SetHandler(reinterpret_cast<Logger::LogHandlerInterface*>(handler));
	return webrtc::NativeToJavaPointer(handler);
}

static void JNI_Logger_FreeLogHandler(JNIEnv* env, jlong j_handler)
{
	auto* handler = reinterpret_cast<LogHandlerInterfaceJNI*>(j_handler);
	delete handler;
}

} // namespace mediasoupclient
