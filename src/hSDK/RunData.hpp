#ifndef hSDK_RunData_HeaderPlusPlus
#define hSDK_RunData_HeaderPlusPlus

#include "hSDK/Extension.hpp"

#include "EditData.hpp"

namespace hSDK
{
	struct RD final
	{
		headerObject rHo;
		char r
		[
			sizeof(rCom)+
			sizeof(rMvt)+
			sizeof(rSpr)+
			sizeof(rVal)
		];
		rCom *rc = nullptr;
		rMvt *rm = nullptr;
		rSpr *rs = nullptr;
		rVal *rv = nullptr;

		std::unique_ptr<Extension> ext;

		RD(ED *ed, createObjectInfo *COB)
		{
			unsigned off = 0;
			if(/*Extension::OEFLAGS*/0 & OEFLAG_MOVEMENTS
			|| /*Extension::OEFLAGS*/0 & OEFLAG_SPRITES)
			{
				rc = (rCom *)(r + off);
				off += sizeof(rCom);
			}
			if(/*Extension::OEFLAGS*/0 & OEFLAG_MOVEMENTS)
			{
				rm = (rMvt *)(r + off);
				off += sizeof(rMvt);
			}
			if(/*Extension::OEFLAGS*/0 & OEFLAG_SPRITES)
			{
				rs = (rSpr *)(r + off);
				off += sizeof(rSpr);
			}
			if(/*Extension::OEFLAGS*/0 & OEFLAG_VALUES)
			{
				rv = (rVal *)(r + off);
			}
		}
		~RD() = default;
	};

	auto RuntimeInfo::Ext()
	-> Extension *
	{
		return Rd()->ext.get();
	}
}

#endif
