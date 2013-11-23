#include "hSDK/Extension.hpp"

#include "RunData.hpp"

namespace hSDK
{
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

	using ExprT = ExpressionType;

	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::None   >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::None   >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::None   >::GetFirst(){ return CNC_GetFirstExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::Integer>::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::Integer>::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::Integer>::GetFirst(){ return CNC_GetFirstExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::Float  >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::Float  >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::Float  >::GetFirst(){ return CNC_GetFirstExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_FLOAT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::String >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::String >::GetFirst(){ return GetNext(); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::String >::GetFirst(){ return CNC_GetFirstExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_STRING); }

	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::None   >::GetNext(){ return CNC_GetParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::None   >::GetNext(){ return CNC_GetParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::None   >::GetNext(){ return CNC_GetNextExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::Integer>::GetNext(){ return CNC_GetIntParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::Integer>::GetNext(){ return CNC_GetIntParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::Integer>::GetNext(){ return CNC_GetNextExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::Float  >::GetNext(){ return CNC_GetFloatParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::Float  >::GetNext(){ return CNC_GetFloatParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::Float  >::GetNext(){ return CNC_GetNextExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_FLOAT); }
	template<> std::int32_t Extension::Params<ACE::Action    , ExprT::String >::GetNext(){ return CNC_GetStringParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::Params<ACE::Condition , ExprT::String >::GetNext(){ return CNC_GetStringParameter(RuntimeInfo::Rd()); }
	template<> std::int32_t Extension::Params<ACE::Expression, ExprT::String >::GetNext(){ return CNC_GetNextExpressionParameter(RuntimeInfo::Rd(), exp_lparam, TYPE_STRING); }
}
