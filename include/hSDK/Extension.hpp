#ifndef hSDK_Extension_HeaderPlusPlus
#define hSDK_Extension_HeaderPlusPlus

#include "hSDK.hpp"
#include "hSDK/Properties.hpp"
#include "hSDK/Parameters.hpp"

#include <functional>
#include <algorithm>
#include <map>

namespace hSDK
{
	struct Extension
	{
		virtual ~Extension() = 0;

		using string = hSDK::string;

	private:
		template<typename T, T...>
		struct ignore final
		{
			static constexpr bool value = true;
		};
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
			using type = T;
			static ExpressionType constexpr ExpT = ExpressionType::None;
		};
		template<typename T>
		struct Enforce32bit
		<
			T, typename std::enable_if
			<
				std::is_same<decltype(T::ExpT), ExpressionType>::value,
				void
			>::type
		> final
		{
			using type = T;
			using fr32 = T;
			static ExpressionType constexpr ExpT = T::ExpT;
		};
		template<typename T>
		struct Enforce32bit
		<
			T, typename std::enable_if
			<
				sizeof(T) == 4
			 && !std::is_floating_point<T>::value
			 && !std::is_integral<T>::value
			 && !std::is_same<T, char_t *>::value
			 && std::is_pod<T>::value
			 && !std::is_reference<T>::value,
				void
			>::type
		> final
		{
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
		struct Enforce32bit<string, T> final
		{
			struct implicit_to32 final
			{
				string const s;
				implicit_to32(string const &str)
				: s(str)
				{
				}
				operator std::int32_t() const
				{
					return reinterpret_cast<std::int32_t>(CopyString(s));
				}
			};
			struct implicit_fr32 final
			{
				string const s;
				implicit_fr32(std::int32_t i32)
				: s(reinterpret_cast<char_t const *>(i32))
				{
				}
				operator string() const
				{
					return s;
				}
			};
			using type = string;
			using to32 = implicit_to32;
			using fr32 = implicit_fr32;
			static ExpressionType constexpr ExpT = ExpressionType::String;
		};
		template<typename T>
		struct Enforce32bit
		<
			T, typename std::enable_if
			<
				std::is_integral<T>::value,
				void
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
			T, typename std::enable_if
			<
				std::is_floating_point<T>::value,
				void
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
		template<typename R, typename = void>
		struct safe_return final
		{
			std::function<R ()> f;
			safe_return(std::function<R ()> func)
			: f(func)
			{
			}
			operator std::int32_t()
			{
				return typename Enforce32bit<R>::to32(f());
			}
		};
		template<typename R>
		struct safe_return<void, R> final
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

		template<typename Base, typename Derived>
		using base_check = typename std::enable_if
		<
			std::is_base_of<Base, Derived>::value,
			Derived
		>::type;

		template<typename ExtT, typename R, typename... Args>
		using ExtMFP_helper = R (ExtT::*)(Args...);

	public:
		template<typename ExtT, typename R, typename... Args>
		using ExtMFP = ExtMFP_helper
		<
			base_check<Extension, ExtT>,
			typename Enforce32bit<R>::type,
			typename Enforce32bit<Args>::type...
		>;

		template<ACE CallT, ACE = CallT>
		struct Forwarder_t_helper final
		{
			using type = std::function
			<
				std::int32_t (Extension &ext, std::int32_t param1, std::int32_t param2)
			>;
		};
		template<ACE CallT>
		struct Forwarder_t_helper<ACE::Expression, CallT> final
		{
			using type = std::function
			<
				std::int32_t (Extension &ext, std::int32_t param1)
			>;
		};
		template<ACE CallT>
		using Forwarder_t = typename Forwarder_t_helper<CallT>::type;
		template<ACE CallT>
		struct ExtMF
		: Forwarder_t<CallT>
		, private std::conditional
		<
			CallT == ACE::Condition,
			std::tuple<bool>,
			std::tuple<>
		>::type
		{
			template<typename ExtT, typename R, typename... Args>
			ExtMF(R (ExtT::*mfp)(Args...))
			: Forwarder_t<CallT>
			(
				typename std::conditional
				<
					CallT == ACE::Expression,
					translator<ExtT, caller<ExtT, R, Args...>, std::int32_t>,
					translator<ExtT, caller<ExtT, R, Args...>, std::int32_t, std::int32_t>
				>::type
				(
					caller<ExtT, R, Args...>
					(
						verify<ExtT, R, Args...>(mfp)
					)
				)
			)
			{
			}
			template<typename ExtT, typename R, typename... Args>
			ExtMF(R (ExtT::*mfp)(Args...), typename std::enable_if<CallT == ACE::Condition || std::is_void<ExtT>::value, bool>::type triggered)
			: Forwarder_t<CallT>
			(
				translator<ExtT, caller<ExtT, R, Args...>, std::int32_t, std::int32_t>
				(
					caller<ExtT, R, Args...>
					(
						verify<ExtT, R, Args...>(mfp)
					)
				)
			)
			, std::tuple<bool>(triggered)
			{
			}
			ExtMF(ExtMF const &) = default;
			ExtMF(ExtMF &&) = default;
			ExtMF &operator=(ExtMF const &) = default;
			ExtMF &operator=(ExtMF &&) = default;

			template<bool E = (CallT == ACE::Condition)>
			auto triggerable() const
			-> typename std::enable_if
			<
				(CallT == ACE::Condition) == E,
				bool
			>::type
			{
				return std::get<0>(*this);
			}

		private:
			template<typename ExtT, typename R, typename... Args>
			static auto verify(R (ExtT::*mfp)(Args...))
			-> typename std::enable_if
			<
				(CallT == ACE::Action &&  std::is_same<R, void>::value)
			 || (CallT != ACE::Action && !std::is_same<R, void>::value),
				ExtMFP<ExtT, R, Args...>
			>::type
			{
				return mfp;
			}
			template<typename ExtT, typename func, typename... Args>
			struct translator final
			{
				func f;
				translator(func fun)
				: f(fun)
				{
				}
				std::int32_t operator()(Extension &ext, Args... args)
				{
					return f(*static_cast<ExtT *>(&ext), args...);
				}
			};

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

				
				template<std::size_t J>
				auto tuple_gen()
				{
					return std::tuple<>{};
				}
				template<std::size_t J, typename First, typename... Rest>
				auto tuple_gen()
				{
					return std::tuple_cat
					(
						std::make_tuple<typename Enforce32bit<First>::fr32>
						(static_cast<typename Enforce32bit<First>::fr32>(GetParam<J, Enforce32bit<First>::ExpT>())),
						tuple_gen<J+1, Rest...>()
					);
				}

				template<int... S>
				auto call(ExtT &ext, tuple_unpack::seq<S...>)
				-> typename std::enable_if
				<
					!std::is_same<R, void>::value && ignore<int, S...>::value,
					Enforce32bit<R>
				>::type::to32
				{
					auto params = tuple_gen<0, Args...>();
					return (ext.*mfp)(std::get<S>(params)...);
				}
				template<int... S>
				auto call(ExtT &ext, tuple_unpack::seq<S...>)
				-> typename std::enable_if
				<
					std::is_same<R, void>::value && ignore<int, S...>::value,
					std::int32_t
				>::type
				{
					auto params = tuple_gen<0, Args...>();
					return (ext.*mfp)(std::get<S>(params)...), 0;
				}

				std::int32_t operator()(ExtT &ext, std::int32_t)
				{
					return (*this)(ext, 0, 0);
				}
				std::int32_t operator()(ExtT &ext, std::int32_t, std::int32_t)
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

				std::int32_t operator()(ExtT &ext, std::int32_t)
				{
					return safe_return<R>([&]()->R{return (ext.*mfp)();});
				}
				std::int32_t operator()(ExtT &ext, std::int32_t, std::int32_t)
				{
					return safe_return<R>([&]()->R{return (ext.*mfp)();});
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

				std::int32_t operator()(ExtT &ext, std::int32_t)
				{
					std::int32_t param1 = Params<CallT, Enforce32bit<Arg1>::ExpT>::GetFirst();
					return safe_return<R>([&]() -> R
					{
						return (ext.*mfp)
						(
							typename Enforce32bit<Arg1>::fr32(param1)
						);
					});
				}
				std::int32_t operator()(ExtT &ext, std::int32_t param1, std::int32_t)
				{
					return safe_return<R>([&]() -> R
					{
						return (ext.*mfp)
						(
							typename Enforce32bit<Arg1>::fr32(param1)
						);
					});
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

				std::int32_t operator()(ExtT &ext, std::int32_t param1, std::int32_t param2)
				{
					return safe_return<R>([&]() -> R
					{
						return (ext.*mfp)
						(
							typename Enforce32bit<Arg1>::fr32(param1),
							typename Enforce32bit<Arg2>::fr32(param2)
						);
					});
				}
				std::int32_t operator()(ExtT &ext, std::int32_t)
				{
					std::int32_t param1 = Params<CallT, Enforce32bit<Arg1>::ExpT>::GetFirst();
					std::int32_t param2 = Params<CallT, Enforce32bit<Arg2>::ExpT>::GetNext();
					return safe_return<R>([&]() -> R
					{
						return (ext.*mfp)
						(
							typename Enforce32bit<Arg1>::fr32(param1),
							typename Enforce32bit<Arg2>::fr32(param2)
						);
					});
				}
			};
		};

		using ActionMF     = ExtMF<ACE::Action>;
		using ConditionMF  = ExtMF<ACE::Condition>;
		using ExpressionMF = ExtMF<ACE::Expression>;
		using ACE_ID_t = ::hSDK::ACE_ID_t;
		using Actions_t     = std::map<ACE_ID_t, ActionMF>;
		using Conditions_t  = std::map<ACE_ID_t, ConditionMF>;
		using Expressions_t = std::map<ACE_ID_t, ExpressionMF>;
	protected:
		Extension(Properties const &props)
		{
		}

		void registerActions(Actions_t a)
		{
			a.insert(std::begin(actions), std::end(actions));
			actions.swap(a);
		}
		void registerConditions(Conditions_t c)
		{
			c.insert(std::begin(conditions), std::end(conditions));
			conditions.swap(c);
		}
		void registerExpressions(Expressions_t e)
		{
			e.insert(std::begin(expressions), std::end(expressions));
			expressions.swap(e);
		}

	public:
		void CallMovement(std::uint32_t action, std::int32_t param);
		void Pause();
		void Continue();
		void Destroy();
		bool DebuggerEditDialog(string const &title, std::int32_t &i);
		bool DebuggerEditDialog(string const &title, string &s, std::uint32_t maxlen = 65535);
		void ExecProgram(string const &path, string const &args_107, bool hide = false, bool wait = false);
		void TriggerCondition(ACE_ID_t conditionID);
		void QueueCondition(ACE_ID_t conditionID);
		void UnqueueCondition(ACE_ID_t conditionID);
		string CurrentDrive();
		string CurrentDirectory();
		string CurrentPath();
		string CurrentAppname();
		void RedrawEntireFrame();
		void QueueRedraw();
		void QueueTick();
		void SetPosition(std::int32_t x, std::int32_t y);
//		void SubclassWindow(std::uint32_t num, std::int32_t hook_routine);
//		void *GetAddress(std::uint32_t wp, std::int32_t lp);
//		std::unique_ptr<CallTables, /*mvFree*/> GetCallTables(std::uint32_t wp, std::int32_t lp);

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

		Extension(Extension const &) = delete;
		Extension(Extension &&) = delete;
		Extension &operator=(Extension const &) = delete;
		Extension &operator=(Extension &&) = delete;

		friend struct ::hSDK::Impl;
	};
	inline Extension::~Extension() = default;

	template<typename PropT, typename ExtT>
	bool ImplementSubtype(string const &subtype = T_(""))
	{
		static_assert(std::is_base_of<Properties, PropT>::value, "PropT must extend hSDK::Properties");
		static_assert(std::is_base_of<Extension, ExtT>::value, "ExtT must extend hSDK::Extension");

		//...

		return false;
	}

	template<> std::int32_t Extension::Params<ACE::Action    , ExpressionType::None   >::GetFirst();
	template<> std::int32_t Extension::Params<ACE::Condition , ExpressionType::None   >::GetFirst();
	template<> std::int32_t Extension::Params<ACE::Expression, ExpressionType::None   >::GetFirst();
	template<> std::int32_t Extension::Params<ACE::Action    , ExpressionType::Integer>::GetFirst();
	template<> std::int32_t Extension::Params<ACE::Condition , ExpressionType::Integer>::GetFirst();
	template<> std::int32_t Extension::Params<ACE::Expression, ExpressionType::Integer>::GetFirst();
	template<> std::int32_t Extension::Params<ACE::Action    , ExpressionType::Float  >::GetFirst();
	template<> std::int32_t Extension::Params<ACE::Condition , ExpressionType::Float  >::GetFirst();
	template<> std::int32_t Extension::Params<ACE::Expression, ExpressionType::Float  >::GetFirst();
	template<> std::int32_t Extension::Params<ACE::Action    , ExpressionType::String >::GetFirst();
	template<> std::int32_t Extension::Params<ACE::Condition , ExpressionType::String >::GetFirst();
	template<> std::int32_t Extension::Params<ACE::Expression, ExpressionType::String >::GetFirst();

	template<> std::int32_t Extension::Params<ACE::Action    , ExpressionType::None   >::GetNext();
	template<> std::int32_t Extension::Params<ACE::Condition , ExpressionType::None   >::GetNext();
	template<> std::int32_t Extension::Params<ACE::Expression, ExpressionType::None   >::GetNext();
	template<> std::int32_t Extension::Params<ACE::Action    , ExpressionType::Integer>::GetNext();
	template<> std::int32_t Extension::Params<ACE::Condition , ExpressionType::Integer>::GetNext();
	template<> std::int32_t Extension::Params<ACE::Expression, ExpressionType::Integer>::GetNext();
	template<> std::int32_t Extension::Params<ACE::Action    , ExpressionType::Float  >::GetNext();
	template<> std::int32_t Extension::Params<ACE::Condition , ExpressionType::Float  >::GetNext();
	template<> std::int32_t Extension::Params<ACE::Expression, ExpressionType::Float  >::GetNext();
	template<> std::int32_t Extension::Params<ACE::Action    , ExpressionType::String >::GetNext();
	template<> std::int32_t Extension::Params<ACE::Condition , ExpressionType::String >::GetNext();
	template<> std::int32_t Extension::Params<ACE::Expression, ExpressionType::String >::GetNext();

	extern template struct Extension::Params<ACE::Action    , ExpressionType::None   >;
	extern template struct Extension::Params<ACE::Condition , ExpressionType::None   >;
	extern template struct Extension::Params<ACE::Expression, ExpressionType::None   >;
	extern template struct Extension::Params<ACE::Action    , ExpressionType::Integer>;
	extern template struct Extension::Params<ACE::Condition , ExpressionType::Integer>;
	extern template struct Extension::Params<ACE::Expression, ExpressionType::Integer>;
	extern template struct Extension::Params<ACE::Action    , ExpressionType::Float  >;
	extern template struct Extension::Params<ACE::Condition , ExpressionType::Float  >;
	extern template struct Extension::Params<ACE::Expression, ExpressionType::Float  >;
	extern template struct Extension::Params<ACE::Action    , ExpressionType::String >;
	extern template struct Extension::Params<ACE::Condition , ExpressionType::String >;
	extern template struct Extension::Params<ACE::Expression, ExpressionType::String >;
}

#endif
