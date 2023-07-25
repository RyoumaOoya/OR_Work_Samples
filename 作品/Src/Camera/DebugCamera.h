#pragma once
#include "CameraBase.h"
#include "GameSystem/Allocator/Pointer.h"
#include "Input/Mouse/Mouse.h"

class DebugCamera : public CameraBase
{
public:
	DebugCamera();
	DebugCamera(Vector pos, Vector look, Vector upVec);
	~DebugCamera();

public:
	void Init() override;
	void Step() override;
	void Draw() override;
	void Fin() override;

private:
	//�E�N���b�N���̏���
	void InputMouseRight(const Pointer<CMouseInput>& mouse);
	//�z�C�[���N���b�N���̏���
	void InputMouseWheel(const Pointer<CMouseInput>& mouse);
	//�z�C�[����]���̏���
	void SpinMouseWheel(const Pointer<CMouseInput>& mouse);
	//�L�[�{�[�h���͂̏���
	void InputKeyMove(const Vector2D& Inputdata);
private:
	dVector2D Rotation_;

	float Speed_;
};

