#include "CollisionParameter.h"
#include "SphereCollision.h"
#include "VectorCollision.h"
#include "BoxCollision.h"
#include "ModelCollision.h"
#include "Constant/Constant.h"

SphereCollision::SphereCollision() : Collision()
{
	Shape_ = static_cast<StateANDShapeType>(CollisionShape::SPHERE);
	Radius_ = 0.0f;
	RadiusCalc_ = nullptr;
}

SphereCollision::SphereCollision(const Vector& pos, float radius)
{
	Shape_ = static_cast<StateANDShapeType>(CollisionShape::SPHERE);
	Pos_ = pos;
	Radius_ = radius;
	RadiusCalc_ = nullptr;
}

SphereCollision::~SphereCollision()
{
	Fin();
}

void SphereCollision::Init()
{
}

void SphereCollision::Step()
{
	Collision::Step();
	if (Owner_) {
		if (RadiusCalc_)
			Radius_ = (*RadiusCalc_)(Owner_.get());
	}
}

void SphereCollision::UpDate()
{
	Collision::UpDate();
	if (Owner_) {
		if (RadiusCalc_)
			Radius_ = (*RadiusCalc_)(Owner_.get());
	}
}

void SphereCollision::Draw()
{
#ifdef _DEBUG
	// “–‚½‚è”»’è‰ÂŽ‹‰»
	DrawSphere3D(Pos_, Radius_, 16, Constant::Color(0, 255, 0), Constant::Color(0, 255, 0), FALSE);
#endif
}

void SphereCollision::Fin()
{
}

Pointer<Collision> SphereCollision::Clone()
{
	return MyNew(SphereCollision, AllocType::Collision, Tags(MemTag::Collision), *this);
}

bool SphereCollision::CheckCollision(Pointer<Collision> other)
{
	switch ((CollisionShape)other->GetShape())
	{
	case CollisionShape::SPHERE:	return isHitSphere(static_cast<SphereCollision*>	(other.get()), this);
	case CollisionShape::BOX:		return isHitBoxSphere(static_cast<BoxCollision*>	(other.get()), this);
	case CollisionShape::VECTOR:	return isHitSphereVector(this,	static_cast<VectorCollision*>	(other.get()));
	case CollisionShape::MODEL:		return isHitSphereModel(this,	static_cast<ModelCollision*>	(other.get()));
	}
	return false;
}
