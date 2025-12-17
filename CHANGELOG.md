# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-12-17

### 🎉 Initial Release (MediaSFU Fork)

This is the first release of MediaSFU's maintained fork of mediasoup-client-android.

### Added

- **WebRTC M137 Support** - Complete migration from M114 to M137
  - Full JNI layer rewrite (23 files) for move-only semantics compatibility
  - Updated `ScopedJavaGlobalRef` patterns throughout
  - New `AddTransceiver` signature support
  - M137 header reorganization

- **Extended Codec Support** via custom WebRTC build:
  - **H.264** - Hardware-accelerated video encoding/decoding
  - **PCMU** - G.711 μ-law audio codec
  - **PCMA** - G.711 A-law audio codec

- **Security Updates** - Latest patches from WebRTC M137

### Changed

- Updated NDK to 26.1.10909125
- Updated build target to Android SDK 31
- Removed insecure protocol workarounds from build.gradle

### Technical Details

Files modified for M137 migration:
- `scoped_java_ref.h` - Foundation header
- `jni_onload.cpp` - JNI initialization
- `jni_generator_helper.cpp` - Helper utilities
- `transport_jni.cpp` - Dual listener pattern (most complex)
- `peerconnection_jni.cpp` - AddTransceiver fix
- `device_jni.cpp`, `producer_jni.cpp`, `consumer_jni.cpp`
- `data_producer_jni.cpp`, `data_consumer_jni.cpp`
- And all corresponding header files

### Credits

This fork is based on [haiyangwu/mediasoup-client-android](https://github.com/haiyangwu/mediasoup-client-android) v3.4.0.
