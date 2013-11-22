#ifndef hSDK_Parameters_HeaderPlusPlus
#define hSDK_Parameters_HeaderPlusPlus

#include "hSDK.hpp"
#include "hSDK/BinaryStream.hpp"

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
		AC_Border,
		AC_Create,
		AC_Shoot,
		AC_SysCreate,
		AC_Animation,
		AC_None,
		AC_Every,
		AC_Frame,
		AC_Sample,
		AC_Music,
		AC_SampleLoop,
		AC_MusicLoop,
		C_Sample,
		C_Music,
		AC_Program,
		AC_Paste,
		AC_InkEffect,
		AC_Menu,
		AC_Extension,
		AC_Direction8,
		AC_Movement,
		AC_ProgramF,
		AC_Effect
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
			Border     = AgnosticParamsType::AC_Border,
			Create     = AgnosticParamsType::AC_Create,
			Shoot      = AgnosticParamsType::AC_Shoot,
			SysCreate  = AgnosticParamsType::AC_SysCreate,
			Animation  = AgnosticParamsType::AC_Animation,
			None       = AgnosticParamsType::AC_None,
			Every      = AgnosticParamsType::AC_Every,
			Frame      = AgnosticParamsType::AC_Frame,
			Sample     = AgnosticParamsType::AC_Sample,
			Music      = AgnosticParamsType::AC_Music,
			SampleLoop = AgnosticParamsType::AC_SampleLoop,
			MusicLoop  = AgnosticParamsType::AC_MusicLoop,
			Program    = AgnosticParamsType::AC_Program,
			Paste      = AgnosticParamsType::AC_Paste,
			InkEffect  = AgnosticParamsType::AC_InkEffect,
			Menu       = AgnosticParamsType::AC_Menu,
			Extension  = AgnosticParamsType::AC_Extension,
			Direction8 = AgnosticParamsType::AC_Direction8,
			Movement   = AgnosticParamsType::AC_Movement,
			ProgramF   = AgnosticParamsType::AC_ProgramF,
			Effect     = AgnosticParamsType::AC_Effect
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
			Border     = AgnosticParamsType::AC_Border,
			Create     = AgnosticParamsType::AC_Create,
			Shoot      = AgnosticParamsType::AC_Shoot,
			SysCreate  = AgnosticParamsType::AC_SysCreate,
			Animation  = AgnosticParamsType::AC_Animation,
			None       = AgnosticParamsType::AC_None,
			Every      = AgnosticParamsType::AC_Every,
			Frame      = AgnosticParamsType::AC_Frame,
			Sample     = AgnosticParamsType::AC_Sample,
			Music      = AgnosticParamsType::AC_Music,
			SampleLoop = AgnosticParamsType::AC_SampleLoop,
			MusicLoop  = AgnosticParamsType::AC_MusicLoop,
			SampleNF   = AgnosticParamsType::C_Sample,
			MusicNF    = AgnosticParamsType::C_Music,
			Program    = AgnosticParamsType::AC_Program,
			Paste      = AgnosticParamsType::AC_Paste,
			InkEffect  = AgnosticParamsType::AC_InkEffect,
			Menu       = AgnosticParamsType::AC_Menu,
			Extension  = AgnosticParamsType::AC_Extension,
			Direction8 = AgnosticParamsType::AC_Direction8,
			Movement   = AgnosticParamsType::AC_Movement,
			ProgramF   = AgnosticParamsType::AC_ProgramF,
			Effect     = AgnosticParamsType::AC_Effect
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
		struct Click final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Click(std::int32_t);

			enum struct Button
			{
				Left,
				Middle,
				Right
			};
			Button button;
			bool doubleclick;
		};
		struct Color final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Color(std::int32_t);

			std::uint32_t rgb;
			std::uint8_t r, g, b;
		};
		struct Filename final
		{
			static ExpressionType constexpr ExpT = ExpressionType::String;
			Filename(std::int32_t);

			string fname;
			operator string() const
			{
				return fname;
			}
		};
		struct Joystick final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Joystick(std::int32_t);

			bool up, down, left, right;
			bool fire1, fire2, fire3, fire4;
		};
		struct Key final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Key(std::int32_t i32)
			: vk(i32)
			{
			}

			std::int32_t vk;
		};
		struct Direction final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Direction(std::int32_t i32)
			: dir32(i32)
			{
			}

			std::int32_t dir32;
		};
		struct Player final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Player(std::int32_t i32)
			: player(i32)
			{
			}

			std::int32_t player;
		};
		struct Position final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Position(std::int32_t);

			std::int16_t x, y;
		};
		struct Speed final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Speed(std::int32_t i32)
			: speed(i32)
			{
			}

			std::int32_t speed;
		};
		struct Time final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Time(std::int32_t i32)
			: millis(i32)
			{
			}

			std::int32_t millis;
		};
		struct Zone final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Zone(std::int32_t);

			std::int16_t x1, y1;
			std::int16_t x2, y2;
		};
		template<typename T>
		struct Comparison final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Comparison(std::int32_t)
			{
			}
		};
		template<std::int16_t ID>
		struct Custom
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Custom(std::int32_t);

			BinaryStream::ParamData::In data;
		};

		struct Object final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Object(std::int32_t);
		};
		struct Border final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Border(std::int32_t);
		};
		struct Create final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Create(std::int32_t);

			struct Position final
			{
				std::int16_t parent_object_info_number;
				std::int16_t flags;
				std::int16_t x, y;
				std::int16_t slope;
				std::int16_t angle;
				std::int32_t dir;
				std::int16_t type_parent;
				std::int16_t object_info_list;
				std::int16_t layer;
			};
			Position pos;
			std::int16_t frame_item_instance;
			std::int16_t object_info_number;
		};
		struct Shoot final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Shoot(std::int32_t);

			Create create;
			std::int16_t speed;
		};
		struct SysCreate final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			SysCreate(std::int32_t);
		};
		struct Animation final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Animation(std::int32_t);
		};
		struct None final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			None(std::int32_t);
		};
		struct Every final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Every(std::int32_t);
		};
		struct Frame final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Frame(std::int32_t);
		};
		struct Sample final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Sample(std::int32_t);

			string name;
			std::int16_t handle;
			bool uninterruptable, bad, iPhone_AudioPlayer, iPhone_OpenAL;
		};
		struct Music final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Music(std::int32_t);

			string name;
			std::int16_t handle;
			bool uninterruptable, bad, iPhone_AudioPlayer, iPhone_OpenAL;
		};
		struct SampleLoop final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			SampleLoop(std::int32_t);
		};
		struct MusicLoop final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			MusicLoop(std::int32_t);
		};
		struct SampleNF final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			SampleNF(std::int32_t);

			string name;
			std::int16_t handle;
		};
		struct MusicNF final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			MusicNF(std::int32_t);

			string name;
			std::int16_t handle;
		};
		struct Program final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Program(std::int32_t);

			string path;
			string arguments;
			bool wait, hide;
		};
		struct Paste final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Paste(std::int32_t);
		};
		struct InkEffect final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			InkEffect(std::int32_t);
		};
		struct Menu final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Menu(std::int32_t);
		};
		struct Extension final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Extension(std::int32_t);
		};
		struct Direction8 final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Direction8(std::int32_t);
		};
		struct Movement final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Movement(std::int32_t);

			std::int16_t num;
			string name;
		};
		struct ProgramF final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			ProgramF(std::int32_t);

			bool wait, hide;
		};
		struct Effect final
		{
			static ExpressionType constexpr ExpT = ExpressionType::None;
			Effect(std::int32_t);
		};
	}

	template<AgnosticParamsType ParamT>
	struct MMF2Params final
	{
		static int const ID;
	};

	template<> extern int const MMF2Params<AgnosticParamsType::AC_Click>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_Color>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_Number>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_String>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_File>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_Joystick>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_Key>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_Direction>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_Player>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_Position>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_Speed>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_Time>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_Zone>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::AC_Custom>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::C_NumberComparison>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::C_StringCOmparison>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::C_TimeComparison>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::E_Number>::ID;
	template<> extern int const MMF2Params<AgnosticParamsType::E_String>::ID;

	extern template struct MMF2Params<AgnosticParamsType::AC_Click>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Color>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Number>;
	extern template struct MMF2Params<AgnosticParamsType::AC_String>;
	extern template struct MMF2Params<AgnosticParamsType::AC_File>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Joystick>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Key>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Direction>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Player>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Position>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Speed>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Time>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Zone>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Custom>;
	extern template struct MMF2Params<AgnosticParamsType::C_NumberComparison>;
	extern template struct MMF2Params<AgnosticParamsType::C_StringCOmparison>;
	extern template struct MMF2Params<AgnosticParamsType::C_TimeComparison>;
	extern template struct MMF2Params<AgnosticParamsType::E_Number>;
	extern template struct MMF2Params<AgnosticParamsType::E_String>;

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
		static std::int16_t constexpr ID = MMF2Params<APT>::ID;
	};
	#define TypeMap(CallT, ParamT, Specific) \
		template<> \
		struct TypeFrom<ACE::CallT, ParamT> final \
		{ \
			static ParamsType<ACE::CallT> constexpr PT = ParamsType<ACE::CallT>::Specific; \
			static AgnosticParamsType constexpr APT = static_cast<AgnosticParamsType>(PT); \
			static std::int16_t constexpr ID = MMF2Params<APT>::ID; \
		}
	#define TypeMapAC(AC) \
		TypeMap(AC, Param::Click, Click); \
		TypeMap(AC, Param::Color, Color); \
		TypeMap(AC, string, String); \
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
		TypeMap(AC, Param::Sample, Sample) \
		TypeMap(AC, Param::Music, Music) \
		TypeMap(AC, Param::SampleLoop, SampleLoop); \
		TypeMap(AC, Param::MusicLoop, MusicLoop); \
		TypeMap(AC, Param::Program, Program); \
		TypeMap(AC, Param::Paste, Paste); \
		TypeMap(AC, Param::InkEffect, InkEffect); \
		TypeMap(AC, Param::Menu, Menu); \
		TypeMap(AC, Param::Extension, Extension); \
		TypeMap(AC, Param::Direction8, Direction8); \
		TypeMap(AC, Param::Movement, Movement); \
		TypeMap(AC, Param::ProgramF, ProgramF); \
		TypeMap(AC, Param::Effect, Effect)
	TypeMapAC(Action);
	typeMapAC(Condition);
	TypeMap(Condition, Param::Comparison<string>, StringComparison);
	TypeMap(Condition, Param::Comparison<Param::Time>, TimeComparison);
	TypeMap(Condition, Param::SampleNF, SampleNF);
	TypeMap(Condition, Param::MusicNF, MusicNF);
	TypeMap(Expression, string, String);
	#undef TypeMapAC
	#undef TypeMap
	template<ACE CallT, std::int16_t CustomID>
	struct TypeFrom
	<
		CallT,
		Param::Custom<CustomID>,
		typename std::enable_if
		<
			CallT != ACE::Expression
		 && CustomID >= 0,
			void
		>::type
	> final
	{
		static ParamsType<CallT> constexpr PT = ParamsType<CallT>::Custom;
		static AgnosticParamsType constexpr APT = static_cast<AgnosticParamsType>(PT);
		static std::int16_t constexpr CustomParamID = CustomID;
		static std::int16_t constexpr ID = MMF2Params<APT>::ID + CustomParamID;
	};
}

#endif
