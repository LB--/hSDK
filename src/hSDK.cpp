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
	template<>
	auto Extension::GetFirstParam<ExpressionType::None>()
	-> std::int32_t
	{
		if(CallType() == ACE::Action || CallType() == ACE::Condtion)
		{
			return GetNextParam<ExpressionType::None>();
		}
		else if(CallType() == ACE::Expression)
		{
			return CNC_GetFirstExpressionParameter(Rd(), Current().exp_lparam, TYPE_INT);
		}
		return 0;
	}
	template<>
	auto Extension::GetFirstParam<ExpressionType::Integer>()
	-> std::int32_t
	{
		if(CallType() == ACE::Action || CallType() == ACE::Condtion)
		{
			return GetNextParam<ExpressionType::Integer>();
		}
		else if(CallType() == ACE::Expression)
		{
			return CNC_GetFirstExpressionParameter(Rd(), Current().exp_lparam, TYPE_INT);
		}
		return 0;
	}
	template<>
	auto Extension::GetFirstParam<ExpressionType::Float>()
	-> std::int32_t
	{
		if(CallType() == ACE::Action || CallType() == ACE::Condtion)
		{
			return GetNextParam<ExpressionType::Float>();
		}
		else if(CallType() == ACE::Expression)
		{
			return CNC_GetFirstExpressionParameter(Rd(), Current().exp_lparam, TYPE_FLOAT);
		}
		return 0;
	}
	template<>
	auto Extension::GetFirstParam<ExpressionType::String>()
	-> std::int32_t
	{
		if(CallType() == ACE::Action || CallType() == ACE::Condtion)
		{
			return GetNextParam<ExpressionType::String>();
		}
		else if(CallType() == ACE::Expression)
		{
			return CNC_GetFirstExpressionParameter(Rd(), Current().exp_lparam, TYPE_STRING);
		}
		return 0;
	}
	template<>
	auto Extension::GetNextParam<ExpressionType::None>()
	-> std::int32_t
	{
		if(CallType() == ACE::Action || CallType() == ACE::Condtion)
		{
			return CNC_GetParameter(Rd());
		}
		else if(CallType() == ACE::Expression)
		{
			return CNC_GetNextExpressionParameter(Rd(), Current().exp_lparam, TYPE_INT);
		}
		return 0;
	}
	template<>
	auto Extension::GetNextParam<ExpressionType::Integer>()
	-> std::int32_t
	{
		if(CallType() == ACE::Action || CallType() == ACE::Condtion)
		{
			return CNC_GetIntParameter(Rd());
		}
		else if(CallType() == ACE::Expression)
		{
			return CNC_GetNextExpressionParameter(Rd(), Current().exp_lparam, TYPE_INT);
		}
		return 0;
	}
	template<>
	auto Extension::GetNextParam<ExpressionType::Float>()
	-> std::int32_t
	{
		if(CallType() == ACE::Action || CallType() == ACE::Condtion)
		{
			return CNC_GetFloatParameter(Rd());
		}
		else if(CallType() == ACE::Expression)
		{
			return CNC_GetNextExpressionParameter(Rd(), Current().exp_lparam, TYPE_FLOAT);
		}
		return 0;
	}
	template<>
	auto Extension::GetNextParam<ExpressionType::String>()
	-> std::int32_t
	{
		if(CallType() == ACE::Action || CallType() == ACE::Condtion)
		{
			return CNC_GetStringParameter(Rd());
		}
		else if(CallType() == ACE::Expression)
		{
			return CNC_GetNextExpressionParameter(Rd(), Current().exp_lparam, TYPE_STRING);
		}
		return 0;
	}
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
