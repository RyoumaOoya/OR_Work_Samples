#pragma once
#include "Collision.h"

class SphereCollision : public Collision {
public:
	SphereCollision();
	SphereCollision(const Vector& pos, float radius);
	~SphereCollision() override;

public:
	void Init() override;
	void Step() override;
	void UpDate() override;
	void Draw() override;
	void Fin() override;
	Pointer<Collision> Clone() override;

	bool CheckCollision(Pointer<Collision> other) override;

public:
	void SetRadius(float radius) { Radius_ = radius; }
	float GetRadius() const { return Radius_; }
	//半径の更新処理の設定
	void SetRadiusCalc(float(*Calc)(Object*)) { RadiusCalc_ = Calc; }
private:
	float(*RadiusCalc_)(Object*);
	float Radius_;
#ifdef _WIN64
	char buffer[4] = { 0 };
#endif
};

/*
	SphereCollision メモリイメージ

	Collision{
		oidCollision_		12byte
		currentCollison_	12byte

		owner_		16byte
		pos_		24byte
		
		if _X86_
			posCalc_	4byte	shape_	2byte	state_	2byte

		else if _M_X64
			posCalc_	8byte
			shape_	4byte	state_	4byte

		vfptr		8byte
	}

	radius_		8byte
	if _X86_
		radiusCalc_	4byte	buffer	4byte
	else if _M_X64
		radiusCalc_	8byte
*/

namespace Size {
	constexpr size_t SPHERE_COLLISION = sizeof(SphereCollision);
}