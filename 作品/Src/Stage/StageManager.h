#pragma once
#include "Stage.h"
#include "StageParameter.h"
#include "ManagerBase.h"
#include <queue>

class StageManager :public SingletonBase<StageManager>
{
	MANAGER;
private:
	StageManager();
	~StageManager();
public:
	void Init()override { Init(STAGE_ID::STAGE_1); }
	void Init(STAGE_ID ID);
	void Load()override;
	void Step()override;
	void ConflictStep()override {}
	void UpDate()override;
	void FrontDraw()override;
	void Delete()override;
	void Fin()override;

	void AddQueueObjectPointer(std::queue<Pointer<Object>>* queue);
public:
	void CreateStage(STAGE_ID ID);
	void DeleteStage();

	auto GetMap() { return (Stage_) ? Stage_->Map_ : Pointer<Map>(); }
private:
	Pointer<Stage> Stage_;
};

namespace Size {
	constexpr size_t STAGE_MANAGER = sizeof(StageManager);
}