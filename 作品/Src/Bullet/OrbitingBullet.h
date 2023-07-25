#pragma once
#include "BulletBase.h"

//ƒvƒŒƒCƒ„[‚ÌüˆÍ‚ğ‰ñ“]‚·‚é’e
class OrbitingBullet : public BulletBase
{
public:
	OrbitingBullet();
	~OrbitingBullet()override;

	Pointer<BulletBase> Clone() const override;
	void OnCollisionBegin(Pointer<Object> other) {}
	void Init(const Pointer<BulletBase>& this_ptr)override;
	void Step()override;
	void Fin()override;

	void SetRot(float Rot) { Rot_ = Rot; }
private:
	static constexpr float RotSpeed = 180_DegreeToRadian_F;
	static constexpr float OffSet = 8.f;
	float Rot_;
};

