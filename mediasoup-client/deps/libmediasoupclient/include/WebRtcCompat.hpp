#ifndef MSC_WEBRTCCOMPAT_HPP
#define MSC_WEBRTCCOMPAT_HPP

#include <api/scoped_refptr.h>
#include <rtc_base/ref_counted_object.h>
#include <api/jsep.h>
#include <rtc_base/thread.h>
#include <map>
#include <string>

namespace webrtc
{
	template<typename T>
	using scoped_refptr = rtc::scoped_refptr<T>;

	template<typename T>
	using RefCountedObject = rtc::RefCountedObject<T>;

	using IceCandidate = IceCandidateInterface;
	using Thread = rtc::Thread;
	using CodecParameterMap = std::map<std::string, std::string>;
}

#endif
