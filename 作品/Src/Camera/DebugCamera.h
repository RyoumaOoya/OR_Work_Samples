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
	//右クリック時の処理
	void InputMouseRight(const Pointer<CMouseInput>& mouse);
	//ホイールクリック時の処理
	void InputMouseWheel(const Pointer<CMouseInput>& mouse);
	//ホイール回転時の処理
	void SpinMouseWheel(const Pointer<CMouseInput>& mouse);
	//キーボード入力の処理
	void InputKeyMove(const Vector2D& Inputdata);
private:
	dVector2D Rotation_;

	float Speed_;
};

