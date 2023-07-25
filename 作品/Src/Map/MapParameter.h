#pragma once
#include "Constant/Constant.h"

enum class MAP_ID
{
	MAP_1,
	NUM
};

CONSTANT_ARRAY(CONSTANT_STRING(64), (int)MAP_ID::NUM) StageFilePath = {
	"Data/Stage/Map.x"
};
CONSTANT_ARRAY(CONSTANT_STRING(64), (int)MAP_ID::NUM) StageCollisionFilePath = {
	"Data/Stage/MapCollision.x"
};