#include "Enemy1.h"
#include "GameSystem/Allocator/Allocator.h"
#include "Src/Player/PlayerManager.h"
#include "EnemyParameter.h"
#include "GameSystem/Common/Common.h"
#include "GameSystem/FrameRate/FrameRate.h"

constexpr auto Param = ENEMY_PARAMETER[IEnemyID::Enemy1];

Enemy1::Enemy1()
{
	ID_ = IEnemyID::Enemy1;
}

Enemy1::~Enemy1()
{
	Fin();
}

void Enemy1::OnCollisionBegin(Pointer<Object> other)
{
	if (other->ObjectHasTag(OBJECT_TAGS::BULLET) /*|| other->ObjectHasTag(OBJECT_TAGS::PLAYER)*/) {
		Death();
	}
}

void Enemy1::OnCollisionEnter(Pointer<Object> other)
{
}

void Enemy1::OnCollisionEnd(Pointer<Object> other)
{
}

Pointer<EnemyBase> Enemy1::Clone() const
{
	Pointer<EnemyBase> ptr = MyNew(Enemy1, AllocType::Enemy, Tags(), *this);
	ptr->Init(ptr);
	//(ptr->*static_cast<void(Object::*)(int)>(&Object::Load))(RenderObject_.Handle_);
	return ptr;
}

void Enemy1::Init()
{
	Init(Allocator::GetPointerStorage(this, AllocType::Enemy));
}

void Enemy1::Init(const Pointer<EnemyBase>& this_ptr)
{
	__super::Init(this_ptr);
}

void Enemy1::Step()
{
	auto vec = RenderObject_.Pos_.Create(GET_PLAYER_POS).Normalize() * Param.MoveSpeed;
	SetRot(static_cast<Vector>(vec.GetRotationXY()));
	Move_.x = vec.x;
	Move_.z = vec.z;
	Move_ += Common::Gravity * FrameRate::GetDelteTime();
	RenderObject_.Pos_ += Move_;
}

