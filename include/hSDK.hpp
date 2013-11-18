#ifndef hSDK_HeaderPlusPlus
#define hSDK_HeaderPlusPlus
#include <string>

namespace hSDK
{
	static_assert(sizeof(void *) == 4 && sizeof(std::size_t) == 4,
	              "MMF2 only supports 32-bit extensions");

#ifdef UNICODE
		using char_t = wchar_t;
#else
		using char_t = char;
#endif
		using string = std::basic_string<char_t>;
}

#endif
