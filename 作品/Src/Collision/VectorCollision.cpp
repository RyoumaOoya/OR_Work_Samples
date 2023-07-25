#include "VectorCollision.h"
#include "BoxCollision.h"
#include "SphereCollision.h"
#include "ModelCollision.h"
#include "Constant/Constant.h"

VectorCollision::VectorCollision() :Collision()
{
	Shape_ = static_cast<StateANDShapeType>(CollisionShape::VECTOR);
	Vec_ = { 0.f };
	Long_ = FLT_MAX * 0.25;
	Radius_ = 0.f;

	VecCalc_ = nullptr;
	LongCalc_ = nullptr;
	RadiusCalc_ = nullptr;
}

VectorCollision::VectorCollision(const Vector& pos, const dVector& vec, const float Long, const float radius)
{
	Shape_ = static_cast<StateANDShapeType>(CollisionShape::VECTOR);
	Pos_ = pos;
	Vec_ = vec;
	Long_ = Long;
	Radius_ = radius;

	VecCalc_ = nullptr;
	LongCalc_ = nullptr;
	RadiusCalc_ = nullptr;
}

VectorCollision::~VectorCollision()
{
	Fin();
}

void VectorCollision::Init()
{
}

void VectorCollision::Step()
{
	Collision::Step();
	if (Owner_) {
		if (VecCalc_)
			Vec_ = (*VecCalc_)(Owner_.get());
		if (LongCalc_)
			Long_ = (*LongCalc_)(Owner_.get());
		if (RadiusCalc_)
			Radius_ = (*RadiusCalc_)(Owner_.get());
	}
}

void VectorCollision::UpDate()
{
	Collision::UpDate();
	if (Owner_) {
		if (VecCalc_)
			Vec_ = (*VecCalc_)(Owner_.get());
		if (LongCalc_)
			Long_ = (*LongCalc_)(Owner_.get());
		if (RadiusCalc_)
			Radius_ = (*RadiusCalc_)(Owner_.get());
	}
}

void VectorCollision::Draw()
{
#ifdef	_DEBUG
	DrawLine3D(Pos_, Pos_ + (Vec_.Normalize() * Long_), Constant::Color(255, 255, 255));
#endif
}

void VectorCollision::Fin()
{
}

Pointer<Collision> VectorCollision::Clone()
{
	return MyNew(VectorCollision, AllocType::Collision, Tags(MemTag::Collision), *this);
}

bool VectorCollision::CheckCollision(Pointer<Collision> other)
{
	if (Pos_.Long(other->GetPos()) > 100.f) return false;

	switch (static_cast<CollisionShape>(other->GetShape())) {
	case CollisionShape::SPHERE:	return isHitSphereVector(static_cast<SphereCollision*>	(other.get()), this);
	case CollisionShape::BOX:		return isHitBoxVector(static_cast<BoxCollision*>		(other.get()), this);
	case CollisionShape::VECTOR:	return isHitVector(this, static_cast<VectorCollision*>	(other.get()));
	case CollisionShape::MODEL:		return isHitVectorModel(this, static_cast<ModelCollision*>	(other.get()));
	}
	return false;
}