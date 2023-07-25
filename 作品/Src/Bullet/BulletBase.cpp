#include "BulletBase.h"
#include "GameSystem/Allocator/Allocator.h"
#include "Src/Collision/CollisionManager.h"
#include "Time/Time.h"

BulletBase::BulletBase()
{
	ID_ = BULLET_ID::Null;
	Time_ = 0.f;
	SetTag(OBJECT_TAGS::BULLET);
}

BulletBase::~BulletBase()
{
	Fin();
}

void BulletBase::OnCollisionBegin(Pointer<Object> other)
{
	if (other->ObjectHasTag(OBJECT_TAGS::ENEMY)) {
		Death();
	}
}

void BulletBase::OnCollisionEnter(Pointer<Object> other)
{
}

void BulletBase::OnCollisionEnd(Pointer<Object> other)
{
}

void BulletBase::Init()
{
	Init(Allocator::GetPointerStorage(this, AllocType::Bullet));
}

void BulletBase::Init(const Pointer<BulletBase>& this_ptr)
{
	SetThisPtr(this_ptr);
	SetScale(Vector(BULLET_PARAMETER[static_cast<int>(ID_)].Size, BULLET_PARAMETER[static_cast<int>(ID_)].Size, BULLET_PARAMETER[static_cast<int>(ID_)].Size));
	PushModelList();
	CreateCollision();
	Time_ = CTime::StartCountSecf();
}

void BulletBase::Load()
{
	__super::Load(BULLET_PARAMETER[static_cast<int>(ID_)].Path.data());
}

void BulletBase::Step()
{
	if (CTime::GetCountSecf(Time_) >= BULLET_PARAMETER[static_cast<int>(ID_)].ExistenceTime) {
		Death();
	}
}

void BulletBase::Delete()
{
	__super::Delete();
	if (Coll_)
		Coll_->SetState(ICollisionState::NONE);
}

void BulletBase::Fin()
{
	Delete();
	__super::Fin();
}

void BulletBase::CreateCollision()
{
	Coll_ = static_cast<Pointer<SphereCollision>>(CollisionManager::GetInstance()->CreateCollision(ICollisionShape::SPHERE, ICollisionCategory::BULLET));
	Coll_->SetOwner(This_);
	Coll_->SetRadius(BULLET_PARAMETER[static_cast<int>(ID_)].Radius);
}

void BulletBase::Death()
{
	SetActive(false);
	SetDeleteFlag();
}

void BulletBase::ChangeColor()
{
	RenderObject_.SetColor(BULLET_PARAMETER[static_cast<int>(ID_)].Color);
}
