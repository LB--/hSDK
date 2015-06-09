#include "hSDK/Parameters.hpp"

#include "FusionAPI.hpp"

namespace hSDK
{
	namespace Param
	{
		Click::Click(std::int32_t i32)
		{
			switch(reinterpret_cast<std::int8_t *>(&i32)[0])
			{
			case 0: button = Button::Left;   break;
			case 1: button = Button::Middle; break;
			case 2: button = Button::Right;  break;
			default: break;
			};
			doubleclick = i32 & PARAMCLICK_DOUBLE ? true : false;
		}
		Color::Color(std::int32_t i32)
		: rgb(i32)
		, r(reinterpret_cast<std::int8_t *>(&i32)[0])
		, g(reinterpret_cast<std::int8_t *>(&i32)[1])
		, b(reinterpret_cast<std::int8_t *>(&i32)[2])
		{
		}
		Filename::Filename(std::int32_t i32)
		: fname(reinterpret_cast<char_t *>(i32))
		{
		}
		Joystick::Joystick(std::int32_t i32)
		: up   (i32 & JOYSTICK_UP ? true : false)
		, down (i32 & JOYSTICK_DOWN ? true : false)
		, left (i32 & JOYSTICK_LEFT ? true : false)
		, right(i32 & JOYSTICK_RIGHT ? true : false)
		, fire1(i32 & JOYSTICK_FIRE1 ? true : false)
		, fire2(i32 & JOYSTICK_FIRE2 ? true : false)
		, fire3(i32 & JOYSTICK_FIRE3 ? true : false)
		, fire4(i32 & JOYSTICK_FIRE4 ? true : false)
		{
		}
		Position::Position(std::int32_t i32)
		: x(reinterpret_cast<std::int16_t *>(&i32)[0])
		, y(reinterpret_cast<std::int16_t *>(&i32)[1])
		{
		}
		struct ZoneRect final
		{
			std::int16_t left, top, right, bottom;
		};
		Zone::Zone(std::int32_t i32)
		: x1(reinterpret_cast<ZoneRect *>(i32)->left)
		, y1(reinterpret_cast<ZoneRect *>(i32)->top)
		, x2(reinterpret_cast<ZoneRect *>(i32)->right)
		, y2(reinterpret_cast<ZoneRect *>(i32)->bottom)
		{
		}

		Create::Create(std::int32_t i32)
		: pos
		{
			reinterpret_cast<CreateDuplicateParam *>(i32)->cdpPos.posOINUMParent,
			reinterpret_cast<CreateDuplicateParam *>(i32)->cdpPos.posFlags,
			reinterpret_cast<CreateDuplicateParam *>(i32)->cdpPos.posX,
			reinterpret_cast<CreateDuplicateParam *>(i32)->cdpPos.posY,
			reinterpret_cast<CreateDuplicateParam *>(i32)->cdpPos.posSlope,
			reinterpret_cast<CreateDuplicateParam *>(i32)->cdpPos.posAngle,
			reinterpret_cast<CreateDuplicateParam *>(i32)->cdpPos.posDir,
			reinterpret_cast<CreateDuplicateParam *>(i32)->cdpPos.posTypeParent,
			reinterpret_cast<CreateDuplicateParam *>(i32)->cdpPos.posOiList,
			reinterpret_cast<CreateDuplicateParam *>(i32)->cdpPos.posLayer
		}
		, frame_item_instance(reinterpret_cast<CreateDuplicateParam *>(i32)->cdpHFII)
		, object_info_number (reinterpret_cast<CreateDuplicateParam *>(i32)->cdpOi)
		{
		}
		Shoot::Shoot(std::int32_t i32)
		: create(reinterpret_cast<std::int32_t>(&(reinterpret_cast<ShootParam *>(i32)->shtCdp)))
		, speed(reinterpret_cast<ShootParam *>(i32)->shtSpeed)
		{
		}
		Sample::Sample(std::int32_t i32)
		: name              (reinterpret_cast<LPSND>(i32)->sndName)
		, handle            (reinterpret_cast<LPSND>(i32)->sndHandle)
		, uninterruptable   (reinterpret_cast<LPSND>(i32)->sndFlags & PSOUNDFLAG_UNINTERRUPTABLE ? true : false)
		, bad               (reinterpret_cast<LPSND>(i32)->sndFlags & PSOUNDFLAG_BAD ? true : false)
		, iPhone_AudioPlayer(reinterpret_cast<LPSND>(i32)->sndFlags & PSOUNDFLAG_IPHONE_AUDIOPLAYER ? true : false)
		, iPhone_OpenAL     (reinterpret_cast<LPSND>(i32)->sndFlags & PSOUNDFLAG_IPHONE_OPENAL ? true : false)
		{
		}
		Music::Music(std::int32_t i32)
		: name              (reinterpret_cast<LPSND>(i32)->sndName)
		, handle            (reinterpret_cast<LPSND>(i32)->sndHandle)
		, uninterruptable   (reinterpret_cast<LPSND>(i32)->sndFlags & PSOUNDFLAG_UNINTERRUPTABLE ? true : false)
		, bad               (reinterpret_cast<LPSND>(i32)->sndFlags & PSOUNDFLAG_BAD ? true : false)
		, iPhone_AudioPlayer(reinterpret_cast<LPSND>(i32)->sndFlags & PSOUNDFLAG_IPHONE_AUDIOPLAYER ? true : false)
		, iPhone_OpenAL     (reinterpret_cast<LPSND>(i32)->sndFlags & PSOUNDFLAG_IPHONE_OPENAL ? true : false)
		{
		}
		SampleNF::SampleNF(std::int32_t i32)
		: name              (reinterpret_cast<LPSND>(i32)->sndName)
		, handle            (reinterpret_cast<LPSND>(i32)->sndHandle)
		{
		}
		MusicNF::MusicNF(std::int32_t i32)
		: name              (reinterpret_cast<LPSND>(i32)->sndName)
		, handle            (reinterpret_cast<LPSND>(i32)->sndHandle)
		{
		}
		Program::Program(std::int32_t i32)
		: path     (reinterpret_cast<prgParam *>(i32)->prgPath)
		, arguments(reinterpret_cast<prgParam *>(i32)->prgCommand)
		, wait     (reinterpret_cast<prgParam *>(i32)->prgFlags & PRGFLAGS_WAIT ? true : false)
		, hide     (reinterpret_cast<prgParam *>(i32)->prgFlags & PRGFLAGS_HIDE ? true : false)
		{
		}
		Movement::Movement(std::int32_t i32)
		: num (reinterpret_cast<LPMVTP>(i32)->mvtNumber)
		, name(reinterpret_cast<LPMVTP>(i32)->mvtName)
		{
		}
		ProgramF::ProgramF(std::int32_t i32)
		: wait     (reinterpret_cast<prgParam *>(i32)->prgFlags & PRGFLAGS_WAIT ? true : false)
		, hide     (reinterpret_cast<prgParam *>(i32)->prgFlags & PRGFLAGS_HIDE ? true : false)
		{
		}
	}

	int const AC_Click_ID           = PARAM_CLICK;
	int const AC_Color_ID           = PARAM_COLOUR;
	int const AC_Number_ID          = PARAM_EXPRESSION;
	int const AC_String_ID          = PARAM_EXPSTRING;
	int const AC_Filename_ID        = PARAM_FILENAME2;
	int const AC_Joystick_ID        = PARAM_JOYDIRECTION;
	int const AC_Key_ID             = PARAM_KEY;
	int const AC_Direction_ID       = PARAM_NEWDIRECTION;
	int const AC_Player_ID          = PARAM_PLAYER;
	int const AC_Position_ID        = PARAM_POSITION;
	int const AC_Speed_ID           = PARAM_SPEED;
	int const AC_Time_ID            = PARAM_TIME;
	int const AC_Zone_ID            = PARAM_ZONE;
	int const AC_Custom_ID          = PARAM_EXTBASE+0;
	int const C_NumberComparison_ID = PARAM_COMPARAISON;
	int const C_StringComparison_ID = PARAM_CMPSTRING;
	int const C_TimeComparison_ID   = PARAM_CMPTIME;
	int const E_Number_ID           = EXPPARAM_LONG;
	int const E_String_ID           = EXPPARAM_STRING;

	int const AC_Object_ID     = PARAM_OBJECT;
	int const AC_Border_ID     = PARAM_BORDER;
	int const AC_Create_ID     = PARAM_CREATE;
	int const AC_Shoot_ID      = PARAM_SHOOT;
	int const AC_SysCreate_ID  = PARAM_SYSCREATE;
	int const AC_Animation_ID  = PARAM_ANIMATION;
	int const AC_None_ID       = PARAM_NOP;
	int const AC_Every_ID      = PARAM_EVERY;
	int const AC_Frame_ID      = PARAM_FRAME;
	int const AC_Sample_ID     = PARAM_SAMPLE;
	int const AC_Music_ID      = PARAM_MUSIC;
	int const AC_SampleLoop_ID = PARAM_SAMLOOP;
	int const AC_MusicLoop_ID  = PARAM_MUSLOOP;
	int const C_Sample_ID      = PARAM_CNDSAMPLE;
	int const C_Music_ID       = PARAM_CNDMUSIC;
	int const AC_Program_ID    = PARAM_PROGRAM;
	int const AC_Paste_ID      = PARAM_PASTE;
	int const AC_InkEffect_ID  = PARAM_INKEFFECT;
	int const AC_Menu_ID       = PARAM_MENU;
	int const AC_Extension_ID  = PARAM_EXTENSION;
	int const AC_Direction8_ID = PARAM_8DIRECTIONS;
	int const AC_Movement_ID   = PARAM_MVT;
	int const AC_ProgramF_ID   = PARAM_PROGRAM2;
	int const AC_Effect_ID     = PARAM_EFFECT;
}
