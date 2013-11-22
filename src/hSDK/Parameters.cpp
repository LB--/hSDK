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
		//
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
}
