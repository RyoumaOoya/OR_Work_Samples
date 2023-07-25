#pragma once
#include "Constant/Constant.h"
#include "Math/MyMathData.h"
#include "Define.h"

enum class BULLET_ID : i8 {
	Null = -1,
	NORMAL,		//指定された角度に飛んでいく弾
	ORBITING,	//プレイヤーの周囲を回転する弾

	NUM
};
#define IBULLET_ID (int)BULLET_ID

enum class SHOT_ID :i8 {
	SINGLE,
	DOUBLE,
	TRIPLE,
	ORBITING,

	NUM
};
#define ISHOT_ID (int)SHOT_ID
ENUM_OPERATORS(SHOT_ID, i8);

struct BulletParameter{
	constexpr BulletParameter(const char* path, float size, float radius, float speed, float existenceTime, Color color)
		:Path(path), Size(size), Radius(radius), Speed(speed), ExistenceTime(existenceTime), Color(color) {}

	CONSTANT_STRING(32) Path;
	float Size;
	float Radius;
	float Speed;
	float ExistenceTime;//生存時間（秒）
	Color Color;
};


CONSTANT_ARRAY(BulletParameter, IBULLET_ID::NUM) BULLET_PARAMETER = {
	BulletParameter("Data\\Model\\Bullet.x", 0.3f, 0.5f, 1.f, 1.f, color::Orange) ,
	BulletParameter("Data\\Model\\Bullet.x", 0.75f, 0.5f * 2.5f, 1.f, 10.f, color::DarkRed) ,
};