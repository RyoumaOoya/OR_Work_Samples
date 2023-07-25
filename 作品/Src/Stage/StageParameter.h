#pragma once
#include "Constant/Constant.h"
#include "Src/Map/MapParameter.h"
#include "Src/Sky/SkyParameter.h"

enum class STAGE_ID :int
{
	STAGE_1,

	NUM
};

struct STAGE_DATA
{
	constexpr STAGE_DATA(MAP_ID map_ID, SKY_ID sky_ID, Color fog, float fog_s, float fog_e) :
						mapID(map_ID), skyID(sky_ID), fogColor(fog), fogStart(fog_s), fogEnd(fog_e) {}
	MAP_ID mapID;
	SKY_ID skyID;
	Color fogColor;			// フォグカラー
	float fogStart, fogEnd;	// フォグの範囲
};

CONSTANT_ARRAY(STAGE_DATA, (int)STAGE_ID::NUM)STAGE_DATABASE = {
	STAGE_DATA(MAP_ID::MAP_1, SKY_ID::MONO, color::LightSkyBlue , 200.f, 340.f),
};
