#pragma once
#include "EnemyParameter.h"
#include "ManagerBase.h"
#include <list>
#include <vector>
#include <functional>

class EnemyBase;

typedef PTR_LIST(EnemyBase) EnemyList;

class EnemyManager :public SingletonBase<EnemyManager>
{
	MANAGER;
private:
	EnemyManager();
	~EnemyManager();
public:
	void Init()override;
	void Load()override;
	void Step()override;
	void DeleteStep()override;
	void ConflictStep()override;
	void UpDate()override;
	void FrontDraw()override;
	void Delete()override;
	void Fin()override;

	Pointer<EnemyBase> CreateEntity(EnemyID ID);

	EnemyList& GetEnemyList();
private:
	PTR_VECTOR(EnemyBase) Prototypes_;
	EnemyList List_;
private:
	//マルチスレッド用変数
	CriticalSection EnemyCS_;
	decltype(List_.begin()) EnemyListItr_;
};

