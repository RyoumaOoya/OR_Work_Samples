#include "EnemyBase.h"
#include "EnemyParameter.h"
#include "Object/ObjectParameter.h"
#include "GameSystem/Allocator/Allocator.h"
#include "GameSystem/Common/Common.h"
#include "GameSystem/FrameRate/FrameRate.h"
#include "Sound/SoundParameter.h"
#include "Sound/SoundManager.h"
#include "Sound/SoundEffect.h"
#include "Time/Time.h"
#include "Src/Collision/CollisionManager.h"
#include "Src/Collision/SphereCollision.h"
#include "Src/Item/ItemManager.h"

EnemyBase::EnemyBase()
{
	SetActive(true);
	ID_ = 0;
	HP_ = 1;
	isDamage_ = false;
	isInvincible_ = false;
	TimeReceivedDamageLast_ = 0.f;
	Object::SetTag(OBJECT_TAGS::ENEMY);
}

EnemyBase::~EnemyBase()
{
	Fin();
}

void EnemyBase::Init()
{
	Init(Allocator::GetPointerStorage(this, AllocType::Enemy));
}

void EnemyBase::Init(const Pointer<EnemyBase>& this_ptr)
{
	__super::Init();
	HP_ = ENEMY_PARAMETER[static_cast<int>(ID_)].MaxHP;
	SetThisPtr(this_ptr);
	PushModelList();
	SetCollision();
	SetSE();
}

void EnemyBase::Load()
{
	__super::Load(ENEMY_PARAMETER[ID_].Path.data());
}

void EnemyBase::Step()
{
	if (HP_ <= 0) {
		Death();
		return;
	}
	if (isDamage_) {
		Move_.y = 0.1f;
		//DamageSE_->Play();
		isDamage_ = false;
	}
	if (isInvincible_ && CTime::GetCountSec(TimeReceivedDamageLast_) >= ENEMY_PARAMETER[ID_].InvincibleTime) {
		//DamageSE_->Play();
		isInvincible_ = false;
	}
	if (RenderObject_.Pos_.y <= Common::Y_LOWER_LIMIT) {
		Death();
		return;
	}
}

void EnemyBase::Delete()
{
	__super::Delete();
	if (Collision_)
		Collision_->SetState(ICollisionState::NONE);
	/*if (DamageSE_)
		DamageSE_->SetState(ISoundState::Fin);*/
}

void EnemyBase::Fin()
{
	Delete();
	__super::Fin();
}

void EnemyBase::SetCollision()
{
	Collision_ = static_cast<Pointer<SphereCollision>>(CollisionManager::GetInstance()->CreateCollision(ICollisionShape::SPHERE, ICollisionCategory::ENEMY));
	Collision_->SetOwner(This_);
	Collision_->SetRadius(ENEMY_PARAMETER[ID_].Radius);
	HP_ = ENEMY_PARAMETER[ID_].MaxHP;
}

void EnemyBase::SetSE()
{
	/*DamageSE_ = static_cast<Pointer<MyLib::SoundEffect>>(SoundManager::GetInstance()->CreateSEHandle(ISE_ID::HIT));
	auto this_ = This_;
	DamageSE_->SetPosCalc([this_]() {return (this_) ? this_->GetPos() : Vector(); });*/
}

void EnemyBase::Death()
{
	SetActive(false);
	//SoundManager::GetInstance()->PlaySE(ISE_ID::HIT, RenderObject_.Pos_);
	SetDeleteFlag();
	if (Common::GetRandom<u8>() <= ENEMY_PARAMETER[ID_].PopRate) {
		ItemManager::GetInstance()->CreateItem(static_cast<ITEM_ID>(Common::GetRandom(IITEM_ID::Num - 1)), RenderObject_.Pos_);
	}
}
