#include "hSDK/Parameters.hpp"

#include "MMF2API.hpp"

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

	template<> int const MMF2Params<AgnosticParamsType::AC_Click>::ID           = PARAM_CLICK;
	template<> int const MMF2Params<AgnosticParamsType::AC_Color>::ID           = PARAM_COLOUR;
	template<> int const MMF2Params<AgnosticParamsType::AC_Number>::ID          = PARAM_EXPRESSION;
	template<> int const MMF2Params<AgnosticParamsType::AC_String>::ID          = PARAM_EXPSTRING;
	template<> int const MMF2Params<AgnosticParamsType::AC_Filename>::ID        = PARAM_FILENAME2;
	template<> int const MMF2Params<AgnosticParamsType::AC_Joystick>::ID        = PARAM_JOYDIRECTION;
	template<> int const MMF2Params<AgnosticParamsType::AC_Key>::ID             = PARAM_KEY;
	template<> int const MMF2Params<AgnosticParamsType::AC_Direction>::ID       = PARAM_NEWDIRECTION;
	template<> int const MMF2Params<AgnosticParamsType::AC_Player>::ID          = PARAM_PLAYER;
	template<> int const MMF2Params<AgnosticParamsType::AC_Position>::ID        = PARAM_POSITION;
	template<> int const MMF2Params<AgnosticParamsType::AC_Speed>::ID           = PARAM_SPEED;
	template<> int const MMF2Params<AgnosticParamsType::AC_Time>::ID            = PARAM_TIME;
	template<> int const MMF2Params<AgnosticParamsType::AC_Zone>::ID            = PARAM_ZONE;
	template<> int const MMF2Params<AgnosticParamsType::AC_Custom>::ID          = PARAM_EXTBASE+0;
	template<> int const MMF2Params<AgnosticParamsType::C_NumberComparison>::ID = PARAM_COMPARAISON;
	template<> int const MMF2Params<AgnosticParamsType::C_StringComparison>::ID = PARAM_CMPSTRING;
	template<> int const MMF2Params<AgnosticParamsType::C_TimeComparison>::ID   = PARAM_CMPTIME;
	template<> int const MMF2Params<AgnosticParamsType::E_Number>::ID           = EXPPARAM_LONG;
	template<> int const MMF2Params<AgnosticParamsType::E_String>::ID           = EXPPARAM_STRING;

	template<> int const MMF2Params<AgnosticParamsType::AC_Object>::ID     = PARAM_OBJECT;
	template<> int const MMF2Params<AgnosticParamsType::AC_Border>::ID     = PARAM_BORDER;
	template<> int const MMF2Params<AgnosticParamsType::AC_Create>::ID     = PARAM_CREATE;
	template<> int const MMF2Params<AgnosticParamsType::AC_Shoot>::ID      = PARAM_SHOOT;
	template<> int const MMF2Params<AgnosticParamsType::AC_SysCreate>::ID  = PARAM_SYSCREATE;
	template<> int const MMF2Params<AgnosticParamsType::AC_Animation>::ID  = PARAM_ANIMATION;
	template<> int const MMF2Params<AgnosticParamsType::AC_None>::ID       = PARAM_NOP;
	template<> int const MMF2Params<AgnosticParamsType::AC_Every>::ID      = PARAM_EVERY;
	template<> int const MMF2Params<AgnosticParamsType::AC_Frame>::ID      = PARAM_FRAME;
	template<> int const MMF2Params<AgnosticParamsType::AC_Sample>::ID     = PARAM_SAMPLE;
	template<> int const MMF2Params<AgnosticParamsType::AC_Music>::ID      = PARAM_MUSIC;
	template<> int const MMF2Params<AgnosticParamsType::AC_SampleLoop>::ID = PARAM_SAMLOOP;
	template<> int const MMF2Params<AgnosticParamsType::AC_MusicLoop>::ID  = PARAM_MUSLOOP;
	template<> int const MMF2Params<AgnosticParamsType::C_Sample>::ID      = PARAM_CNDSAMPLE;
	template<> int const MMF2Params<AgnosticParamsType::C_Music>::ID       = PARAM_CNDMUSIC;
	template<> int const MMF2Params<AgnosticParamsType::AC_Program>::ID    = PARAM_PROGRAM;
	template<> int const MMF2Params<AgnosticParamsType::AC_Paste>::ID      = PARAM_PASTE;
	template<> int const MMF2Params<AgnosticParamsType::AC_InkEffect>::ID  = PARAM_INKEFFECT;
	template<> int const MMF2Params<AgnosticParamsType::AC_Menu>::ID       = PARAM_MENU;
	template<> int const MMF2Params<AgnosticParamsType::AC_Extension>::ID  = PARAM_EXTENSION;
	template<> int const MMF2Params<AgnosticParamsType::AC_Direction8>::ID = PARAM_8DIRECTIONS;
	template<> int const MMF2Params<AgnosticParamsType::AC_Movement>::ID   = PARAM_MVT;
	template<> int const MMF2Params<AgnosticParamsType::AC_ProgramF>::ID   = PARAM_PROGRAM2;
	template<> int const MMF2Params<AgnosticParamsType::AC_Effect>::ID     = PARAM_EFFECT;
}
