#ifndef hSDK_Resources_HeaderPlusPlus
#define hSDK_Resources_HeaderPlusPlus

#include "hSDK.hpp"

namespace hSDK
{
	struct Resource final
	{
		using data_t = std::uint8_t const *;
		data_t const data;
		std::size_t const size;

		static Resource Get(string const &name);
	};
}

#endif
