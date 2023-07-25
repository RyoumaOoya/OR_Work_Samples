#pragma once
// �J�����Ǘ��N���X
#include "CameraBase.h"
#include "ManagerBase.h"
#include "Define.h"

class CameraManager :public SingletonBase<CameraManager> {
	MANAGER;
private:
	CameraManager();
	~CameraManager();
public:
	void Init()override;
	void Load()override {}
	void Step()override;
	void ConflictStep()override {}
	void UpDate()override;
	void FrontDraw()override;
	void Delete()override {}
	void Fin()override;

	Vector GetPos() { return Camera_->GetPos(); }
	Pointer<CameraBase> GetCamera() { return Camera_; }

	//void Test() { Camera_->Step(); Camera_->UpDate(); }

	//�J�����̐؂�ւ�
	void StartCamera(int id);
	void DeleteCamera();
	//�j�A�t�@�[�ݒ�
	void SetNearFur(float Near, float Fur);
private:
	Pointer<CameraBase> Camera_;
	Vector tempPos_;
	Vector tempLook_;
	Vector tempUpVec_;
};

namespace Size {
	constexpr size_t CAMERA_MANAGER = sizeof(CameraManager);
}