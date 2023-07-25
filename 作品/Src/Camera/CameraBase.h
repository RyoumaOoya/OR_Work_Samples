#pragma once
// カメラ基底クラス
#include "DxLib.h"
#include "Lib/Math/MyMathData.h"
#include "CameraParameter.h"

class CameraBase {
public:
	CameraBase();
	CameraBase(Vector pos, Vector look, Vector upVec);
	~CameraBase();
public:
	virtual void Init() = 0;
	virtual void Step() = 0;
	void UpDate();
	virtual void Draw() = 0;
	virtual void Fin() = 0;

	Vector GetPos() const { return Pos_; }
	Vector GetLook() const { return Look_; }

	//カメラの向きベクトルの取得
	Vector GetCameraLookVec() const { return Pos_.Create(Look_); }

	void SetPos(const Vector& pos) { Pos_ = pos; }
	void SetLook(const Vector& look) { Look_ = look; }

	auto GetID() const { return ID_; }
protected:
	Vector Pos_;
	Vector Look_;
	Vector UpVec_;
	CameraID ID_;
	// vfptr(8byte)
};

/*
	CameraBase メモリイメージ

	pos_	24byte
	look_	24byte
	up_vec_	24byte
	vfptr	8byte
*/

namespace Size {
	constexpr size_t CAMARA_BASE_BASE = sizeof(CameraBase);
}