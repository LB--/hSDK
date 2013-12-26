#ifndef hSDK_HeaderPlusPlus
#define hSDK_HeaderPlusPlus

#include <cstdint>
#include <type_traits>
#include <tuple>
#include <string>
#include <memory>

#if __cplusplus > 201103L
using namespace std::string_literals;
#endif

namespace hSDK
{
	static_assert(sizeof(void *) == 4 && sizeof(std::size_t) == 4, "MMF2 only supports 32-bit extensions");

#ifdef UNICODE
	#define IS_UNICODE 1
	#define T_ u
	template<typename, typename T>
	using unicode_type = T;
#else
	#define IS_UNICODE 0
	#define T_ /*u8*/
	template<typename T, typename>
	using unicode_type = T;
#endif
	using string8 = std::string;
	using string16 = std::u16string;
	using string = unicode_type<string8, string16>;
	using char_t = typename string::value_type;

	string8         str_to8fr16(string16 const &s);
	string16        str_to16fr8(string8  const &s);
	string8  inline str_to8    (string8  const &s){ return s;              }
	string8  inline str_to8    (string16 const &s){ return str_to8fr16(s); }
	string16 inline str_to16   (string8  const &s){ return str_to16fr8(s); }
	string16 inline str_to16   (string16 const &s){ return s;              }
#ifdef UNICODE
	string   inline str_fr     (string8  const &s){ return str_to16(s);    }
	string   inline str_fr     (string16 const &s){ return s;              }
#else
	string   inline str_fr     (string8  const &s){ return s;              }
	string   inline str_fr     (string16 const &s){ return str_to8(s);     }
#endif

	string8 EntireFile(string const &filename);

	struct Impl;

	bool isUnicode();
	bool isHWA();

	struct ED;
	struct Properties;
	struct EdittimeInfo
	{
		static EdittimeInfo &Current()
		{
			static EdittimeInfo eti;
			return eti;
		}
		static ED &Ed()
		{
			return *Current().ed;
		}
		static std::unique_ptr<Properties> Props();

	private:
		ED *ed = nullptr;

		EdittimeInfo() = default;
		EdittimeInfo(EdittimeInfo const &) = delete;
		EdittimeInfo(EdittimeInfo &&) = delete;
		EdittimeInfo &operator=(EdittimeInfo const &) = delete;
		EdittimeInfo &operator=(EdittimeInfo &&) = delete;

		friend struct ::hSDK::Impl;
	};
	struct RD;
	struct Extension;
	struct RuntimeInfo
	{
		static RuntimeInfo &Current()
		{
			static RuntimeInfo rti;
			return rti;
		}
		static RD &Rd()
		{
			return *Current().rd;
		}
		static Extension *Ext();

	private:
		RD *rd = nullptr;

		RuntimeInfo() = default;
		RuntimeInfo(RuntimeInfo const &) = delete;
		RuntimeInfo(RuntimeInfo &&) = delete;
		RuntimeInfo &operator=(RuntimeInfo const &) = delete;
		RuntimeInfo &operator=(RuntimeInfo &&) = delete;

		friend struct ::hSDK::Impl;
	};

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

	enum struct ACE
	{
		Action,
		Condition,
		Expression
	};
	using ACE_ID_t = std::uint16_t;
	enum struct ExpressionType
	{
		None,
		Integer,
		Float,
		String
	};
}

#endif
