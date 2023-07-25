#pragma once
// �v���C�V�[���Ŏg�p����J����
#include "CameraBase.h"

class PlayCamera : public CameraBase {
public:
	PlayCamera();
	~PlayCamera();

public:
	void Init() override;
	void Step() override;
	void Draw() override;
	void Fin() override;

private:
	Vector2D Rotation_;
};

/*
	PlayCamera �������C���[�W

	CameraBase{
		pos_	24byte
		look_	24byte
		up_vec_	24byte
		vfptr	8byte
	}

	Rotation_ 16byte
*/

namespace Size {
	constexpr size_t PLAY_CAMERA = sizeof(PlayCamera);
}