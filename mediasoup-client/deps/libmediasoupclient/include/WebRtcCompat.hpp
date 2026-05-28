#ifndef MSC_WEBRTCCOMPAT_HPP
#define MSC_WEBRTCCOMPAT_HPP

#include <api/scoped_refptr.h>
#include <rtc_base/ref_counted_object.h>

namespace rtc
{
	template<typename T>
	using scoped_refptr = webrtc::scoped_refptr<T>;

	template<typename T>
	using RefCountedObject = webrtc::RefCountedObject<T>;
}

#endif
