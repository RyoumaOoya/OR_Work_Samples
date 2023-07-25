#pragma once
#include "Character/Character.h"
#include "GameSystem/Allocator/Allocator.h"
#include "Sound/SoundEffect.h"
#include "Src/Collision/CollisionManager.h"
#include "Src/Bullet/BulletParameter.h"

class Player :public Character
{
public:
	Player();
	~Player() override;
	void OnCollisionBegin(Pointer<Object> other) override;
	void OnCollisionEnter(Pointer<Object> other) override;
	void OnCollisionEnd(Pointer<Object> other) override;

public:
	void Init() override;
	void Init(const Pointer<Player>& this_ptr);
	void Load() override;
	void Step() override;
	void ConflictStep() override;
	void FrontDraw() override;
	void Delete() override;
	void Fin() override;

public:
	bool Damage(i32 damage) { if (!isInvincible_) { /*ここだけアトミック操作を利用する*/ InterlockedAdd(&HP_, -damage); isInvincible_ = isDamage_ = true; return true; } return false; }
	Vector GetMove() const { return Move_; }
	void SetMove(const Vector& move) { Move_ = move; }
	Pointer<SphereCollision> GetCollision() const { return Collision_; }
private:
	void Move();

	//パッド入力に対する移動処理
	Vector PadMove(const Vector& foradVec, double delteTime);
	//キーボード入力に対する移動処理
	Vector KeyMove(const Vector& foradVec, double delteTime);
	void Death();	//死亡時の処理
private:
	Vector(*MoveInput)(Pointer<Player> ply, double delteTime, float y);
	Pointer<SphereCollision> Collision_;
	Pointer<SoundEffect> DamageSE_;
	Vector Move_;
	float MovePower_;
	std::list<float> EffectTime_;
	float TimeReceivedDamageLast_;
	volatile long HP_;
	float ShotInterval_;
	bool isDamage_;
	bool isInvincible_;
	SHOT_ID Shot;
	u8 : 8;
};

/*
	Player メモリイメージ

	Object{
		Pos_	24byte
		Tag_	8byte
		Handle_ 4byte	Active_ 1byte	buffer 3byte
		vfptr	8byte
	}	:48byte

	Character{
		Anim_{
			Owner_		 16byte
			AnimNum_	 4byte	AnimIndex_	4byte
			PlayType_	 4byte	AttachAnim_ 4byte
			TotalTime_	 8byte
			CurrentTime_ 8byte
			AnimSpd_	 8byte
		}	:56byte
	}

	collision_	16byte
	move_		24byte
	rot_		24byte

	move_power_ 4byte	state_	4byte
*/

namespace Size {
	constexpr size_t PLAYER = sizeof(Player);
}