#include "Item.h"
#include "ItemParameter.h"
#include "Math/MyMath.h"
#include "Render/Render.h"
#include "GameSystem/FrameRate/FrameRate.h"
#include "GameSystem/Allocator/Allocator.h"
#include "GameSystem/Common/Common.h"
#include "Time/Time.h"
#include "Src/Collision/SphereCollision.h"
#include "Src/Player/PlayerManager.h"

namespace {
	constexpr float IETM_RADIUS(1.5);
	constexpr float ExistenceTime(5.f);
	constexpr float SuckingRange(10.f);
	constexpr float SuckingPower(0.4f);
}

Item::Item(ITEM_ID ID)
{
	ID_ = ID;
	SetTag(OBJECT_TAGS::ITEM);
	RenderObject_.Scale_ = Vector(1.75,1.75,1.75);
	Time_ = 0.f;
}

Item::~Item()
{
	Fin();
}

void Item::OnCollisionBegin(Pointer<Object> other)
{
	if (other->ObjectHasTag(OBJECT_TAGS::PLAYER) && IsActive())
	{
		Death();
	}
}

void Item::OnCollisionEnter(Pointer<Object> other)
{
}

void Item::OnCollisionEnd(Pointer<Object> other)
{
}

Pointer<Item> Item::Clone() const
{
	Pointer<Item> ptr = MyNew(Item, AllocType::Item, Tags(), *this);
	(ptr->*static_cast<void(Object::*)(int)>(&Object::Load))(RenderObject_.Handle_);
	//MV1DuplicateModelでカラー変更が反映されてないみたいなのでここで適用
	ptr->RenderObject_.SetColor(ITEM_DATA[static_cast<int>(ID_)].color);
	return ptr;
}

void Item::Init()
{
	Init(Allocator::GetPointerStorage(this, AllocType::Item));
}

void Item::Init(const Pointer<Item>& this_ptr)
{
	Object::Init();
	SetActive(true);
	auto ptr = (Pointer<Object>)this_ptr;
	SetThisPtr(ptr);
	Collision_ = static_cast<Pointer<SphereCollision>>(CollisionManager::GetInstance()->CreateCollision(ICollisionShape::SPHERE, ICollisionCategory::ITEM));
	Collision_->SetOwner(ptr);
	Collision_->SetRadius(IETM_RADIUS);
	PushModelList();
	Time_ = CTime::StartCountSecf();
}

void Item::Load()
{
	Object::Load(ITEM_DATA[static_cast<int>(ID_)].Path.data());
}

void Item::Step()
{
	Move_ += Common::Gravity * FrameRate::GetDelteTime();
	auto vec = RenderObject_.Pos_.Create(GET_PLAYER_POS);
	if (vec.LongXZSq() <= MyMath::Squared(SuckingRange)) {
		Move_ += vec.Normalize() * SuckingPower;
	}
	if (RenderObject_.Pos_.y <= Common::Y_LOWER_LIMIT || CTime::GetCountSecf(Time_) >= ExistenceTime) {
		Death();
	}
	RenderObject_.Pos_ += Move_;
}

void Item::Delete()
{
	Object::Delete();
	if (Collision_)
		Collision_->SetState(ICollisionState::NONE);
	EreseModelList();
}

void Item::Fin()
{
	Delete();
}

void Item::Death()
{
	SetActive(false);
	this->SetRenderTag(RenderTags::isDeleteTarget);
}
