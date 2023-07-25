#include "ModelCollision.h"
#include "BoxCollision.h"
#include "SphereCollision.h"
#include "VectorCollision.h"

ModelCollision::ModelCollision()
{
	Shape_ = static_cast<StateANDShapeType>(CollisionShape::MODEL);
	Handle_ = -1;
}

ModelCollision::ModelCollision(int handle)
{
	Handle_ = MV1DuplicateModel(handle);
	MV1SetupReferenceMesh(Handle_, -1, TRUE);
}

ModelCollision::ModelCollision(const ModelCollision& model_col)
{
	Handle_ = MV1DuplicateModel(model_col.Handle_);
	MV1SetupReferenceMesh(Handle_, -1, TRUE);
	CurrentCollison_	= model_col.CurrentCollison_;
	OidCollision_		= model_col.OidCollision_;
	CurrentHitPolygon_	= model_col.CurrentHitPolygon_;
	OidHitPolygon_		= model_col.OidHitPolygon_;
	Owner_				= model_col.Owner_;
	Pos_				= model_col.Pos_;
	PosCalc_			= model_col.PosCalc_;
	Shape_				= model_col.Shape_;
	State_				= model_col.State_;
}

ModelCollision::~ModelCollision()
{
	Fin();
}

void ModelCollision::Init()
{
}

void ModelCollision::Step()
{
	Collision::Step();
	OidHitPolygon_ = CurrentHitPolygon_;
	CurrentHitPolygon_.clear();
}

void ModelCollision::UpDate()
{
	// ポリゴン情報を更新する
	Collision::UpDate();
	MV1SetPosition(Handle_, Pos_);
	MV1RefreshReferenceMesh(Handle_, -1, TRUE);
	Polygons_ = MV1GetReferenceMesh(Handle_, -1, TRUE);
}

void ModelCollision::Fin()
{
	DeleteCollisionModel();
}

Pointer<Collision> ModelCollision::Clone()
{
	Pointer<ModelCollision> clone = MyNew(ModelCollision,AllocType::Collision,Tags(MemTag::Collision));
	*clone = *this;
	if (Handle_ != -1) {
		clone->Handle_ = MV1DuplicateModel(Handle_);
		MV1SetupReferenceMesh(clone->Handle_, -1, TRUE);
	}
	return clone;
}

bool ModelCollision::CheckCollision(Pointer<Collision> other)
{
	switch (static_cast<CollisionShape>(other->GetShape()))
	{
	case CollisionShape::SPHERE:	return isHitSphereModel(static_cast<Pointer<SphereCollision>>(other).get(), this);
	case CollisionShape::BOX:		return isHitBoxModel(static_cast<Pointer<BoxCollision>>(other).get(), this);
	case CollisionShape::VECTOR:	return isHitVectorModel(static_cast<Pointer<VectorCollision>>(other).get(), this);
	case CollisionShape::MODEL:		return isHitModel(this, static_cast<Pointer<ModelCollision>>(other).get());
	}
	return false;
}

void ModelCollision::LoadCollisionModel(const char* model_path)
{
	Handle_ = MV1LoadModel(model_path);
	// 当たり判定用モデルのポリゴン情報をセットアップ
	MV1SetupReferenceMesh(Handle_, -1, TRUE);
}

void ModelCollision::DeleteCollisionModel()
{
	if (Handle_ != -1) {
		MV1DeleteModel(Handle_);
		Handle_ = -1;
	}
}
