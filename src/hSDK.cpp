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

	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExpressionType::None   >(){ return GetNextParam<ACE::Action   , ExpressionType::None>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExpressionType::None   >(){ return GetNextParam<ACE::Condition, ExpressionType::None>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExpressionType::None   >(){ return CNC_GetFirstExpressionParameter(Rd(), Current().exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExpressionType::Integer>(){ return GetNextParam<ACE::Action   , ExpressionType::Integer>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExpressionType::Integer>(){ return GetNextParam<ACE::Condition, ExpressionType::Integer>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExpressionType::Integer>(){ return CNC_GetFirstExpressionParameter(Rd(), Current().exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExpressionType::Float  >(){ return GetNextParam<ACE::Action   , ExpressionType::Float>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExpressionType::Float  >(){ return GetNextParam<ACE::Condition, ExpressionType::Float>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExpressionType::Float  >(){ return CNC_GetFirstExpressionParameter(Rd(), Current().exp_lparam, TYPE_FLOAT); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Action    , ExpressionType::String >(){ return GetNextParam<ACE::Action   , ExpressionType::String>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Condition , ExpressionType::String >(){ return GetNextParam<ACE::Condition, ExpressionType::String>(); }
	template<> std::int32_t Extension::GetFirstParam<ACE::Expression, ExpressionType::String >(){ return CNC_GetFirstExpressionParameter(Rd(), Current().exp_lparam, TYPE_STRING); }

	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExpressionType::None   >(){ return CNC_GetParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExpressionType::None   >(){ return CNC_GetParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExpressionType::None   >(){ return CNC_GetNextExpressionParameter(Rd(), Current().exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExpressionType::Integer>(){ return CNC_GetIntParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExpressionType::Integer>(){ return CNC_GetIntParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExpressionType::Integer>(){ return CNC_GetNextExpressionParameter(Rd(), Current().exp_lparam, TYPE_INT); }
	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExpressionType::Float  >(){ return CNC_GetFloatParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExpressionType::Float  >(){ return CNC_GetFloatParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExpressionType::Float  >(){ return CNC_GetNextExpressionParameter(Rd(), Current().exp_lparam, TYPE_FLOAT); }
	template<> std::int32_t Extension::GetNextParam<ACE::Action    , ExpressionType::String >(){ return CNC_GetStringParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Condition , ExpressionType::String >(){ return CNC_GetStringParameter(Rd()); }
	template<> std::int32_t Extension::GetNextParam<ACE::Expression, ExpressionType::String >(){ return CNC_GetNextExpressionParameter(Rd(), Current().exp_lparam, TYPE_STRING); }
}
namespace
{
	template<typename T>
	using identity = T;

	template<typename T, typename... V>
	struct RAII_Set_impl
	{
		T &t;
		std::tuple<identity<V T::*>...> mop;
		std::tuple<V...> ds;
		RAII_Set_impl(T &t_, std::tuple<V T::*, V, V>... sets)
		: t(t_)
		, mop(std::get<0>(sets)...)
		, ds(std::get<2>(sets)...)
		{
			set(std::get<1>(sets)...);
		}
		~RAII_Set_impl()
		{
			unset(typename gens<sizeof...(V)>::type());
		}
		template<typename... U>
		void set(U... u)
		{
			do_set<0, U...>(u...);
		}
		template<std::size_t i, typename First, typename... Rest>
		void do_set(First first, Rest... rest)
		{
			t.*std::get<i>(mop) = first;
			do_set<i+1, Rest...>(rest...);
		}
		template<std::size_t i, typename Last>
		void do_set(Last last)
		{
			t.*std::get<i>(mop) = last;
		}
		template<std::size_t i>
		void do_set()
		{
		}
		template<std::size_t...> struct seq{};
		template<std::size_t N, std::size_t... S>
		struct gens : gens<N-1, N-1, S...> {};
		template<std::size_t... S>
		struct gens<0, S...>
		{
			using type = seq<S...>;
		};
		template<std::size_t... S>
		void unset(seq<S...>)
		{
			set(std::get<S>(ds)...);
		}
	};
	template<typename T, typename... V>
	auto RAII_Set(T &t, std::tuple<V T::*, V, V>... sets)
	-> RAII_Set_impl<T, V...>
	{
		return RAII_Set_impl<T, V...>(t, sets...);
	}
}
//
