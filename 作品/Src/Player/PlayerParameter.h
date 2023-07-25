#pragma once
#include "Constant/Constant.h"
#include "Math/MyMathData.h"

namespace PLAYER_PARAMETER
{
	CONSTEXPR_STRING(64) PLAYER_FILE_PATH = "Data/Model/Player.x";

	constexpr u32 PLAYER_MAX_HP(1);
	constexpr float PLAYER_INVINCIBLE(0.4f);

	constexpr float PLAYER_RADIUS(1.35f);
	constexpr double PLAYER_SPEED(1.5 * 60);
	constexpr float PLAYER_SPEED_MAX(60.f);
	constexpr float SHOT_INTERVAL(0.1f);
	constexpr Vector PLAYER_SCALE(0.9f, 0.9f, 0.9f);

	constexpr float FIRCTION(1.8f);
	constexpr float GROUND_FIRCTION(3.6f);

	constexpr float PLAYER_JUMP_POWER(0.5f);
	constexpr float GRAVITY_SPEED_MAX(90.f);
}