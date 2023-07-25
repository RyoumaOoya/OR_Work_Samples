#pragma once
#include "Object/Object.h"
#include "BulletParameter.h"
#include "Src/Collision/SphereCollision.h"

class BulletBase :public Object{
public:
	BulletBase();
	virtual ~BulletBase() override;

	virtual Pointer<BulletBase> Clone() const = 0;

	virtual void OnCollisionBegin(Pointer<Object> other) override;
	virtual void OnCollisionEnter(Pointer<Object> other) override;
	virtual void OnCollisionEnd(Pointer<Object> other) override;
public:
	virtual void Init()override;
	virtual void Init(const Pointer<BulletBase>& this_ptr);
	virtual void Load()override;
	virtual void Step()override;
	virtual void Delete()override;
	virtual void Fin()override;

	//コリジョンデータを生成
	void CreateCollision();
	//死亡時の共通処理
	void Death();
	//色を帰る
	void ChangeColor();
	virtual void SetMove(Vector move) {
		Move_ = move;
	}
protected:
	Pointer<SphereCollision> Coll_;
	Vector Move_;
	float Time_;
	BULLET_ID ID_;
};