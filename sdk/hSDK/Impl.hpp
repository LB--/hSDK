#ifndef hSDK_Impl_HeaderPlusPlus
#define hSDK_Impl_HeaderPlusPlus

#include "hSDK.hpp"

class mv;

namespace hSDK
{
	struct Impl
	{
		static ED *&ed;
		static RD *&rd;
		static mv *mV;
	};
}

#endif
