#pragma once
#include "BulletBase.h"

//指定された角度に飛んでいく弾
class NormalBullet : public BulletBase {
public:
	NormalBullet();
	~NormalBullet() override;
	Pointer<BulletBase> Clone() const override;
public:
	void Init(const Pointer<BulletBase>& this_ptr)override;
	void Step()override;
	void Fin()override;
};

