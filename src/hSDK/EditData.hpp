#ifndef hSDK_EditData_HeaderPlusPlus
#define hSDK_EditData_HeaderPlusPlus

#include "hSDK/Properties.hpp"

#include "MinGW.hpp"
#include "MMF2API/Ccxhdr.h"

namespace hSDK
{
	struct ED
	{
		extHeader h;
		char data[1];
		//...data...
	};
}

#endif
