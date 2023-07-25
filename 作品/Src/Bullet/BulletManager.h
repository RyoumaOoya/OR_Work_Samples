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
		�ˌ�
		����: ID ���, ShooterPos �ˎ�̈ʒu, angle �ˌ��������
	*/
	void Shot(SHOT_ID ID, const Vector& ShooterPos, float angle);
private:
	//�e�𐶐�
	Pointer<BulletBase> CreateBullet(BULLET_ID id);

	//�ˌ�(�P��)
	void SingleShot(const Vector& ShooterPos, float angle);
	//�ˌ�(2way)
	void DoubleShot(const Vector& ShooterPos, float angle);
	//�ˌ�(3way)
	void TripleShot(const Vector& ShooterPos, float angle);
	//���͂���]����e
	void OrbitingShot(const Vector& ShooterPos, float angle);
private:
	PTR_LIST(BulletBase) Bullets;
	PTR_VECTOR(BulletBase) Prototypes;
};

