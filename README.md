<p align="center">
  <img src="https://www.mediasfu.com/logo192.png" width="100" alt="MediaSFU Logo">
</p>

<p align="center">
  <a href="https://central.sonatype.com/search?q=g:com.mediasfu+mediasoup-client"><img src="https://img.shields.io/maven-central/v/com.mediasfu/mediasoup-client.svg?label=Maven%20Central" alt="Maven Central"></a>
  <a href="https://opensource.org/licenses/MIT"><img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="License: MIT"></a>
  <a href="https://twitter.com/media_sfu"><img src="https://img.shields.io/badge/Twitter-1DA1F2?style=flat&logo=twitter&logoColor=white" alt="Twitter"></a>
  <a href="https://github.com/MediaSFU"><img src="https://img.shields.io/badge/GitHub-181717?style=flat&logo=github&logoColor=white" alt="GitHub"></a>
  <a href="https://www.mediasfu.com/"><img src="https://img.shields.io/badge/Website-4285F4?style=flat&logo=google-chrome&logoColor=white" alt="Website"></a>
  <a href="https://www.youtube.com/channel/UCELghZRPKMgjih5qrmXLtqw"><img src="https://img.shields.io/badge/YouTube-FF0000?style=flat&logo=youtube&logoColor=white" alt="YouTube"></a>
  <a href="https://www.mediasfu.com/forums"><img src="https://img.shields.io/badge/Community-Forum-blue?style=flat&logo=discourse&logoColor=white" alt="Community"></a>
</p>

<p align="center">
  <a href="https://mediasfu.com"><img src="https://img.shields.io/badge/Built%20with-MediaSFU-blue" alt="Built with MediaSFU"></a>
  <a href="https://developer.android.com"><img src="https://img.shields.io/badge/Android-3DDC84?style=flat&logo=android&logoColor=white" alt="Android"></a>
  <a href="https://mediasoup.org"><img src="https://img.shields.io/badge/mediasoup-v3-orange" alt="mediasoup v3"></a>
  <a href="https://webrtc.org"><img src="https://img.shields.io/badge/WebRTC-M137-brightgreen" alt="WebRTC M137"></a>
</p>

---

# mediasfu-mediasoup-client-android

**MediaSFU's maintained fork** of [haiyangwu/mediasoup-client-android](https://github.com/haiyangwu/mediasoup-client-android) with WebRTC M137 and extended codec support.

## 🔧 What We Changed

This fork includes significant modifications from the original:

### 1. WebRTC M137 Migration (Complete JNI Rewrite)

The original library used WebRTC M114. We migrated to **WebRTC M137**, which required a **complete rewrite of the JNI layer** (23 files) due to breaking API changes:

- `ScopedJavaGlobalRef` now uses move-only semantics (copy assignment deleted)
- Updated callback patterns and listener registration
- `AddTransceiver` signature changes
- Header reorganization for M137 compatibility

**Files rewritten:**
- `scoped_java_ref.h`, `jni_onload.cpp`, `jni_generator_helper.cpp`
- `transport_jni.cpp`, `peerconnection_jni.cpp`, `device_jni.cpp`
- `producer_jni.cpp`, `consumer_jni.cpp`, `data_producer_jni.cpp`, `data_consumer_jni.cpp`
- And 13 more header/implementation files

### 2. Custom WebRTC Build with Extended Codecs

The original used default WebRTC builds which **did not include H264, PCMU, or PCMA codecs**. We built a custom WebRTC AAR with these flags:

```
rtc_use_h264=true
rtc_include_ilbc=true
rtc_include_legacy_audio_device=true
```

**Codecs now enabled:**

| Codec | Type | Use Case |
|-------|------|----------|
| **H.264** | Video | Hardware-accelerated video encoding/decoding |
| **PCMU** | Audio | G.711 μ-law (North America/Japan telephony) |
| **PCMA** | Audio | G.711 A-law (Europe/International telephony) |
| VP8, VP9 | Video | Software video codecs (already in original) |
| Opus | Audio | Modern audio codec (already in original) |

### 3. Security Updates

- Latest WebRTC security patches from M137
- Removed insecure build configurations

---

## 📦 Installation

### Gradle (Maven Central)

```groovy
dependencies {
    implementation 'com.mediasfu:mediasoup-client:1.0.2'
}
```

### Manual AAR

Download from [Releases](https://github.com/MediaSFU/mediasfu-mediasoup-client-android/releases):

```groovy
dependencies {
    implementation files('libs/mediasoup-client-release.aar')
}
```

---

## 🔄 Comparison with Original

| Feature | Original (haiyangwu v3.4.0) | MediaSFU Fork (v1.0.2) |
|---------|---------------------------|----------------------|
| WebRTC Version | M114 | **M137** |
| H.264 Codec | ❌ Not compiled | ✅ **Enabled** |
| PCMU/PCMA | ❌ Not compiled | ✅ **Enabled** |
| JNI Layer | Legacy API | **M137 move semantics** |
| Maintenance | Last update: 2022 | **Active (2025)** |
| Security Patches | M114 level | **M137 level** |

---

## 📖 Quick Start

### 1. Initialize MediasoupClient

```java
import org.mediasoup.droid.MediasoupClient;

MediasoupClient.initialize(getApplicationContext());
```

### 2. Create Device and Load Capabilities

```java
import org.mediasoup.droid.Device;

Device device = new Device();
String routerRtpCapabilities = // ... from your server
device.load(routerRtpCapabilities, null);
```

### 3. Produce Media

```java
import org.mediasoup.droid.SendTransport;
import org.mediasoup.droid.Producer;

SendTransport sendTransport = device.createSendTransport(
    listener, transportId, iceParameters, iceCandidates, dtlsParameters
);

// Now with H.264 and PCMU/PCMA available!
Producer videoProducer = sendTransport.produce(
    producerListener, videoTrack, encodings, codecOptions, codec
);
```

### 4. Consume Media

```java
import org.mediasoup.droid.RecvTransport;
import org.mediasoup.droid.Consumer;

RecvTransport recvTransport = device.createRecvTransport(
    listener, transportId, iceParameters, iceCandidates, dtlsParameters
);

Consumer consumer = recvTransport.consume(
    consumerListener, consumerId, producerId, kind, rtpParameters, appData
);
```

---

## 📱 Supported ABIs

| Architecture | Supported | Notes |
|-------------|-----------|-------|
| `arm64-v8a` | ✅ | Primary target (modern devices) |
| `armeabi-v7a` | ✅ | Legacy 32-bit ARM |
| `x86_64` | ✅ | Emulator support |
| `x86` | ✅ | Legacy emulator |

---

## 🔨 Build from Source

### Prerequisites

- Ubuntu 20.04/22.04 (for WebRTC build)
- 16+ CPU cores, 64 GB RAM, 250 GB disk
- NDK 26.1.10909125
- CMake 3.22+

### Build Steps

```bash
git clone https://github.com/MediaSFU/mediasfu-mediasoup-client-android.git
cd mediasfu-mediasoup-client-android
./gradlew :mediasoup-client:assembleRelease
```

---

## 🛠️ Technical Details

### JNI Migration Summary

| Component | Changes Made |
|-----------|-------------|
| `scoped_java_ref.h` | Foundation header for M137 move semantics |
| `transport_jni.cpp` | Dual listener pattern, most complex file |
| `peerconnection_jni.cpp` | Critical `AddTransceiver` signature fix |
| `jni_onload.cpp` | JNI initialization for M137 |
| All `*_jni.cpp` files | Updated to use move-assign instead of copy |

### Dependencies

| Dependency | Version | Notes |
|------------|---------|-------|
| WebRTC | M137 (branch-heads/6151) | Custom build with H264/PCMU/PCMA |
| libmediasoupclient | 3.x | C++ mediasoup client library |
| AndroidX AppCompat | 1.1.0 | Compatibility library |

---

## 🙏 Credits & Acknowledgments

This project is a fork of the excellent work by:

- **[haiyangwu](https://github.com/haiyangwu)** - Original [mediasoup-client-android](https://github.com/haiyangwu/mediasoup-client-android) author
- **[versatica](https://github.com/versatica)** - [mediasoup](https://mediasoup.org) and [libmediasoupclient](https://github.com/versatica/libmediasoupclient) team
- **WebRTC Project** - [webrtc.org](https://webrtc.org)

---

## 📚 Related MediaSFU SDKs

<table>
  <tr>
    <td align="center"><a href="https://github.com/MediaSFU/MediaSFU-ReactNative"><img src="https://img.shields.io/badge/React_Native-20232A?style=for-the-badge&logo=react&logoColor=61DAFB" alt="React Native"></a></td>
    <td align="center"><a href="https://github.com/MediaSFU/MediaSFU-ReactNative-Expo"><img src="https://img.shields.io/badge/Expo-000020?style=for-the-badge&logo=expo&logoColor=white" alt="Expo"></a></td>
    <td align="center"><a href="https://github.com/MediaSFU/MediaSFUKotlin"><img src="https://img.shields.io/badge/Kotlin-7F52FF?style=for-the-badge&logo=kotlin&logoColor=white" alt="Kotlin"></a></td>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/MediaSFU/MediaSFU-Angular"><img src="https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white" alt="Angular"></a></td>
    <td align="center"><a href="https://github.com/MediaSFU/MediaSFUVue"><img src="https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D" alt="Vue.js"></a></td>
    <td align="center"><a href="https://github.com/MediaSFU/MediaSFUReactJS"><img src="https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB" alt="ReactJS"></a></td>
  </tr>
</table>

---

## 📄 License

MIT License - See [LICENSE](LICENSE) for details.

---

<p align="center">
  <strong>Maintained by <a href="https://mediasfu.com">MediaSFU</a></strong>
  <br>
  Part of the MediaSFU SDK ecosystem
</p>

<p align="center">
  <a href="https://mediasfu.com"><img src="https://www.mediasfu.com/logo192.png" width="50" alt="MediaSFU"></a>
</p>
