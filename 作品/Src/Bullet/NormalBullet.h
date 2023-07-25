#pragma once
#include "BulletBase.h"

//Žw’è‚³‚ê‚½Šp“x‚É”ò‚ñ‚Å‚¢‚­’e
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

