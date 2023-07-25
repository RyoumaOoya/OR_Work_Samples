#pragma once
// ボックスの当たり判定（AABB）
#include "DxLib.h"
#include "Collision.h"

class BoxCollision : public Collision {
public:
	BoxCollision();
	BoxCollision(const Vector& pos, const Vector& size);
	~BoxCollision() override;

public:
	void Init() override;
	void Step() override;
	void UpDate() override;
	void Draw() override;
	void Fin() override;
	Pointer<Collision> Clone() override;

	bool CheckCollision(Pointer<Collision> other) override;
public:
	Vector GetSize() const { return Size_; }

	void SetSize(Vector size) { Size_ = size; }
	void SetSize(float width, float height, float depth) { Size_ = VGet(width, height, depth); }

	//サイズの更新処理の設定
	void SetSizeCalc(Vector(*Calc)(Object*)) { SizeCalc_ = Calc; }
private:
	Vector(*SizeCalc_)(Object*);

	Vector Size_;
private:
#ifdef _WIN64
	char buffer[4] = { 0 };
#endif // _WIN64
};

/*
	BoxCollision メモリイメージ

	Collision{
		oidCollision_		12byte
		currentCollison_	12byte

		owner_		16byte
		pos_		24byte

		if not _WIN64
			posCalc_	8byte
			shape_	4byte	state_	4byte

		else
			posCalc_	4byte	shape_	2byte	state_	2byte

		vfptr		8byte
	}

	size_		24byte

	if not _WIN64
		sizeCalc_	4byte	buffer		4byte
*/

namespace Size
{
	constexpr size_t BOX_COLLISON = sizeof(BoxCollision);
}