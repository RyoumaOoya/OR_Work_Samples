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
	CONSTANT_STRING(32) Path;	//�t�@�C���p�X
	i32 MaxHP;					//�ő�̗�
	float InvincibleTime;		//���G����
	float Radius;				//���a
	float MoveSpeed;			//�ړ����x
	u8 PopRate;					//�A�C�e���̃h���b�v��
};

CONSTANT_ARRAY(EnemyParameter, IEnemyID::Num) ENEMY_PARAMETER = {
	//				�t�@�C���p�X	�ő�̗� ���G����	���a  �ړ����x	�h���b�v��
	EnemyParameter("Data/Model/Enemy.x", 1,	 0.1f,		1.1f, 0.1f,	0.3f),
};
