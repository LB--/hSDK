#ifndef hSDK_HeaderPlusPlus
#define hSDK_HeaderPlusPlus
#include <string>
#include <tuple>
#include <type_traits>

class mv;

namespace hSDK
{
	static_assert(sizeof(void *) == 4 && sizeof(std::size_t) == 4, "MMF2 only supports 32-bit extensions");

#ifdef UNICODE
	using char_t = wchar_t;
	#define T_ u
#else
	using char_t = char;
	#define T_ u8
#endif
	using string = std::basic_string<char_t>;

	struct ED;
	struct RD;

	struct tuple_unpack final
	{
		template<int...> struct seq {};
		template<int N, int... S> struct gens : gens<N-1, N-1, S...> {};
		template<int... S> struct gens<0, S...>
		{
			using type = seq<S...>;
		};
	};

	template<typename T>
	using identity = std::common_type<T>;

	template<typename T, typename... V>
	struct RAII_Set_impl final
	{
		T &t;
		std::tuple<typename identity<V T::*>::type...> mop;
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
			unset(typename tuple_unpack::gens<sizeof...(V)>::type());
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
		template<std::size_t... S>
		void unset(tuple_unpack::seq<S...>)
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

#endif
