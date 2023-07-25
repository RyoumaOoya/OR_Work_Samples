#pragma once
#include "../Constant/Constant.h"

namespace MyLib
{
	enum class BGM_ID :u8 {
		BGM1,
		NUM
	};
#define IBGM_ID (u8)BGM_ID

	enum class SE_ID :u8 {
		BEGIN = 0,
		HIT = 0,
		NUM
	};
#define ISE_ID (u8)SE_ID
	enum class SoundState :u8 {
		Fin = (u8)-2,
		Error = (u8)-1,
		Stop = 0,
		Play,
	};
#define ISoundState (u8)SoundState

	struct SoundParameter {
		constexpr SoundParameter() :Path(""), Volume(255) {}
		constexpr SoundParameter(const char* path, const u8 volume) : Path(path), Volume(volume) {}

		CONSTANT_STRING(64) Path;
		u8 Volume;
	};

	struct SE_Parameter :public SoundParameter
	{
		constexpr SE_Parameter() :SoundParameter(), Radius(0.f) {}
		constexpr SE_Parameter(const char* path, const u8 volume, float radius) : SoundParameter(path, volume), Radius(radius) {}
		float Radius;
	};

	namespace SOUND_PARANETER {
		constexpr auto SE_RANGE = 2500;/*SEÇÃç∂âEÇ÷ÇÃêUÇÍïù*/ static_assert(SE_RANGE <= 10000 && SE_RANGE >= 0, "SE_RANGE Error");
		constexpr auto CENTER_OFFSET = 50;/*íÜâõÇ∆Ç›Ç»Ç∑îÕàÕ*/ static_assert(CENTER_OFFSET <= 10000 && CENTER_OFFSET >= 0, "CENTER_OFFSET Error");
	}

	CONSTANT_ARRAY(SoundParameter, IBGM_ID::NUM) BGM_PARANETER = {
		SoundParameter(),
	};

	CONSTANT_ARRAY(SE_Parameter, ISE_ID::NUM) SE_PARANETER = {
		SE_Parameter("Data/Sound/Hit.wav", 255, 50.f) ,
	};
}