#include "hSDK.hpp"
#include <tuple>

#include "Ccxhdr.h"

namespace hSDK
{
	auto Extension::CopyString(string const &s) -> string::const_pointer_type
	{
		auto p = reinterpret_cast<string::pointer_type>(callRunTimeFunction
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

	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExpressionType::None   >(){ return GetNextParam<ACE::Action   , ExpressionType::None>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExpressionType::None   >(){ return GetNextParam<ACE::Condition, ExpressionType::None>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExpressionType::None   >(){ return CNC_GetFirstExpressionParameter(Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExpressionType::Integer>(){ return GetNextParam<ACE::Action   , ExpressionType::Integer>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExpressionType::Integer>(){ return GetNextParam<ACE::Condition, ExpressionType::Integer>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExpressionType::Integer>(){ return CNC_GetFirstExpressionParameter(Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExpressionType::Float  >(){ return GetNextParam<ACE::Action   , ExpressionType::Float>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExpressionType::Float  >(){ return GetNextParam<ACE::Condition, ExpressionType::Float>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExpressionType::Float  >(){ return CNC_GetFirstExpressionParameter(Rd(), exp_lparam, TYPE_FLOAT); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExpressionType::String >(){ return GetNextParam<ACE::Action   , ExpressionType::String>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExpressionType::String >(){ return GetNextParam<ACE::Condition, ExpressionType::String>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExpressionType::String >(){ return CNC_GetFirstExpressionParameter(Rd(), exp_lparam, TYPE_STRING); }

	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExpressionType::None   >(){ return CNC_GetParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExpressionType::None   >(){ return CNC_GetParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExpressionType::None   >(){ return CNC_GetNextExpressionParameter(Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExpressionType::Integer>(){ return CNC_GetIntParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExpressionType::Integer>(){ return CNC_GetIntParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExpressionType::Integer>(){ return CNC_GetNextExpressionParameter(Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExpressionType::Float  >(){ return CNC_GetFloatParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExpressionType::Float  >(){ return CNC_GetFloatParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExpressionType::Float  >(){ return CNC_GetNextExpressionParameter(Rd(), exp_lparam, TYPE_FLOAT); }
	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExpressionType::String >(){ return CNC_GetStringParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExpressionType::String >(){ return CNC_GetStringParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExpressionType::String >(){ return CNC_GetNextExpressionParameter(Rd(), exp_lparam, TYPE_STRING); }
}

//
