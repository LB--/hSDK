#include "hSDK.hpp"
#include <tuple>

#include "MinGW.hpp"
#include "MMF2API/Ccxhdr.h"

namespace hSDK
{
	struct Extension::RD
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

		Extension &ext;

		RD(ED *ed, createObjectInfo *COB)
		: ext(*reinterpret_cast<Extension *>(0)) //PLACEHOLDER
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
	};

	Extension &Extension::RuntimeInfo::Ext()
	{
		return Rd()->ext;
	}

	auto Extension::CopyString(string const &s) -> string::const_pointer
	{
		auto p = reinterpret_cast<string::pointer>(callRunTimeFunction
		(
			RuntimeInfo::Rd(),
			RFUNCTION_GETSTRINGSPACE_EX,
			0,
			(s.size()+1)*sizeof(string::value_type))
		);
		std::copy(s.c_str(), s.c_str()+s.size()+1, p);
		return p;
	}

	std::int32_t Extension::exp_lparam = 0;

	using ACE = Extension::ACE;
	using ExprT = Extension::ExpressionType;

	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExprT::None   >(){ return GetNextParam<ACE::Action   , ExprT::None>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExprT::None   >(){ return GetNextParam<ACE::Condition, ExprT::None>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExprT::None   >(){ return CNC_GetFirstExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExprT::Integer>(){ return GetNextParam<ACE::Action   , ExprT::Integer>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExprT::Integer>(){ return GetNextParam<ACE::Condition, ExprT::Integer>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExprT::Integer>(){ return CNC_GetFirstExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExprT::Float  >(){ return GetNextParam<ACE::Action   , ExprT::Float>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExprT::Float  >(){ return GetNextParam<ACE::Condition, ExprT::Float>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExprT::Float  >(){ return CNC_GetFirstExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_FLOAT); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExprT::String >(){ return GetNextParam<ACE::Action   , ExprT::String>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExprT::String >(){ return GetNextParam<ACE::Condition, ExprT::String>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExprT::String >(){ return CNC_GetFirstExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_STRING); }

	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExprT::None   >(){ return CNC_GetParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExprT::None   >(){ return CNC_GetParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExprT::None   >(){ return CNC_GetNextExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExprT::Integer>(){ return CNC_GetIntParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExprT::Integer>(){ return CNC_GetIntParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExprT::Integer>(){ return CNC_GetNextExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExprT::Float  >(){ return CNC_GetFloatParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExprT::Float  >(){ return CNC_GetFloatParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExprT::Float  >(){ return CNC_GetNextExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_FLOAT); }
	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExprT::String >(){ return CNC_GetStringParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExprT::String >(){ return CNC_GetStringParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExprT::String >(){ return CNC_GetNextExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_STRING); }
}

//
