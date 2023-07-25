#pragma once
#include "Collision.h"
#include <float.h>

class VectorCollision : public Collision
{
public:
	VectorCollision();
	VectorCollision(const Vector& pos, const dVector& vec, const float Long = FLT_MAX * 0.25, const float radius = 0);
	~VectorCollision() override;

public:
	void Init() override;
	void Step() override;
	void UpDate() override;
	void Draw() override;
	void Fin() override;
	Pointer<Collision> Clone() override;

	bool CheckCollision(Pointer<Collision> other) override;
public:
	Vector GetVector() const { return Vec_; }

	void SetVector(const dVector& vec) { Vec_ = vec; }

	float GetLong() const { return Long_; }
	void SetLong(float Long) { Long_ = Long; }

	float GetRadius() const { return Radius_; }
	void SetRadius(float Radius) { Radius_ = Radius; }

	//方向の更新処理の設定
	void SetVecCalc(Vector(*Calc)(Object*)) { VecCalc_ = Calc; }
	//長さの更新処理の設定
	void SetLongCalc(float (*Calc_)(Object*)) { LongCalc_ = Calc_; }
	//半径の更新処理の設定
	void SetRadiusCalc(float(*Calc)(Object*)) { RadiusCalc_ = Calc; }
private:
	Vector(*VecCalc_)(Object*);
	float (*LongCalc_)(Object*);
	float (*RadiusCalc_)(Object*);

	Vector Vec_;
	float Long_;
	float Radius_;

#ifdef _WIN64
	char buffer[4] = { 0 };
#endif // _WIN64
};

/*
	SphereCollision メモリイメージ

	Collision{
		oidCollision_		12byte
		currentCollison_	12byte

		owner_		16byte
		pos_		24byte
		
		if _X_64_
			posCalc_	8byte
			shape_	4byte	state_	4byte

		else if _X_86_
			posCalc_	4byte	shape_	2byte	state_	2byte

		vfptr		8byte
	}

	dVector		24byte
	long_		8byte
	radius_		8byte
	if _X_64_
		vecCalc_	8byte
		longCalc_	8byte
		radiusCalc_	8byte

	else if _X_86_
		vecCalc_	4byte	longCalc_	4byte
		radiusCalc_	4byte	buffer		4byte
*/

namespace Size {
	constexpr size_t VECTOR_COLLISION = sizeof(VectorCollision);
}