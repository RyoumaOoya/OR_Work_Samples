#include "NormalBullet.h"
#include "GameSystem/Allocator/Allocator.h"

constexpr auto Param = BULLET_PARAMETER[IBULLET_ID::NORMAL];
NormalBullet::NormalBullet()
{
	ID_ = BULLET_ID::NORMAL;
}

NormalBullet::~NormalBullet()
{
	Fin();
}

Pointer<BulletBase> NormalBullet::Clone() const
{
	Pointer<NormalBullet> ptr = MyNew(NormalBullet, AllocType::Bullet, Tags(MemTag::Bullet), *this);
	ptr->Init(ptr);
	return ptr;
}

void NormalBullet::Init(const Pointer<BulletBase>& this_ptr)
{
	__super::Init(this_ptr);
}

void NormalBullet::Step()
{
	__super::Step();
	RenderObject_.Pos_ += Move_;
}

void NormalBullet::Fin()
{
	__super::Fin();
}
