#pragma once
#include "EnemyBase.h"

class Enemy1 :public EnemyBase {
public:
	Enemy1();
	~Enemy1() override;

	void OnCollisionBegin(Pointer<Object> other) override;
	void OnCollisionEnter(Pointer<Object> other) override;
	void OnCollisionEnd(Pointer<Object> other) override;

	 Pointer<EnemyBase> Clone() const override;
public:
	void Init()override;
	void Init(const Pointer<EnemyBase>& this_ptr)override;
	void Step()override;
private:
};