#ifndef hSDK_HeaderPlusPlus
#define hSDK_HeaderPlusPlus
#include <cstdint>
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
			static Extension &Ext()
			{
				return **Current().rd;
			}
			static ExpressionType ExpType()
			{
				return Current().et;
			}

		private:
			RD *rd = nullptr;
			ExpressionType et = ExpressionType::None;
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
					auto p = reinterpret_cast<string::pointer_type>(callRunTimeFunction
					(
						RuntimeInfo::Ext(),
						RFUNCTION_GETSTRINGSPACE_EX,
						0,
						(s.size()+1)*sizeof(string::value_type))
					);
					std::copy(s.c_str(), s.c_str()+s.size()+1, p);
					return reinterpret_cast<std::in32_t>(p);
				}
			};
			using type = T;
			using to32 = implicit_to32;
			using fr32 = string;
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
		};

		template<typename Base, typename Derived>
		using base_check = typename std::enable_if
		<
			std::is_base_of<Base, Derived>::value,
			Derived
		>::type;

	public:
		template<typename ExtT, typename R, typename... Args>
		using ExtMFP =
			typename Enforce32bit<R>::type //return
			(base_check<Extension<>, ExtT>::*) //member function pointer
			(typename Enforce32bit<Args>::type...); //arguments

		using Forwarder_t = std::function<std::int32_t ()>;
		struct ExtMF : public Forwarder_t
		{
			template<typename ExtT, typename R, typename... Args>
			ExtMF(ExtMFP<ExtT, R, Args...> mfp)
			: Forwarder_t(Forwarder<ExtT, R, Args...>(mfp))
			{
			}
			ExtMF(std::nullptr_t = nullptr)
			: Forwarder_t(&null_forwarder)
			{
			}
			template<typename ExtT, typename R, typename... Args>
			static Forwarder_t Forwarder(ExtMFP<ExtT, R, Args...> mfp)
			{
				return [&,mfp]() -> std::int32_t
				{
					//
				}
			}
			static std::int32_t null_forwarder()
			{
				switch(RuntimeInfo::ExpType())
				{
				case ExpressionType::Float: return Enforce32bit<float>::to32(0.0f);
				case ExpressionType::String: return Enforce32bit<char const *>::to32("");
				}
				return 0;
			}
		};

		using ACEs_t = std::map<std::size_t, ExtMF>;
	protected:
		Extension(ACEs_t const &ACEs)
		: funcs(ACEs)
		{
		}

	private:
		ACEs_t funcs;

		Extension() = delete;
		Extension(Extension const &) = delete;
		Extension(Extension &&) = delete;
		Extension &operator=(Extension const &) = delete;
		Extension &operator=(Extension &&) = delete;
	};
	inline Extension::~Extension() = default;
}

#endif
