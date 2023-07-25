#pragma once
#include "Constant/Constant.h"

#define SKY_POS_Y (-10.0f)

enum class SKY_ID :int{
	MONO,
	NUM
};

#define ISKY_ID (int)SKY_ID

struct SkyParameter {
	CONSTANT_STRING(64) FILE_PATH;
};

static CONSTANT_ARRAY(SkyParameter, ISKY_ID::NUM) SKY_DATABASE = {
	"Data/Sky/Day.x",
};
