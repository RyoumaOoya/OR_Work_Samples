#pragma once
#include "Constant/Constant.h"
#include "Math/MyMathData.h"

enum class PRODUCE_OBJECT_ID {
	END = -1,
	ENEMY,
	NUM
};

#define IPRODUCE_OBJECT_ID (int)PRODUCE_OBJECT_ID

struct ProduceObjectParameter {
	constexpr ProduceObjectParameter(const char* str, float interval_min, float interval_max) :Path(str), ProduceIntervalMin(interval_min), ProduceIntervalMax(interval_max) {}
	CONSTANT_STRING(64) Path;
	float ProduceIntervalMin;
	float ProduceIntervalMax;
};

#define PARAM(...) ProduceObjectParameter(__VA_ARGS__)

static CONSTANT_ARRAY(ProduceObjectParameter, IPRODUCE_OBJECT_ID::NUM) PRODUCE_OBJECT_DATABASE = {
	PARAM("", 0.0f, 0.0f),
};

#undef PARAM

struct ProducePositioningData {
	constexpr ProducePositioningData() :ID(PRODUCE_OBJECT_ID::END), Pos() {}
	constexpr ProducePositioningData(PRODUCE_OBJECT_ID id, Vector pos) :ID(id), Pos(pos) {}
	
	PRODUCE_OBJECT_ID ID;
	Vector Pos;
};

#define PARAM(...) ProducePositioningData(__VA_ARGS__)

static CONSTANT_ARRAY(ProducePositioningData, 50) PRODUCE_POSITIONING_DATA = {
	PARAM(PRODUCE_OBJECT_ID::ENEMY, Vector(0.f,0.f,0.f)),
};

#undef PARAM