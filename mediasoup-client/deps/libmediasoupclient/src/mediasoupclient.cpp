#define MSC_CLASS "mediasoupclient"

#include "mediasoupclient.hpp"
#include "Logger.hpp"
#include "version.hpp"
#include <api/environment/deprecated_global_field_trials.h>
#include <rtc_base/crypto_random.h>
#include <rtc_base/ssl_adapter.h>
#include <rtc_base/time_utils.h>
#include <sstream>

namespace mediasoupclient
{
	void Initialize() // NOLINT(readability-identifier-naming)
	{
		MSC_TRACE();

		MSC_DEBUG("mediasoupclient v%s", Version().c_str());

		webrtc::DeprecatedGlobalFieldTrials::Set("WebRTC-SupportVP9SVC/EnabledByFlag_3SL3TL/");

		webrtc::InitializeSSL();
		webrtc::InitRandom(static_cast<int>(webrtc::Time()));
	}

	void Cleanup() // NOLINT(readability-identifier-naming)
	{
		MSC_TRACE();

		webrtc::CleanupSSL();
	}

	std::string Version() // NOLINT(readability-identifier-naming)
	{
		std::stringstream ss;

		ss << MEDIASOUPCLIENT_VERSION_MAJOR << "." << MEDIASOUPCLIENT_VERSION_MINOR << "."
		   << MEDIASOUPCLIENT_VERSION_PATCH;

		return ss.str();
	}
} // namespace mediasoupclient
