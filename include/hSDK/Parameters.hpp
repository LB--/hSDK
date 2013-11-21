#ifndef hSDK_Parameters_HeaderPlusPlus
#define hSDK_Parameters_HeaderPlusPlus

#include "hSDK.hpp"

namespace hSDK
{
	enum struct AgnosticParamsType
	{
		AC_Click,
		AC_Color,
		AC_Number,
		AC_String,
		AC_File,
		AC_Joystick,
		AC_Key,
		AC_Direction,
		AC_Player,
		AC_Position,
		AC_Speed,
		AC_Time,
		AC_Zone,
		AC_Custom,
		C_NumberComparison,
		C_StringComparison,
		C_TimeComparison,
		E_Number,
		E_String,

		//Not in MMF2SDK Documentation:
		AC_Object,
		AC_Create,
		AC_SysCreate,
		AC_Animation,
		AC_None,
		AC_Every,
		AC_Shoot,
		AC_Frame,
		AC_SampleLoop,
		AC_MusicLoop,
		AC_Program,
	};
	template<ACE CallT>
	struct Params;
	template<>
	struct Params<ACE::Action> final
	{
		enum struct Type : typename std::underlying_type<AgnosticParamsType>::type
		{
			Click     = AgnosticParamsType::AC_Click,
			Color     = AgnosticParamsType::AC_Color,
			Number    = AgnosticParamsType::AC_Number,
			String    = AgnosticParamsType::AC_String,
			File      = AgnosticParamsType::AC_File,
			Joystick  = AgnosticParamsType::AC_Joystick,
			Key       = AgnosticParamsType::AC_Key,
			Direction = AgnosticParamsType::AC_Direction,
			Player    = AgnosticParamsType::AC_Player,
			Position  = AgnosticParamsType::AC_Position,
			Speed     = AgnosticParamsType::AC_Speed,
			Time      = AgnosticParamsType::AC_Time,
			Zone      = AgnosticParamsType::AC_Zone,
			Custom    = AgnosticParamsType::AC_Custom,

			Object     = AgnosticParamsType::AC_Object,
			Create     = AgnosticParamsType::AC_Create,
			SysCreate  = AgnosticParamsType::AC_SysCreate,
			Animation  = AgnosticParamsType::AC_Animation,
			None       = AgnosticParamsType::AC_None,
			Every      = AgnosticParamsType::AC_Every,
			Shoot      = AgnosticParamsType::AC_Shoot,
			Frame      = AgnosticParamsType::AC_Frame,
			SampleLoop = AgnosticParamsType::AC_SampleLoop,
			MusicLoop  = AgnosticParamsType::AC_MusicLoop,
			Program    = AgnosticParamsType::AC_Program
		};
	};
	template<>
	struct Params<ACE::Condition> final
	{
		enum struct Type : typename std::underlying_type<AgnosticParamsType>::type
		{
			Click            = AgnosticParamsType::AC_Click,
			Color            = AgnosticParamsType::AC_Color,
			Number           = AgnosticParamsType::AC_Number,
			String           = AgnosticParamsType::AC_String,
			File             = AgnosticParamsType::AC_File,
			Joystick         = AgnosticParamsType::AC_Joystick,
			Key              = AgnosticParamsType::AC_Key,
			Direction        = AgnosticParamsType::AC_Direction,
			Player           = AgnosticParamsType::AC_Player,
			Position         = AgnosticParamsType::AC_Position,
			Speed            = AgnosticParamsType::AC_Speed,
			Time             = AgnosticParamsType::AC_Time,
			Zone             = AgnosticParamsType::AC_Zone,
			Custom           = AgnosticParamsType::AC_Custom,
			NumberComparison = AgnosticParamsType::C_NumberComparison,
			StringComparison = AgnosticParamsType::C_StringComparison,
			TimeComparison   = AgnosticParamsType::C_TimeComparison,

			Object     = AgnosticParamsType::AC_Object,
			Create     = AgnosticParamsType::AC_Create,
			SysCreate  = AgnosticParamsType::AC_SysCreate,
			Animation  = AgnosticParamsType::AC_Animation,
			None       = AgnosticParamsType::AC_None,
			Every      = AgnosticParamsType::AC_Every,
			Shoot      = AgnosticParamsType::AC_Shoot,
			Frame      = AgnosticParamsType::AC_Frame,
			SampleLoop = AgnosticParamsType::AC_SampleLoop,
			MusicLoop  = AgnosticParamsType::AC_MusicLoop,
			Program    = AgnosticParamsType::AC_Program
		};
	};
	template<>
	struct Params<ACE::Expression> final
	{
		enum struct Type : typename std::underlying_type<AgnosticParamsType>::type
		{
			Number = AgnosticParamsType::E_Number,
			String = AgnosticParamsType::E_String
		};
	};
	template<ACE CallT>
	using ParamsType = typename Params<CallT>::Type;

	namespace Param
	{
		struct Click;
		struct Color;
		struct Filename;
		struct Joystick;
		struct Key;
		struct Direction;
		struct Player;
		struct Position;
		struct Speed;
		struct Time;
		struct Zone;
		template<typename T>
		struct Comparison;
		template<typename T>
		struct Custom;

		struct Object;
		struct Create;
		struct SysCreate;
		struct Animation;
		struct None;
		struct Every;
		struct Shoot;
		struct Frame;
		struct SampleLoop;
		struct MusicLoop;
		struct Program;
	}

	template<ACE CallT, typename ParamT, typename = void>
	struct TypeFrom;
	template<ACE CallT, typename ParamT>
	struct TypeFrom
	<
		CallT, ParamT,
		typename std::enable_if
		<
			std::is_integral<ParamT>::value
		 || std::is_floating_point<ParamT>::value,
			void
		>::type
	> final
	{
		static ParamsType<CallT> constexpr PT = ParamsType<CallT>::Number;
		static AgnosticParamsType constexpr APT = static_cast<AgnosticParamsType>(PT);
		static bool constexpr Override = false;
	};
	#define TypeMapO(CallT, ParamT, Specific, Special) \
		template<> \
		struct TypeFrom<ACE::CallT, ParamT> final \
		{ \
			static ParamsType<ACE::CallT> constexpr PT = ParamsType<ACE::CallT>::Specific; \
			static AgnosticParamsType constexpr APT = static_cast<AgnosticParamsType>(PT); \
			static bool constexpr Override = Special; \
		}
	#define TypeMap(CallT, ParamT, Specific) \
		TypeMapO(CallT, ParamT, Specific, true)
	#define TypeMapAC(AC) \
		TypeMap(AC, Param::Click, Click); \
		TypeMap(AC, Param::Color, Color); \
		TypeMapO(AC, string, String, false); \
		TypeMap(AC, Param::Filename, Filename); \
		TypeMap(AC, Param::Joystick, Joystick); \
		TypeMap(AC, Param::Key, Key); \
		TypeMap(AC, Param::Direction, Direction); \
		TypeMap(AC, Param::Player, Player); \
		TypeMap(AC, Param::Position, Position); \
		TypeMap(AC, Param::Speed, Speed); \
		TypeMap(AC, Param::Time, Time); \
		TypeMap(AC, Param::Zone, Zone); \
		\
		TypeMap(AC, Param::Object, Object); \
		TypeMap(AC, Param::Create, Create); \
		TypeMap(AC, Param::SysCreate, SysCreate); \
		TypeMap(AC, Param::Animation, Animation); \
		TypeMap(AC, Param::None, None); \
		TypeMap(AC, Param::Every, Every); \
		TypeMap(AC, Param::Shoot, Shoot); \
		TypeMap(AC, Param::Frame, Frame); \
		TypeMap(AC, Param::SampleLoop, SampleLoop); \
		TypeMap(AC, Param::MusicLoop, MusicLoop); \
		TypeMap(AC, Param::Program, Program)
	TypeMapAC(Action);
	typeMapAC(Condition);
	TypeMap(Condition, Param::Comparison<string>, StringComparison);
	TypeMap(Condition, Param::Comparison<Param::Time>, TimeComparison);
	TypeMapO(Expression, string, String, false);
	#undef TypeMapAC
	#undef TypeMap
	#undef TypeMapO
	template<ACE CallT, typename T>
	struct TypeFrom<CallT, Param::Custom<T>> final
	{
		static ParamsType<CallT> constexpr PT = ParamsType<CallT>::Custom;
		static AgnosticParamsType constexpr APT = static_cast<AgnosticParamsType>(PT);
		static bool constexpr Override = true;
	};

	template<ACE CallT, AgnosticParamsType ParamT>
	struct MMF2Params final
	{
		static int const ID;
	};

	template<> extern int const MMF2Params</**/>::ID;
}

#endif
