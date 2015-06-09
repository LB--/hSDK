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
		AC_Filename,
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
	using AgnosticParamsType_t = std::underlying_type<AgnosticParamsType>::type;
	template<ACE CallT>
	struct Params;
	template<>
	struct Params<ACE::Action> final
	{
		enum struct Type
		: AgnosticParamsType_t
		{
			Click     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Click),
			Color     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Color),
			Number    = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Number),
			String    = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_String),
			Filename  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Filename),
			Joystick  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Joystick),
			Key       = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Key),
			Direction = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Direction),
			Player    = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Player),
			Position  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Position),
			Speed     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Speed),
			Time      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Time),
			Zone      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Zone),
			Custom    = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Custom),

			Object     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Object),
			Border     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Border),
			Create     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Create),
			Shoot      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Shoot),
			SysCreate  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_SysCreate),
			Animation  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Animation),
			None       = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_None),
			Every      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Every),
			Frame      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Frame),
			Sample     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Sample),
			Music      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Music),
			SampleLoop = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_SampleLoop),
			MusicLoop  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_MusicLoop),
			Program    = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Program),
			Paste      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Paste),
			InkEffect  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_InkEffect),
			Menu       = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Menu),
			Extension  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Extension),
			Direction8 = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Direction8),
			Movement   = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Movement),
			ProgramF   = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_ProgramF),
			Effect     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Effect)
		};
	};
	template<>
	struct Params<ACE::Condition> final
	{
		enum struct Type
		: AgnosticParamsType_t
		{
			Click            = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Click),
			Color            = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Color),
			Number           = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Number),
			String           = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_String),
			Filename         = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Filename),
			Joystick         = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Joystick),
			Key              = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Key),
			Direction        = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Direction),
			Player           = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Player),
			Position         = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Position),
			Speed            = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Speed),
			Time             = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Time),
			Zone             = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Zone),
			Custom           = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Custom),
			NumberComparison = static_cast<AgnosticParamsType_t>(AgnosticParamsType::C_NumberComparison),
			StringComparison = static_cast<AgnosticParamsType_t>(AgnosticParamsType::C_StringComparison),
			TimeComparison   = static_cast<AgnosticParamsType_t>(AgnosticParamsType::C_TimeComparison),

			Object     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Object),
			Border     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Border),
			Create     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Create),
			Shoot      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Shoot),
			SysCreate  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_SysCreate),
			Animation  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Animation),
			None       = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_None),
			Every      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Every),
			Frame      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Frame),
			Sample     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Sample),
			Music      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Music),
			SampleLoop = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_SampleLoop),
			MusicLoop  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_MusicLoop),
			SampleNF   = static_cast<AgnosticParamsType_t>(AgnosticParamsType::C_Sample),
			MusicNF    = static_cast<AgnosticParamsType_t>(AgnosticParamsType::C_Music),
			Program    = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Program),
			Paste      = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Paste),
			InkEffect  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_InkEffect),
			Menu       = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Menu),
			Extension  = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Extension),
			Direction8 = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Direction8),
			Movement   = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Movement),
			ProgramF   = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_ProgramF),
			Effect     = static_cast<AgnosticParamsType_t>(AgnosticParamsType::AC_Effect)
		};
	};
	template<>
	struct Params<ACE::Expression> final
	{
		enum struct Type
		: AgnosticParamsType_t
		{
			Number = static_cast<AgnosticParamsType_t>(AgnosticParamsType::E_Number),
			String = static_cast<AgnosticParamsType_t>(AgnosticParamsType::E_String)
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
			Custom(std::int32_t i32)
			: data(reinterpret_cast<void *>(i32))
			{
			}

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

	extern int const AC_Click_ID;
	extern int const AC_Color_ID;
	extern int const AC_Number_ID;
	extern int const AC_String_ID;
	extern int const AC_Filename_ID;
	extern int const AC_Joystick_ID;
	extern int const AC_Key_ID;
	extern int const AC_Direction_ID;
	extern int const AC_Player_ID;
	extern int const AC_Position_ID;
	extern int const AC_Speed_ID;
	extern int const AC_Time_ID;
	extern int const AC_Zone_ID;
	extern int const AC_Custom_ID;
	extern int const C_NumberComparison_ID;
	extern int const C_StringComparison_ID;
	extern int const C_TimeComparison_ID;
	extern int const E_Number_ID;
	extern int const E_String_ID;

	template<> int const MMF2Params<AgnosticParamsType::AC_Click>::ID = AC_Click_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Color>::ID = AC_Color_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Number>::ID = AC_Number_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_String>::ID = AC_String_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Filename>::ID = AC_Filename_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Joystick>::ID = AC_Joystick_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Key>::ID = AC_Key_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Direction>::ID = AC_Direction_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Player>::ID = AC_Player_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Position>::ID = AC_Position_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Speed>::ID = AC_Speed_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Time>::ID = AC_Time_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Zone>::ID = AC_Zone_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Custom>::ID = AC_Custom_ID;
	template<> int const MMF2Params<AgnosticParamsType::C_NumberComparison>::ID = C_NumberComparison_ID;
	template<> int const MMF2Params<AgnosticParamsType::C_StringComparison>::ID = C_StringComparison_ID;
	template<> int const MMF2Params<AgnosticParamsType::C_TimeComparison>::ID = C_TimeComparison_ID;
	template<> int const MMF2Params<AgnosticParamsType::E_Number>::ID = E_Number_ID;
	template<> int const MMF2Params<AgnosticParamsType::E_String>::ID = E_String_ID;

	extern template struct MMF2Params<AgnosticParamsType::AC_Click>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Color>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Number>;
	extern template struct MMF2Params<AgnosticParamsType::AC_String>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Filename>;
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
	extern template struct MMF2Params<AgnosticParamsType::C_StringComparison>;
	extern template struct MMF2Params<AgnosticParamsType::C_TimeComparison>;
	extern template struct MMF2Params<AgnosticParamsType::E_Number>;
	extern template struct MMF2Params<AgnosticParamsType::E_String>;

	extern int const AC_Object_ID;
	extern int const AC_Border_ID;
	extern int const AC_Create_ID;
	extern int const AC_Shoot_ID;
	extern int const AC_SysCreate_ID;
	extern int const AC_Animation_ID;
	extern int const AC_None_ID;
	extern int const AC_Every_ID;
	extern int const AC_Frame_ID;
	extern int const AC_Sample_ID;
	extern int const AC_Music_ID;
	extern int const AC_SampleLoop_ID;
	extern int const AC_MusicLoop_ID;
	extern int const C_Sample_ID;
	extern int const C_Music_ID;
	extern int const AC_Program_ID;
	extern int const AC_Paste_ID;
	extern int const AC_InkEffect_ID;
	extern int const AC_Menu_ID;
	extern int const AC_Extension_ID;
	extern int const AC_Direction8_ID;
	extern int const AC_Movement_ID;
	extern int const AC_ProgramF_ID;
	extern int const AC_Effect_ID;

	template<> int const MMF2Params<AgnosticParamsType::AC_Object>::ID = AC_Object_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Border>::ID = AC_Border_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Create>::ID = AC_Create_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Shoot>::ID = AC_Shoot_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_SysCreate>::ID = AC_SysCreate_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Animation>::ID = AC_Animation_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_None>::ID = AC_None_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Every>::ID = AC_Every_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Frame>::ID = AC_Frame_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Sample>::ID = AC_Sample_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Music>::ID = AC_Music_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_SampleLoop>::ID = AC_SampleLoop_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_MusicLoop>::ID = AC_MusicLoop_ID;
	template<> int const MMF2Params<AgnosticParamsType::C_Sample>::ID = C_Sample_ID;
	template<> int const MMF2Params<AgnosticParamsType::C_Music>::ID = C_Music_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Program>::ID = AC_Program_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Paste>::ID = AC_Paste_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_InkEffect>::ID = AC_InkEffect_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Menu>::ID = AC_Menu_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Extension>::ID = AC_Extension_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Direction8>::ID = AC_Direction8_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Movement>::ID = AC_Movement_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_ProgramF>::ID = AC_ProgramF_ID;
	template<> int const MMF2Params<AgnosticParamsType::AC_Effect>::ID = AC_Effect_ID;

	extern template struct MMF2Params<AgnosticParamsType::AC_Object>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Border>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Create>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Shoot>;
	extern template struct MMF2Params<AgnosticParamsType::AC_SysCreate>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Animation>;
	extern template struct MMF2Params<AgnosticParamsType::AC_None>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Every>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Frame>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Sample>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Music>;
	extern template struct MMF2Params<AgnosticParamsType::AC_SampleLoop>;
	extern template struct MMF2Params<AgnosticParamsType::AC_MusicLoop>;
	extern template struct MMF2Params<AgnosticParamsType::C_Sample>;
	extern template struct MMF2Params<AgnosticParamsType::C_Music>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Program>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Paste>;
	extern template struct MMF2Params<AgnosticParamsType::AC_InkEffect>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Menu>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Extension>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Direction8>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Movement>;
	extern template struct MMF2Params<AgnosticParamsType::AC_ProgramF>;
	extern template struct MMF2Params<AgnosticParamsType::AC_Effect>;

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
	};
	#define TypeMap(CallT, ParamT, Specific) \
		template<> \
		struct TypeFrom<ACE::CallT, ParamT> final \
		{ \
			static ParamsType<ACE::CallT> constexpr PT = ParamsType<ACE::CallT>::Specific; \
			static AgnosticParamsType constexpr APT = static_cast<AgnosticParamsType>(PT); \
		};
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
		TypeMap(AC, Param::Effect, Effect);
	TypeMapAC(Action);
	TypeMapAC(Condition);
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
		static int constexpr CustomParamID = CustomID;
	};
}

#endif
