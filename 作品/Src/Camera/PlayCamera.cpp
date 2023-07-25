#include "PlayCamera.h"
#include "Math/MyMathData.h"
#include "Input/InputManager.h"
#include "Literal.h"
#include "Src/Player/PlayerManager.h"

namespace {
	constexpr double CAMERA_NEAR_CLIP	= 0.3;		// ニアクリップ
	constexpr double CAMERA_FAR_CLIP	= 550.0;	// ファークリップ
	constexpr double CAMERA_DISTANCE	= 20;		// プレイヤーとの距離
	constexpr Vector2D ROTATIONAL_SPEED = { 0.5f, 1.0f };		//
	constexpr double CAMERA_POS_X		= 0.0;		// 固定座標X
	constexpr double CAMERA_POS_Y		= 35.0;		// 固定座標Y
	constexpr double CAMERA_LOOK_X		= 0.0;		// 固定注視点X
	constexpr double CAMERA_LOOK_Y		= 14.0;		// 固定注視点Y
	constexpr Vector POS_OFFSET			= Vector(0.f, 50.f, -0.1f);
	constexpr Vector LOCK_OFFSET		= Vector(0.f, 15.f, 0.f);
	constexpr float  ROTATION_LIMIT_X	= 0.49999_PI_F;
}

PlayCamera::PlayCamera() : CameraBase()
{
	//見下ろしにする
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