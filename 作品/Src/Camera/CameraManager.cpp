#include "CameraManager.h"
#include "CameraParameter.h"
#include "PlayCamera.h"
#include "DebugCamera.h"
#include "Sound/SoundManager.h"

CameraManager::CameraManager()
: Camera_()
{
	Init();
	Index_ = MANAGER_INDEX::CameraStep;
}

CameraManager::~CameraManager()
{
	Fin();
}

void CameraManager::Init()
{
	StartCamera((int)CameraID::PLAY);

	SoundManager::GetInstance()->SetListenerCalc([](Vector* pos, Vector* front) {
		auto camera = CameraManager::GetInstance()->GetCamera();
		*pos = camera->GetPos();
		*front = camera->GetLook();
		});
}

void CameraManager::Step()
{
	Camera_->Step();
}

void CameraManager::UpDate()
{
	Camera_->UpDate();
}

void CameraManager::FrontDraw()
{
	Camera_->Draw();
}

void CameraManager::Fin()
{
	DeleteCamera();
}

// カメラ起動処理（既に起動していたものは停止します）
void CameraManager::StartCamera(int id)
{
	if (Camera_) {
		if (Camera_->GetID() == CameraID::PLAY) {
			tempPos_ = Camera_->GetPos();
			tempLook_ = Camera_->GetLook();
			tempUpVec_ = Vector(0, 1, 0);
		}
		DeleteCamera();
	}

	switch (id) {
	case (int)CameraID::PLAY: Camera_ = MyNew(PlayCamera, AllocType::Camera, 0); break;
	case (int)CameraID::DEBUG: Camera_ = MyNew(DebugCamera, AllocType::Camera, 0, tempPos_, tempLook_, tempUpVec_); break;
	}
	Camera_->Init();
}

void CameraManager::DeleteCamera()
{
	if (Camera_) MyFree(Camera_);
}

void CameraManager::SetNearFur(float Near, float Fur)
{
	SetCameraNearFar(Near, Fur);
}
