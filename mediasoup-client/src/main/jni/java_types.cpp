#include "include/java_types.h"
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "MSC-JNI", __VA_ARGS__)

namespace mediasoupclient
{
ScopedJavaLocalRef<jstring> NativeToJavaString(JNIEnv* jni, const std::string& str)
{
	auto j_str = webrtc::NativeToJavaString(jni, str);
	return ScopedJavaLocalRef<jstring>(jni, j_str.Release());
}

ScopedJavaLocalRef<jstring> NativeToJavaString(JNIEnv* jni, const absl::optional<std::string>& str)
{
	if (!str.has_value())
	{
		return ScopedJavaLocalRef<jstring>();
	}
	return NativeToJavaString(jni, str.value());
}

// JavaToNativeString removed - using webrtc::JavaToNativeString directly

void JavaToNativeOptions(
  JNIEnv* env, const JavaRef<jobject>& configuration, jlong factory, PeerConnection::Options& options)
{
	LOGI("=== JavaToNativeOptions ===");
	LOGI("  factory pointer (jlong): %lld", (long long)factory);
	
	// ALWAYS set the factory first, even if configuration is null
	options.factory = reinterpret_cast<webrtc::PeerConnectionFactoryInterface*>(factory);
	LOGI("  options.factory set to: %p", options.factory);
	
	if (configuration.is_null())
	{
		LOGI("  RTCConfiguration is null (using defaults)");
		return;
	}

	webrtc::PeerConnectionInterface::RTCConfiguration rtc_config(
	  webrtc::PeerConnectionInterface::RTCConfigurationType::kAggressive);
	webrtc::jni::JavaToNativeRTCConfiguration(
	  env, webrtc::JavaParamRef<jobject>(configuration.obj()), &rtc_config);
	options.config = rtc_config;
	LOGI("  RTCConfiguration parsed OK");
}
} // namespace mediasoupclient
