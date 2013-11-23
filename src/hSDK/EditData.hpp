#ifndef hSDK_EditData_HeaderPlusPlus
#define hSDK_EditData_HeaderPlusPlus

#include "hSDK/Properties.hpp"

#include "MMF2API.hpp"

namespace hSDK
{
	struct ED
	{
		extHeader h;
		char data[1];
		//...data...
	};

	auto EdittimeInfo::Props()
	-> std::unique_ptr<Properties>
	{
		//
	}
}

#endif
