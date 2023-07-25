#include "StageManager.h"
#include "Constant/Constant.h"

StageManager::StageManager() :Stage_()
{
	Index_ = MANAGER_INDEX::Last;
}

StageManager::~StageManager()
{
	Fin();
}

void StageManager::Init(STAGE_ID ID)
{
	CreateStage(ID);
}

void StageManager::Load()
{
	Stage_->Load();
}

void StageManager::Step()
{
	if(Stage_)
		Stage_->Step();
}

void StageManager::UpDate()
{
	if (Stage_)
		Stage_->UpDate();
}
void StageManager::FrontDraw()
{
}

void StageManager::Delete()
{
	if (Stage_)
		Stage_->Delete();
}

void StageManager::Fin()
{
	if (Stage_)
		Stage_->Fin();
	DeleteStage();
}

void StageManager::AddQueueObjectPointer(std::queue<Pointer<Object>>* queue)
{
	queue->push(Stage_->Map_);
	queue->push(Stage_->Sky_);
}

void StageManager::CreateStage(STAGE_ID ID)
{
	DeleteStage();

	Stage_ = MyNew(Stage, AllocType::Stage, Tags(MemTag::Stage));
	Stage_->Init(static_cast<int>(ID));
}

void StageManager::DeleteStage()
{
	if (Stage_) {
		MyFree(Stage_);
	}
}
