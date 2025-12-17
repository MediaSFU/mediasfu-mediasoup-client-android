#include "mediasoupclient.hpp"
#include <jni.h>
// WebRTC M137: class_reference_holder.h removed, use JNI helpers directly
#include "sdk/android/native_api/base/init.h"
#include "sdk/android/native_api/jni/jvm.h"

namespace mediasoupclient
{
extern "C" jint JNIEXPORT JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved)
{
	// WebRTC M137: Initialize JVM for WebRTC
	webrtc::InitAndroid(jvm);
	
	// Initialize mediasoupclient
	mediasoupclient::Initialize();
	
	return JNI_VERSION_1_6;
}

extern "C" void JNIEXPORT JNICALL JNI_OnUnload(JavaVM* jvm, void* reserved)
{
	// WebRTC M137: Cleanup is simpler
	mediasoupclient::Cleanup();
}

} // namespace mediasoupclient
