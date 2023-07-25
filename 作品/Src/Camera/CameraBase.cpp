#include "CameraBase.h"
#
CameraBase::CameraBase()
{
	Pos_(0.0f, 0.0f, 0.0f);
	Look_(0.0f, 0.0f, 0.0f);
	UpVec_(0.0f, 1.0f, 0.0f);
	ID_ = CameraID::Null;
}

CameraBase::CameraBase(Vector pos, Vector look, Vector upVec) :Pos_(pos), Look_(look), UpVec_(upVec), ID_(CameraID::Null)
{
}

CameraBase::~CameraBase()
{
}

void CameraBase::UpDate()
{
	SetCameraPositionAndTargetAndUpVec(Pos_, Look_, UpVec_);
}