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

		struct RuntimeInfo
		{
			using RD = Extension *;
			static RuntimeInfo &Current()
			{
				static RuntimeInfo rti;
				return rti;
			}
			static RD *Rd()
			{
				return Current.rd;
			}
			static Extension &Ext()
			{
				return **Current().rd;
			}

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

		template<typename T>
		struct Enforce32bit final
		{
			static_assert(sizeof(T) == 4, "Type is not compatible with MMF2");
			struct reinterpret_to32 final
			{
				T t;
				reinterpret_to32(T const &from)
				: t(from);
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
				: i(i32);
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
			static_assert(false, "MMF2 does not support pass-by-reference");
		};
		template<>
		struct Enforce32bit<char *> final
		{
			//Don't allow pointers to non-const characters
		};
		template<>
		struct Enforce32bit<string> final
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
					return reinterpret_cast<std::in32_t>(CopyString(s));
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
			>::type
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
			>::type
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
				implicit_to32(std::int32_t i)
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
		template<typename R>
		struct safe_return final
		{
			std::function<R ()> f;
			safe_return(std::function<R ()> func)
			: f(func);
			{
			}
			operator std::int32_t()
			{
				return Enforce32bit<R>::to32(f());
			}
		};
		template<>
		struct safe_return<void> final
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
		struct null_returning_void_t   final { null_returning_void_t  (std::nullptr_t){} };
		struct null_returning_int_t    final { null_returning_int_t   (std::nullptr_t){} };
		struct null_returning_float_t  final { null_returning_float_t (std::nullptr_t){} };
		struct null_returning_string_t final { null_returning_string_t(std::nullptr_t){} };
		static null_returning_void_t   const   null_returning_void   = nullptr;
		static null_returning_int_t    const   null_returning_int    = nullptr;
		static null_returning_float_t  const   null_returning_float  = nullptr;
		static null_returning_string_t const   null_returning_string = nullptr;

		template<typename ExtT, typename R, typename... Args>
		using ExtMFP =
			typename Enforce32bit<R>::type //return
			(base_check<Extension<>, ExtT>::*) //member function pointer
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
			template<typename = typename std::enable_if<CallT == ACE::Action>::type>
			ExtMF(null_returning_void_t)
			: Forwarder_t(&null_forwarder<ExpressionType::None>)
			{
			}
			template<typename = typename std::enable_if<CallT != ACE::Action>::type>
			ExtMF(null_returning_int_t)
			: Forwarder_t(&null_forwarder<ExpressionType::Int>)
			{
			}
			template<typename = typename std::enable_if<CallT != ACE::Action>::type>
			ExtMF(null_returning_float_t)
			: Forwarder_t(&null_forwarder<ExpressionType::Float>)
			{
			}
			template<typename = typename std::enable_if<CallT != ACE::Action>::type>
			ExtMF(null_returning_string_t)
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

			template<typename ExtT, typename R, typename... Args, std::size_t I = sizeof...(Args)>
			struct caller final
			{
				ExtMFP<ExtT, R, Args...> mfp;
				caller(ExtMFP<ExtT, R, Args...> mf)
				: mfp(mf)
				{
				}

				template<std::size_t, ExpressionType ExpT>
				std::uint32_t GetParam()
				{
					return GetNextParam<CallT, ExpT>();
				}
				template<ExpressionType ExpT>
				std::uint32_t GetParam<0, ExpT>
				{
					return GetFirstParam<CallT, ExpT>();
				}

				template<std::size_t J, typename First, typename... Rest>
				auto tuple_gen()
				-> decltype(std::tuple_cat
				(
					std::make_tuple<Enforce32bit<First>::fr32>
						(GetParam<J, Enforce32bit<First>::ExpT>()),
					tuple_gen<J+1, Enforce32bit<Rest>::fr32...>()
				))
				{
					return std::tuple_cat
					(
						std::make_tuple<Enforce32bit<First>::fr32>
							(GetParam<J, Enforce32bit<First>::ExpT>()),
						tuple_gen<J+1, Enforce32bit<Rest>::fr32...>()
					);
				}
				template<std::size_t J, typename Last>
				auto tuple_gen() -> std::tuple<Enforce32bit<Last>::fr32>
				{
					return std::make_tuple<Enforce32bit<Last>::fr32>
						(GetParam<J, Enforce32bit<Last>::ExpT>());
				}

				template<int... S>
				auto call(Extension &ext, tuple_unpack::seq<S...>)
				-> typename std::enable_if<!std::is_same<R, void>, Enforce32bit<R>::to32>::type
				{
					auto params = tuple_gen<0, Args...>();
					return (ext.*mfp)(std::get<S>(params)...);
				}
				template<int... S>
				auto call(Extension &ext, tuple_unpack::seq<S...>)
				-> typename std::enable_if<std::is_same<R, void>, std::int32_t>::type
				{
					auto params = tuple_gen<0, Args...>();
					return (ext.*mfp)(std::get<S>(params)...), 0;
				}

				std::int32_t operator()(Extension &ext, std::int32_t, std::int32_t)
				{
					return call(ext, typename gens<I>::type());
				}
			};
			template<typename ExtT, typename R, typename... Args>
			struct caller<ExtT, R, Args..., 0> final
			{
				ExtMFP<ExtT, R, Args...> mfp;
				caller(ExtT &e, ExtMFP<ExtT, R, Args...> mf)
				: ext(e)
				, mfp(mf)
				{
				}

				auto operator()(Extension &ext, std::int32_t, std::int32_t)
				-> typename std::enable_if<!std::is_same<R, void>, std::int32_t>::type
				{
					return Enforce32bit<R>::to32((ext.*mfp)());
				}
			};
			template<typename ExtT, typename R, typename... Args>
			struct caller<ExtT, R, Args..., 1> final
			{
				ExtMFP<ExtT, R, Args...> mfp;
				caller(ExtT &e, ExtMFP<ExtT, R, Args...> mf)
				: ext(e)
				, mfp(mf)
				, param1(p1)
				{
				}

				template<typename First, typename...>
				using A1_t = First;

				auto operator()(Extension &ext, std::int32_t param1, std::int32_t)
				-> typename std::enable_if<!std::is_same<R, void>, std::int32_t>::type
				{
					return Enforce32bit<R>::to32((ext.*mfp)
					(
						Enforce32bit<A1_t<Args...>>::fr32(param1)
					));
				}
			};
			template<typename ExtT, typename R, typename... Args>
			struct caller<ExtT, R, Args..., 2> final
			{
				ExtMFP<ExtT, R, Args...> mfp;
				caller(ExtT &e, ExtMFP<ExtT, R, Args...> mf)
				: ext(e)
				, mfp(mf)
				{
				}

				template<typename First, typename...>
				using A1_t = First;
				template<typename, typename Second, typename...>
				using A2_t = Second;

				auto operator()(Extension &ext, std::int32_t param1, std::int32_t param2)
				-> typename std::enable_if<!std::is_same<R, void>, std::int32_t>::type
				{
					return Enforce32bit<R>::to32((ext.*mfp)
					(
						Enforce32bit<A1_t<Args...>>::fr32(param1),
						Enforce32bit<A2_t<Args...>>::fr32(param2)
					));
				}
			};
		};

		using Actions_t     = std::map<std::size_t, ExtMF<ACE::Action>>;
		using Conditions_t  = std::map<std::size_t, ExtMF<ACE::Condition>>;
		using Expressions_t = std::map<std::size_t, ExtMF<ACE::Expression>>;
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

		static string::const_pointer_type CopyString(string const &s);

		static std::int32_t exp_lparam;
		template<ACE CallT, ExpressionType ExpT>
		static std::int32_t GetFirstParam();
		template<ACE CallT, ExpressionType ExpT>
		static std::int32_t GetNextParam();

		Extension() = delete;
		Extension(Extension const &) = delete;
		Extension(Extension &&) = delete;
		Extension &operator=(Extension const &) = delete;
		Extension &operator=(Extension &&) = delete;
	};
	inline Extension::~Extension() = default;
}

#endif
