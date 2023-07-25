#pragma once
#include "Math/MyMath.h"
#include "Constant/Constant.h"

enum class EnemyID {
	Enemy1,

	Num
};

#define IEnemyID (int)EnemyID

struct EnemyParameter
{
	constexpr EnemyParameter(const char* path, i32 maxHP, float invincible_time, float radius, float move_speed, float rate)
		:Path(path), MaxHP(maxHP), InvincibleTime(invincible_time), Radius(radius), MoveSpeed(move_speed), PopRate(static_cast<u8>(MIN(1.0f, rate) * 255)) {}
	CONSTANT_STRING(32) Path;	//ファイルパス
	i32 MaxHP;					//最大体力
	float InvincibleTime;		//無敵時間
	float Radius;				//半径
	float MoveSpeed;			//移動速度
	u8 PopRate;					//アイテムのドロップ率
};

CONSTANT_ARRAY(EnemyParameter, IEnemyID::Num) ENEMY_PARAMETER = {
	//				ファイルパス	最大体力 無敵時間	半径  移動速度	ドロップ率
	EnemyParameter("Data/Model/Enemy.x", 1,	 0.1f,		1.1f, 0.1f,	0.3f),
};
