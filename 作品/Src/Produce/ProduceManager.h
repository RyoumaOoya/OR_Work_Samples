#pragma once
#include "ManagerBase.h"
#include "ProduceObjectBase.h"
#include <list>

class ProduceManager:public SingletonBase<ProduceManager>
{
	MANAGER;
private:
	ProduceManager();
	~ProduceManager();
public:
	void Init()override;
	void Load()override;
	void OnLoadEnd()override;
	void Step()override;
	void DeleteStep()override;
	void ConflictStep()override {}
	void UpDate()override;
	void FrontDraw()override;
	void Delete()override;
	void Fin()override;

	Pointer<ProduceObjectBase> CreateProduceObject(int id);

	auto& GetProduceList() { return List_; }
private:
	PTR_LIST(ProduceObjectBase) List_;
};

