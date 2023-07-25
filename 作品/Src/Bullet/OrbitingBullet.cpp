#include "OrbitingBullet.h"
#include "GameSystem/FrameRate/FrameRate.h"
#include "Src/Player/PlayerManager.h"

OrbitingBullet::OrbitingBullet()
{
	ID_ = BULLET_ID::ORBITING;
	Rot_ = 0.f;
}

OrbitingBullet::~OrbitingBullet()
{
}

Pointer<BulletBase> OrbitingBullet::Clone() const
{
	Pointer<OrbitingBullet> ptr = MyNew(OrbitingBullet, AllocType::Bullet, Tags(MemTag::Bullet), *this);
	ptr->Init(ptr);
	return ptr;
}

void OrbitingBullet::Init(const Pointer<BulletBase>& this_ptr)
{
	__super::Init(this_ptr);
}

void OrbitingBullet::Step()
{
	__super::Step();
	Rot_ += RotSpeed * FrameRate::GetDelteTimef();
	RenderObject_.Pos_ = GET_PLAYER_POS + Vector(0.f, 0.f, OffSet).Rotation(Vector(0.f, Rot_, 0.f));
}

void OrbitingBullet::Fin()
{
	__super::Fin();
}
