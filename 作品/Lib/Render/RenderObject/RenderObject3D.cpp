#include "RenderObject3D.h"

RenderObject3DBase::RenderObject3DBase() :Tags(0)
{
	Handle_ = -1;
	SetTags({ RenderTags::isDraw, RenderTags::ShadowWrite });
}

RenderObject3DBase::~RenderObject3DBase()
{
}

RenderObject3D::RenderObject3D() :Scale_(1.f, 1.f, 1.f)
{
}

RenderObject3D::RenderObject3D(const RenderObject3D& other)
{
	memcpy(this, &other, sizeof(RenderObject3D));
	Load(other.Handle_);
}

RenderObject3D::~RenderObject3D()
{
}

void RenderObject3D::Load(const char* path)
{
	if (Handle_ == -1) {
		Handle_ = MV1LoadModel(path);
	}
}

void RenderObject3D::Load(handle handle)
{
	if (handle == Handle_ || Handle_ == -1) {
		Handle_ = MV1DuplicateModel(handle);
	}
}

void RenderObject3D::UpDate()
{
	this->SetWritingFlag(true);
	MV1SetPosition(Handle_, Pos_);
	MV1SetRotationXYZ(Handle_, Rot_);
	MV1SetScale(Handle_, Scale_);
	this->SetWritingFlag(false);
}

void RenderObject3D::Draw()
{
	MV1DrawModel(Handle_);
}

void RenderObject3D::Delete()
{
	if (Handle_ != -1) {
		MV1DeleteModel(Handle_);
		Handle_ = -1;
	}
}

RenderObjectBillboard::RenderObjectBillboard()
{
	ExRate_ = 1.f;
	Angle_ = 0.f;
	ReverseX_ = false;
	ReverseY_ = false;
}

RenderObjectBillboard::~RenderObjectBillboard()
{
	Delete();
}

void RenderObjectBillboard::Load(const char* path)
{
	if (Handle_) {
		Handle_ = LoadGraph(path);
	}
}

void RenderObjectBillboard::Draw()
{
	DrawRotaGraph3D(Pos_.x, Pos_.y, Pos_.z, ExRate_, Angle_, Handle_, true, ReverseX_, ReverseY_);
}

void RenderObjectBillboard::Delete()
{
	if (Handle_ != -1) {
		DeleteGraph(Handle_);
	}
}
