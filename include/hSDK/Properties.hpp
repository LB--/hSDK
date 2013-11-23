#ifndef hSDK_Properties_HeaderPlusPlus
#define hSDK_Properties_HeaderPlusPlus

#include "hSDK.hpp"

namespace hSDK
{
	struct Properties
	{
		virtual ~Properties() = 0;

		using string = hSDK::string;

	protected:
		Properties()
		{
		}

	private:
		Properties(Properties const &) = delete;
		Properties(Properties &&) = delete;
		Properties &operator=(Properties const &) = delete;
		Properties &operator=(Properties &&) = delete;
	};
	inline Properties::~Properties() = default;
}

#endif
