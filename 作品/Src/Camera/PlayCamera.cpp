#include "PlayCamera.h"
#include "Math/MyMathData.h"
#include "Input/InputManager.h"
#include "Literal.h"
#include "Src/Player/PlayerManager.h"

namespace {
	constexpr double CAMERA_NEAR_CLIP	= 0.3;		// �j�A�N���b�v
	constexpr double CAMERA_FAR_CLIP	= 550.0;	// �t�@�[�N���b�v
	constexpr double CAMERA_DISTANCE	= 20;		// �v���C���[�Ƃ̋���
	constexpr Vector2D ROTATIONAL_SPEED = { 0.5f, 1.0f };		//
	constexpr double CAMERA_POS_X		= 0.0;		// �Œ���WX
	constexpr double CAMERA_POS_Y		= 35.0;		// �Œ���WY
	constexpr double CAMERA_LOOK_X		= 0.0;		// �Œ蒍���_X
	constexpr double CAMERA_LOOK_Y		= 14.0;		// �Œ蒍���_Y
	constexpr Vector POS_OFFSET			= Vector(0.f, 50.f, -0.1f);
	constexpr Vector LOCK_OFFSET		= Vector(0.f, 15.f, 0.f);
	constexpr float  ROTATION_LIMIT_X	= 0.49999_PI_F;
}

PlayCamera::PlayCamera() : CameraBase()
{
	//�����낵�ɂ���
	Pos_(POS_OFFSET);
	Look_(LOCK_OFFSET);
	Rotation_(0.f, 0.f);
	ID_ = CameraID::PLAY;
}

PlayCamera::~PlayCamera()
{
	Fin();
}

void PlayCamera::Init()
{
	SetCameraNearFarD(CAMERA_NEAR_CLIP, CAMERA_FAR_CLIP);
}

void PlayCamera::Step()
{
}

void PlayCamera::Draw()
{
}

void PlayCamera::Fin()
{
}