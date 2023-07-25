#pragma once
#include "Object/Object.h"
#include "Sound/SoundEffect.h"
#include "typedef.h"

class SphereCollision;
class AIBase;
class Entity;

class EnemyBase:public Object
{
	friend class EnemyAI;
	friend class EnemyManager;
public:
	EnemyBase();
	virtual ~EnemyBase() override;

	virtual Pointer<EnemyBase> Clone() const = 0;
public:
	virtual void Init()override;
	virtual void Init(const Pointer<EnemyBase>& this_ptr);
	virtual void Load()override;
	virtual void Step()override;
	virtual void Delete()override;
	virtual void Fin()override;

	virtual bool Damage(i16 damage) { if (!isInvincible_) { HP_ -= damage; isDamage_ = isInvincible_ = true; return true; } return false; }
	auto GetCollision() { return Collision_; }
	Vector GetMove() { return Move_; }
	void SetMove(Vector move) { Move_ = move; }
	//“–‚½‚è”»’è¶¬A‰Šú‰»ŠÖ”
	void SetCollision();
	void SetSE();
protected:
	void Death();	//€–S‚Ì‹¤’Êˆ—
protected:
	u8 ID_;
	bool isDamage_ : 1;
	bool isInvincible_ : 1;
	u8 : 6;
	i16 HP_;
	float TimeReceivedDamageLast_;
	Vector Move_;
	Pointer<SphereCollision> Collision_;
//	Pointer<MyLib::SoundEffect> DamageSE_;
};

namespace Size {
	constexpr auto ENEMY_BASE = sizeof(EnemyBase) - sizeof(Object);
}
