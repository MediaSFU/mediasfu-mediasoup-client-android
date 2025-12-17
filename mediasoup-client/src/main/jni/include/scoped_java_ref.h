// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIASOUP_CLIENT_SCOPED_JAVA_REF_H_
#define MEDIASOUP_CLIENT_SCOPED_JAVA_REF_H_

// WebRTC M137 Migration: Use WebRTC's types directly instead of wrapping
// All ScopedJava* types are now from webrtc namespace with move-only semantics

#include "sdk/android/native_api/jni/scoped_java_ref.h"

namespace mediasoupclient {

// Import WebRTC M137 types into mediasoupclient namespace for compatibility
using webrtc::JavaRef;
using webrtc::JavaParamRef;
using webrtc::ScopedJavaGlobalRef;
using webrtc::ScopedJavaLocalRef;

// Also import utility functions
using webrtc::AttachCurrentThreadIfNeeded;

}  // namespace mediasoupclient

#endif  // MEDIASOUP_CLIENT_SCOPED_JAVA_REF_H_
