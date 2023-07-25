#pragma once
// �R���W�������N���X
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
	//�O�̂��߉��z�֐��ɂ��Ă���
	virtual void EnterCollision(Pointer<Collision> other);
public:
	//�R���W�����̎������ݒ�
	void SetOwner(const Pointer<Object>& owner) { Owner_ = owner; }
	void SetState(StateANDShapeType state) { State_ = state; }

	//�R���W�����̎�������擾
	Pointer<Object> GetOwner() const { return Owner_; }
	StateANDShapeType GetState() const { return State_; }
	StateANDShapeType GetShape() const { return Shape_; }
	Vector GetPos() const { return Pos_; }
	bool isActive() const { return State_ != static_cast<StateANDShapeType>(CollisionState::NONE); }
	bool isCheck() const { return State_ == static_cast<StateANDShapeType>(CollisionState::ACTIVE); }

	//���W�ݒ菈���̓o�^
	void SetPosCalc(Vector(*Calc)(Object*)) { PosCalc_ = Calc; }
	//other�Ƃǂ̂悤�ɓ������������擾
	HitType CheckHitType(const Pointer<Collision>& other);

	CriticalSection* GetCriticalSection() { return &ListCS_; }
protected:
	// XY���ʏ�ł̎O�p�`�̓����蔻��
	static bool IsHitTriangleXY(const Vector& point, const POLYGON& polygon);
	// XZ���ʏ�ł̎O�p�`�̓����蔻��
	static bool IsHitTriangleXZ(const Vector& point, const POLYGON& polygon);
	// YZ���ʏ�ł̎O�p�`�̓����蔻��
	static bool IsHitTriangleYZ(const Vector& point, const POLYGON& polygon);

	// ���f���̓����蔻��͍쐬��

	//���ǂ����̓����蔻��
	static bool isHitBox(const BoxCollision* boxA, const BoxCollision* boxB);
	//���Ɠ_
	static bool isHitBoxDot(const BoxCollision* box, const Vector& dot);
	//���ƃx�N�g��
	static bool isHitBoxVector(const BoxCollision* box, const VectorCollision* vector);
	//���ƃ��f��
	static bool isHitBoxModel(const BoxCollision* box, ModelCollision* model);
	//���Ƌ���
	static bool isHitBoxSphere(const BoxCollision* box, const SphereCollision* sphere);
	//���̂ǂ����̓����蔻��
	static bool isHitSphere(const SphereCollision* sphereA, const SphereCollision* sphereB);
	//���̂Ɠ_
	static bool isHitSphereDot(const SphereCollision* sphere, const Vector& dot);
	//���̂ƃx�N�g��
	static bool isHitSphereVector(const SphereCollision* sphere, const VectorCollision* vector);
	//���̂ƃ��f��
	static bool isHitSphereModel(const SphereCollision* sphere, ModelCollision* model);
	//�x�N�g���ǂ����̓����蔻��
	static bool isHitVector(const VectorCollision* vectorA, const VectorCollision* vectorB);
	//�x�N�g���ƃ��f��
	static bool isHitVectorModel(const VectorCollision* vector, ModelCollision* model);
	//���f���ǂ����̓����蔻��
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
	Collision �������C���[�W

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