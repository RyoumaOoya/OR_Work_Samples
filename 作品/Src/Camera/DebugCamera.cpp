#include "DebugCamera.h"
#include "Input/InputManager.h"

namespace {
	constexpr float CAMERA_NEAR_CLIP = 0.1f;				// �j�A�N���b�v
	constexpr float CAMERA_FAR_CLIP = 500.0f;				// �t�@�[�N���b�v
	constexpr Vector CAMERA_POS(0.0f, 5.0f, -10.0f);	// �������W
	constexpr Vector CAMERA_LOOK(0.0f, 5.0f, 0.0f);		// ���������_
	const float CAMERA_DISTANCE = CAMERA_POS.Long(CAMERA_LOOK);
	constexpr float LOOK_SPEED = 0.04f;
	constexpr float MAX_SPEED = 10.f;
}

DebugCamera::DebugCamera()
{
	Pos_ = CAMERA_POS;
	Look_ = CAMERA_LOOK;
	ID_ = CameraID::DEBUG;
	Speed_ = 0.f;
}

DebugCamera::DebugCamera(Vector pos, Vector look, Vector upVec) :CameraBase(pos, look, upVec)
{
	Speed_ = 2.f;
	ID_ = CameraID::DEBUG;
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::Init()
{
	SetCameraNearFarD(CAMERA_NEAR_CLIP, CAMERA_FAR_CLIP);
}

void DebugCamera::Step()
{
	auto mouse = GET_MOUSE;
	auto keyboard = GET_KEY;
	// �E�N���b�N�ɂ�钍���_��]
	if (mouse->Input(MOUSE_INPUT_RIGHT, INPUT_TYPE::Down)) {
		InputMouseRight(mouse);
	}
	// �z�C�[���h���b�O�h���b�v�ɂ��X���C�h�ړ�
	else if (mouse->Input(MOUSE_INPUT_MIDDLE, INPUT_TYPE::Down)) {
		InputMouseWheel(mouse);
	}

	Vector2D move((keyboard->Input(KEY_INPUT_A, INPUT_TYPE::Down))	?	1.f :
					(keyboard->Input(KEY_INPUT_D, INPUT_TYPE::Down))	?	-1.f :
																		0.f,
					(keyboard->Input(KEY_INPUT_W, INPUT_TYPE::Down))	?	1.f :
					(keyboard->Input(KEY_INPUT_S, INPUT_TYPE::Down))	?	-1.f :
																		0.f);
	if (move != Vector2D::ZEROVEC())
	{
		InputKeyMove(move);
	}

	SpinMouseWheel(mouse);

	// ���������ɕۂ�
	Vector dist_vec = Pos_.Create(Look_);
	dist_vec = dist_vec.Normalize();
	dist_vec *= -CAMERA_DISTANCE;
	Pos_ = Look_ + dist_vec;
}

void DebugCamera::Draw()
{
}

void DebugCamera::Fin()
{
}

// �E�N���b�N����
void DebugCamera::InputMouseRight(const Pointer<CMouseInput>& mouse)
{
	// �h���b�O���h���b�v��������ړ����x���v�Z
	Vector2D mouse_move = static_cast<Vector2D>(mouse->GetOffSetMousePos());
	mouse_move *= -LOOK_SPEED;

	auto forward_vec = GetCameraLookVec().Normalize();

	// �ړ��ɕK�v�ȃx�N�g��
	Vector side_vec = forward_vec ^ Vector(0.0f, -1.0f, 0.0f);	// �J�����̐^���x�N�g��
	Vector up_vec = forward_vec ^ side_vec;					// �J�����̐^��x�N�g��

	// �J�����̏c��]
	up_vec *= mouse_move.y;
	Look_ += up_vec;

	// �J�����̉���]
	side_vec *= -mouse_move.x;
	Look_ += side_vec;
}

// �z�C�[���N���b�N����
void DebugCamera::InputMouseWheel(const Pointer<CMouseInput>& mouse)
{
	// �h���b�O���h���b�v��������ړ����x���v�Z
	Vector2D mouse_move = static_cast<Vector2D>(mouse->GetOffSetMousePos());
	mouse_move *= -LOOK_SPEED;

	auto forward_vec = GetCameraLookVec().Normalize();

	// �c�ړ�
	Pos_.y += mouse_move.y;
	Look_.y += mouse_move.y;

	// ���ړ�
	Vector side_vec = forward_vec ^ Vector(0.0f, 1.0f, 0.0f);
	side_vec *= mouse_move.x;
	Pos_ += side_vec;
	Look_ += side_vec;
}

// �z�C�[����]����
void DebugCamera::SpinMouseWheel(const Pointer<CMouseInput>& mouse)
{
	// �z�C�[����]�ɂ��O�i���
	float wheel_vol = static_cast<float>(mouse->GetMouseWheelRotVolume());
	Speed_ += wheel_vol * 0.1f;
	if (Speed_ > MAX_SPEED)
		Speed_ = MAX_SPEED;
	else if (Speed_ < 0.f)
		Speed_ = 0.f;
}

void DebugCamera::InputKeyMove(const Vector2D& Inputdata)
{
	auto forward_vec = GetCameraLookVec().Normalize();
	auto move = forward_vec * Inputdata.y;
	move += (forward_vec ^ Vector(0.0f, 1.0f, 0.0f)) * Inputdata.x;

	move = move.Normalize();
	move *= Speed_;

	Pos_ += move;
	Look_ += move;
}
