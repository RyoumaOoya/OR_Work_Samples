#pragma once
// コリジョン基底クラス
#include "CollisionParameter.h"
#include "Object/Object.h"
#include "Math/MyMath.h"
#include "GameSystem/Allocator/Allocator.h"
#include "GameSystem/Thread/CriticalSection.h"
#include <list>

class BoxCollision;
class SphereCollision;
class VectorCollision;
class ModelCollision;

class Collision {
protected:
#ifndef _WIN64
	typedef u16 StateANDShapeType;
#else
	typedef u32 StateANDShapeType;
#endif // _X86_
public:
	enum class HitType {
		NONE,

		BEGIN,
		ENTER,
		END,

		NUM
	};
public:
	Collision();
	virtual ~Collision();

public:
	virtual void Init();
	virtual void Step();
	virtual void UpDate();
	virtual void Draw();
	virtual void Fin();
	virtual Pointer<Collision> Clone() = 0;

	virtual bool CheckCollision(Pointer<Collision> other) = 0;
	//念のため仮想関数にしておく
	virtual void EnterCollision(Pointer<Collision> other);
public:
	//コリジョンの持ち主を設定
	void SetOwner(const Pointer<Object>& owner) { Owner_ = owner; }
	void SetState(StateANDShapeType state) { State_ = state; }

	//コリジョンの持ち主を取得
	Pointer<Object> GetOwner() const { return Owner_; }
	StateANDShapeType GetState() const { return State_; }
	StateANDShapeType GetShape() const { return Shape_; }
	Vector GetPos() const { return Pos_; }
	bool isActive() const { return State_ != static_cast<StateANDShapeType>(CollisionState::NONE); }
	bool isCheck() const { return State_ == static_cast<StateANDShapeType>(CollisionState::ACTIVE); }

	//座標設定処理の登録
	void SetPosCalc(Vector(*Calc)(Object*)) { PosCalc_ = Calc; }
	//otherとどのように当たったかを取得
	HitType CheckHitType(const Pointer<Collision>& other);

	CriticalSection* GetCriticalSection() { return &ListCS_; }
protected:
	// XY平面上での三角形の当たり判定
	static bool IsHitTriangleXY(const Vector& point, const POLYGON& polygon);
	// XZ平面上での三角形の当たり判定
	static bool IsHitTriangleXZ(const Vector& point, const POLYGON& polygon);
	// YZ平面上での三角形の当たり判定
	static bool IsHitTriangleYZ(const Vector& point, const POLYGON& polygon);

	// モデルの当たり判定は作成中

	//箱どうしの当たり判定
	static bool isHitBox(const BoxCollision* boxA, const BoxCollision* boxB);
	//箱と点
	static bool isHitBoxDot(const BoxCollision* box, const Vector& dot);
	//箱とベクトル
	static bool isHitBoxVector(const BoxCollision* box, const VectorCollision* vector);
	//箱とモデル
	static bool isHitBoxModel(const BoxCollision* box, ModelCollision* model);
	//箱と球体
	static bool isHitBoxSphere(const BoxCollision* box, const SphereCollision* sphere);
	//球体どうしの当たり判定
	static bool isHitSphere(const SphereCollision* sphereA, const SphereCollision* sphereB);
	//球体と点
	static bool isHitSphereDot(const SphereCollision* sphere, const Vector& dot);
	//球体とベクトル
	static bool isHitSphereVector(const SphereCollision* sphere, const VectorCollision* vector);
	//球体とモデル
	static bool isHitSphereModel(const SphereCollision* sphere, ModelCollision* model);
	//ベクトルどうしの当たり判定
	static bool isHitVector(const VectorCollision* vectorA, const VectorCollision* vectorB);
	//ベクトルとモデル
	static bool isHitVectorModel(const VectorCollision* vector, ModelCollision* model);
	//モデルどうしの当たり判定
	static bool isHitModel(ModelCollision* modelA, ModelCollision* modelB);
protected:
	std::list<Pointer<Collision>> OidCollision_;
	std::list<Pointer<Collision>> CurrentCollison_;

	CriticalSection ListCS_;

	Pointer<Object> Owner_;
	Vector Pos_;

	Vector(*PosCalc_)(Object*);

	StateANDShapeType Shape_;
	StateANDShapeType State_;

	// vfptr(8byte)
};

/*
	Collision メモリイメージ

	oidCollision_		24byte
	currentCollison_	24byte

	owner_		16byte
	pos_		24byte

	if not _WIN64
		posCalc_	8byte
		shape_	4byte	state_	4byte

	else 
		posCalc_	4byte	shape_	2byte	state_	2byte

	vfptr		8byte
*/

namespace Size {
	constexpr size_t COLLISON_BASE = sizeof(Collision);
}