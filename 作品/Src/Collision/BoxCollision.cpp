#include "BoxCollision.h"
#include "VectorCollision.h"
#include "SphereCollision.h"
#include "ModelCollision.h"
#include "CollisionParameter.h"
#include "Constant/Constant.h"

BoxCollision::BoxCollision() : Collision()
{
	Shape_ = static_cast<u16>(CollisionShape::BOX);
	Size_ = { 0.0f };
	SizeCalc_ = nullptr;
}

BoxCollision::BoxCollision(const Vector& pos, const Vector& size) : Collision()
{
	Shape_ = static_cast<u16>(CollisionShape::BOX);

	Pos_ = pos;
	Size_ = size;
	SizeCalc_ = nullptr;
}

BoxCollision::~BoxCollision()
{
	Fin();
}

void BoxCollision::Init()
{
}

void BoxCollision::Step()
{
	Collision::Step();
	if (Owner_) {
		if (SizeCalc_)
			Size_ = (*SizeCalc_)(Owner_.get());
	}
}

void BoxCollision::UpDate()
{
	Collision::UpDate();
	if (Owner_) {
		if (SizeCalc_)
			Size_ = (*SizeCalc_)(Owner_.get());
	}
}

void BoxCollision::Draw()
{
#ifdef	_DEBUG
	// “–‚½‚è”»’è‚Ì‰ÂŽ‹‰»
	Vector posA = Pos_ - Size_ * 0.5;
	Vector posB = Pos_ - Size_ * 0.5;

	DrawCube3D(posA, posB, Constant::Color(0, 255, 0), Constant::Color(0, 255, 0), FALSE);
#endif
}

void BoxCollision::Fin()
{
}

Pointer<Collision> BoxCollision::Clone()
{
	return MyNew(BoxCollision, AllocType::Collision, Tags(MemTag::Collision), *this);
}

bool BoxCollision::CheckCollision(Pointer<Collision> other)
{
	switch (static_cast<CollisionShape>(other->GetShape()))
	{
	case CollisionShape::SPHERE:	return isHitBoxSphere	(this, static_cast<SphereCollision*>(other.get()));
	case CollisionShape::BOX:		return isHitBox			(this, static_cast<BoxCollision*>	(other.get()));
	case CollisionShape::VECTOR:	return isHitBoxVector	(this, static_cast<VectorCollision*>(other.get()));
	case CollisionShape::MODEL:		return isHitBoxModel	(this, static_cast<ModelCollision*>	(other.get()));
	}
	return false;
}