#pragma once
#include "ManagerBase.h"
#include "BulletBase.h"

class BulletManager :public SingletonBase<BulletManager>{
	MANAGER;
private:
	BulletManager();
	~BulletManager();

public:
	void Init() override;
	void Load() override;
	void Step() override;
	void DeleteStep() override;
	void ConflictStep()override {}
	void UpDate() override;
	void FrontDraw() override;
	void Delete() override;
	void Fin() override;

	/*
		ËŒ‚
		ˆø”: ID í—Ş, ShooterPos Ëè‚ÌˆÊ’u, angle ËŒ‚‚·‚é•ûŒü
	*/
	void Shot(SHOT_ID ID, const Vector& ShooterPos, float angle);
private:
	//’e‚ğ¶¬
	Pointer<BulletBase> CreateBullet(BULLET_ID id);

	//ËŒ‚(’P”­)
	void SingleShot(const Vector& ShooterPos, float angle);
	//ËŒ‚(2way)
	void DoubleShot(const Vector& ShooterPos, float angle);
	//ËŒ‚(3way)
	void TripleShot(const Vector& ShooterPos, float angle);
	//üˆÍ‚ğ‰ñ“]‚·‚é’e
	void OrbitingShot(const Vector& ShooterPos, float angle);
private:
	PTR_LIST(BulletBase) Bullets;
	PTR_VECTOR(BulletBase) Prototypes;
};

