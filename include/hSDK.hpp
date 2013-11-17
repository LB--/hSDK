#ifndef hSDK_HeaderPlusPlus
#define hSDK_HeaderPlusPlus
#include <cstdint>
#include <functional>
#include <type_traits>
#include <algorithm>
#include <map>

namespace hSDK
{
	static_assert(sizeof(void *) == 4 && sizeof(std::size_t) == 4,
	              "MMF2 only supports 32-bit extensions");

	struct Extension
	{
		virtual ~Extension() = 0;

#ifdef UNICODE
		using char_t = wchar_t;
#else
		using char_t = char;
#endif

		using string = std::basic_string<char_t>;

		enum struct ACE
		{
			Action,
			Condition,
			Expression
		};
		enum struct ExpressionType
		{
			None,
			Integer,
			Float,
			String
		};

		struct ED;
		struct RD;

		struct RuntimeInfo
		{
			static RuntimeInfo &Current()
			{
				static RuntimeInfo rti;
				return rti;
			}
			static RD *Rd()
			{
				return Current().rd;
			}
			static Extension &Ext();

		private:
			RD *rd = nullptr;
		};

	private:
		template<typename Fr, typename To>
		struct implicit_cast final
		{
			Fr const &f;
			implicit_cast(Fr const &from)
			: f(from)
			{
			}
			operator To() const
			{
				return static_cast<To>(f);
			}
		};

		template<typename T, typename = void>
		struct Enforce32bit final
		{
			static_assert(sizeof(T) == 4, "Type is not compatible with MMF2");
			struct reinterpret_to32 final
			{
				T t;
				reinterpret_to32(T const &from)
				: t(from)
				{
				}
				operator std::int32_t() const
				{
					return reinterpret_cast<std::int32_t>(t);
				}
			};
			struct reinterpret_fr32 final
			{
				std::int32_t i;
				reinterpret_fr32(std::int32_t i32)
				: i(i32)
				{
				}
				operator T() const
				{
					return reinterpret_cast<T>(i);
				}
			};
			using type = T;
			using to32 = reinterpret_to32;
			using fr32 = reinterpret_fr32;
			static ExpressionType constexpr ExpT = ExpressionType::None;
		};
		template<typename T>
		struct Enforce32bit<T &> final
		{
			//MMF2 doesn't support pass-by-reference
		};
		template<typename T>
		struct Enforce32bit<char *, T> final
		{
			//Don't allow pointers to non-const characters
		};
		template<typename T>
		struct Enforce32bit<string, T> final
		{
			struct implicit_to32 final
			{
				string const &s;
				implicit_to32(string const &str)
				: s(str)
				{
				}
				operator std::int32_t() const
				{
					return reinterpret_cast<std::int32_t>(CopyString(s));
				}
			};
			using type = T;
			using to32 = implicit_to32;
			using fr32 = string;
			static ExpressionType constexpr ExpT = ExpressionType::String;
		};
		template<typename T>
		struct Enforce32bit
		<
			typename std::enable_if
			<
				std::is_integral<T>::value && !std::is_same<T, std::int32_t>::value,
				T
			>::type, T
		> final
		{
			using type = T;
			using to32 = implicit_cast<T, std::int32_t>;
			using fr32 = implicit_cast<std::int32_t, T>;
			static ExpressionType constexpr ExpT = ExpressionType::Integer;
		};
		template<typename T>
		struct Enforce32bit
		<
			typename std::enable_if
			<
				std::is_floating_point<T>::value,
				T
			>::type, T
		> final
		{
			static_assert(sizeof(float) == 4, "MMF2 only supports 32-bit floats");
			struct implicit_to32 final
			{
				float f;
				implicit_to32(T t)
				: f(static_cast<float>(t))
				{
				}
				operator std::int32_t()
				{
					return *reinterpret_cast<std::int32_t *>(&f);
				}
			};
			struct implicit_fr32 final
			{
				float f;
				implicit_fr32(std::int32_t i)
				: f(*reinterpret_cast<float *>(&i))
				{
				}
				operator T()
				{
					return static_cast<T>(f);
				}
			};
			using type = T;
			using to32 = implicit_to32;
			using fr32 = implicit_fr32;
			static ExpressionType constexpr ExpT = ExpressionType::Float;
		};
		template<typename R, typename = void *>
		struct safe_return final
		{
			std::function<R ()> f;
			safe_return(std::function<R ()> func)
			: f(func)
			{
			}
			operator std::int32_t()
			{
				return Enforce32bit<R>::to32(f());
			}
		};
		template<typename R>
		struct safe_return
		<
			typename std::enable_if
			<
				std::is_same<R, void>::value,
				R
			>::type, R
		> final
		{
			std::function<void ()> f;
			safe_return(std::function<void ()> func)
			: f(func)
			{
			}
			operator std::int32_t()
			{
				return f(), 0;
			}
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

		template<typename Base, typename Derived>
		using base_check = typename std::enable_if
		<
			std::is_base_of<Base, Derived>::value,
			Derived
		>::type;

	public:
		struct null_returning_void_t   final { constexpr null_returning_void_t  () = default; };
		struct null_returning_int_t    final { constexpr null_returning_int_t   () = default; };
		struct null_returning_float_t  final { constexpr null_returning_float_t () = default; };
		struct null_returning_string_t final { constexpr null_returning_string_t() = default;};
		static null_returning_void_t           constexpr null_returning_void    {};
		static null_returning_int_t            constexpr null_returning_int     {};
		static null_returning_float_t          constexpr null_returning_float   {};
		static null_returning_string_t         constexpr null_returning_string  {};
		template<int>
		struct invalid_ACE final
		{
			invalid_ACE() = delete;
			invalid_ACE(invalid_ACE const &) = delete;
			invalid_ACE(invalid_ACE &&) = delete;
			invalid_ACE &operator=(invalid_ACE const &) = delete;
			invalid_ACE &operator=(invalid_ACE &&) = delete;
			~invalid_ACE() = default;
		};

		template<typename ExtT, typename R, typename... Args>
		using ExtMFP =
			typename Enforce32bit<R>::type //return
			(base_check<Extension, ExtT>::*) //member function pointer
			(typename Enforce32bit<Args>::type...); //arguments

		using Forwarder_t = std::function
		<
			std::int32_t (Extension &ext, std::int32_t param1, std::int32_t param2)
		>;
		template<ACE CallT>
		struct ExtMF : public Forwarder_t
		{
			template<typename ExtT, typename R, typename... Args>
			ExtMF(ExtMFP
			<
				ExtT,
				typename std::enable_if
				<
					(CallT == ACE::Action &&  std::is_same<R, void>::value)
				 || (CallT != ACE::Action && !std::is_same<R, void>::value),
					R
				>::type,
				Args...
			> mfp)
			: Forwarder_t(caller<ExtT, R, Args...>(mfp))
			{
			}
			ExtMF(typename std::conditional<CallT == ACE::Action, null_returning_void_t, invalid_ACE<0>>::type)
			: Forwarder_t(&null_forwarder<ExpressionType::None>)
			{
			}
			ExtMF(typename std::conditional<CallT != ACE::Action, null_returning_int_t, invalid_ACE<1>>::type)
			: Forwarder_t(&null_forwarder<ExpressionType::Integer>)
			{
			}
			ExtMF(typename std::conditional<CallT != ACE::Action, null_returning_float_t, invalid_ACE<2>>::type)
			: Forwarder_t(&null_forwarder<ExpressionType::Float>)
			{
			}
			ExtMF(typename std::conditional<CallT != ACE::Action, null_returning_string_t, invalid_ACE<3>>::type)
			: Forwarder_t(&null_forwarder<ExpressionType::String>)
			{
			}
			ExtMF(ExtMF const &) = default;
			ExtMF(ExtMF &&) = default;
			ExtMF &operator=(ExtMF const &) = default;
			ExtMF &operator=(ExtMF &&) = default;

		private:
			template<ExpressionType ExpT>
			static std::int32_t null_forwarder(Extension &, std::int32_t, std::int32_t)
			{
				if(CallT == ACE::Expression)
				{
					switch(ExpT)
					{
					case ExpressionType::Float: return Enforce32bit<float>::to32(0.0f);
					case ExpressionType::String: return Enforce32bit<char const *>::to32("");
					}
				}
				return 0;
			}

			template<typename ExtT, typename R, typename... Args>
			struct caller final
			{
				static std::size_t constexpr I = sizeof...(Args);
				ExtMFP<ExtT, R, Args...> mfp;
				caller(ExtMFP<ExtT, R, Args...> mf)
				: mfp(mf)
				{
				}

				template<std::size_t, ExpressionType ExpT>
				struct GetParam final
				{
					std::int32_t p;
					GetParam()
					: p(Params<CallT, ExpT>::GetNext())
					{
					}
					operator std::int32_t()
					{
						return p;
					}
				};
				template<ExpressionType ExpT>
				struct GetParam<0, ExpT> final
				{
					std::int32_t p;
					GetParam()
					: p(Params<CallT, ExpT>::GetFirst())
					{
					}
					operator std::int32_t()
					{
						return p;
					}
				};

				
				template<std::size_t J, typename Last>
				auto tuple_gen() -> std::tuple<typename Enforce32bit<Last>::fr32>
				{
					return std::make_tuple<typename Enforce32bit<Last>::fr32>
						(GetParam<J, Enforce32bit<Last>::ExpT>());
				}
				template<std::size_t J, typename First, typename... Rest>
				auto tuple_gen()
				-> decltype(std::tuple_cat
				(
					std::make_tuple<typename Enforce32bit<First>::fr32>
						(GetParam<J, Enforce32bit<First>::ExpT>()),
					tuple_gen<J+1, typename Enforce32bit<Rest>::fr32...>()
				))
				{
					return std::tuple_cat
					(
						std::make_tuple<typename Enforce32bit<First>::fr32>
							(GetParam<J, Enforce32bit<First>::ExpT>()),
						tuple_gen<J+1, typename Enforce32bit<Rest>::fr32...>()
					);
				}

				template<int... S>
				auto call(Extension &ext, tuple_unpack::seq<S...>)
				-> typename std::enable_if
				<
					!std::is_same<R, void>::value,
					typename Enforce32bit<R>::to32
				>::type
				{
					auto params = tuple_gen<0, Args...>();
					return (ext.*mfp)(std::get<S>(params)...);
				}
				template<int... S>
				auto call(Extension &ext, tuple_unpack::seq<S...>)
				-> typename std::enable_if
				<
					std::is_same<R, void>::value,
					std::int32_t
				>::type
				{
					auto params = tuple_gen<0, Args...>();
					return (ext.*mfp)(std::get<S>(params)...), 0;
				}

				std::int32_t operator()(Extension &ext, std::int32_t, std::int32_t)
				{
					return call(ext, typename tuple_unpack::gens<I>::type());
				}
			};
			template<typename ExtT, typename R>
			struct caller<ExtT, R> final
			{
				ExtMFP<ExtT, R> mfp;
				caller(ExtMFP<ExtT, R> mf)
				: mfp(mf)
				{
				}

				auto operator()(Extension &ext, std::int32_t, std::int32_t)
				-> typename std::enable_if
				<
					!std::is_same<R, void>::value,
					std::int32_t
				>::type
				{
					return Enforce32bit<R>::to32((ext.*mfp)());
				}
			};
			template<typename ExtT, typename R, typename Arg1>
			struct caller<ExtT, R, Arg1> final
			{
				ExtMFP<ExtT, R, Arg1> mfp;
				caller(ExtMFP<ExtT, R, Arg1> mf)
				: mfp(mf)
				{
				}

				auto operator()(Extension &ext, std::int32_t param1, std::int32_t)
				-> typename std::enable_if
				<
					!std::is_same<R, void>::value,
					std::int32_t
				>::type
				{
					return Enforce32bit<R>::to32((ext.*mfp)
					(
						Enforce32bit<Arg1>::fr32(param1)
					));
				}
			};
			template<typename ExtT, typename R, typename Arg1, typename Arg2>
			struct caller<ExtT, R, Arg1, Arg2> final
			{
				ExtMFP<ExtT, R, Arg1, Arg2> mfp;
				caller(ExtMFP<ExtT, R, Arg1, Arg2> mf)
				: mfp(mf)
				{
				}

				auto operator()(Extension &ext, std::int32_t param1, std::int32_t param2)
				-> typename std::enable_if
				<
					!std::is_same<R, void>::value,
					std::int32_t
				>::type
				{
					return Enforce32bit<R>::to32((ext.*mfp)
					(
						Enforce32bit<Arg1>::fr32(param1),
						Enforce32bit<Arg2>::fr32(param2)
					));
				}
			};
		};

		using Actions_t     = std::map<std::uint16_t, ExtMF<ACE::Action>>;
		using Conditions_t  = std::map<std::uint16_t, ExtMF<ACE::Condition>>;
		using Expressions_t = std::map<std::uint16_t, ExtMF<ACE::Expression>>;
	protected:
		Extension(Actions_t const &a, Conditions_t const &c, Expressions_t const &e)
		: actions(a)
		, conditions(c)
		, expressions(e)
		{
		}

	private:
		Actions_t actions;
		Conditions_t conditions;
		Expressions_t expressions;

		static string::const_pointer CopyString(string const &s);

		static std::int32_t exp_lparam;
		template<ACE CallT, ExpressionType ExpT>
		struct Params final
		{
			static std::int32_t GetFirst();
			static std::int32_t GetNext();
		};

		Extension() = delete;
		Extension(Extension const &) = delete;
		Extension(Extension &&) = delete;
		Extension &operator=(Extension const &) = delete;
		Extension &operator=(Extension &&) = delete;
	};
	inline Extension::~Extension() = default;

	template<> std::int32_t Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::None   >::GetFirst();
	template<> std::int32_t Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::None   >::GetFirst();
	template<> std::int32_t Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::None   >::GetFirst();
	template<> std::int32_t Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::Integer>::GetFirst();
	template<> std::int32_t Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::Integer>::GetFirst();
	template<> std::int32_t Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::Integer>::GetFirst();
	template<> std::int32_t Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::Float  >::GetFirst();
	template<> std::int32_t Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::Float  >::GetFirst();
	template<> std::int32_t Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::Float  >::GetFirst();
	template<> std::int32_t Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::String >::GetFirst();
	template<> std::int32_t Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::String >::GetFirst();
	template<> std::int32_t Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::String >::GetFirst();

	template<> std::int32_t Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::None   >::GetNext();
	template<> std::int32_t Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::None   >::GetNext();
	template<> std::int32_t Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::None   >::GetNext();
	template<> std::int32_t Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::Integer>::GetNext();
	template<> std::int32_t Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::Integer>::GetNext();
	template<> std::int32_t Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::Integer>::GetNext();
	template<> std::int32_t Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::Float  >::GetNext();
	template<> std::int32_t Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::Float  >::GetNext();
	template<> std::int32_t Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::Float  >::GetNext();
	template<> std::int32_t Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::String >::GetNext();
	template<> std::int32_t Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::String >::GetNext();
	template<> std::int32_t Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::String >::GetNext();

	extern template struct Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::None   >;
	extern template struct Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::None   >;
	extern template struct Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::None   >;
	extern template struct Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::Integer>;
	extern template struct Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::Integer>;
	extern template struct Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::Integer>;
	extern template struct Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::Float  >;
	extern template struct Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::Float  >;
	extern template struct Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::Float  >;
	extern template struct Extension::Params<Extension::ACE::Action    , Extension::ExpressionType::String >;
	extern template struct Extension::Params<Extension::ACE::Condition , Extension::ExpressionType::String >;
	extern template struct Extension::Params<Extension::ACE::Expression, Extension::ExpressionType::String >;
}

#endif
