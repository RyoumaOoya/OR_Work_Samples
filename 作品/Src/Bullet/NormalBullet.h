#pragma once
#include "BulletBase.h"

//�w�肳�ꂽ�p�x�ɔ��ł����e
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

