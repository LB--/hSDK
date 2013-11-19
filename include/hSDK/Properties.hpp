#ifndef hSDK_Properties_HeaderPlusPlus
#define hSDK_Properties_HeaderPlusPlus

#include "hSDK.hpp"

#include <cstdint>
#include <functional>
#include <type_traits>
#include <algorithm>
#include <map>

namespace hSDK
{
	struct Properties
	{
		virtual ~Properties() = 0;

		using string = hSDK::string;

		struct ED;

		struct EdittimeInfo
		{
			static EdittimeInfo &Current()
			{
				static EdittimeInfo eti;
				return eti;
			}
			static ED *Ed()
			{
				return Current().ed;
			}
			static Properties &Props();

		private:
			ED *ed = nullptr;
		};

	private:
		Properties() = delete;
		Properties(Properties const &) = delete;
		Properties(Properties &&) = delete;
		Properties &operator=(Properties const &) = delete;
		Properties &operator=(Properties &&) = delete;
	};
	inline Properties::~Properties() = default;
}

#endif
