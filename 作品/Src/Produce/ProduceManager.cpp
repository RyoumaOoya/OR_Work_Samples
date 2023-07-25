#include "ProduceManager.h"
#include "ProduceEnemy.h"
#include "ProduceParameter.h"
#include "GameSystem/Thread/ThreadManager.h"
#include "Math/MyMath.h"

ProduceManager::ProduceManager()
{
	Index_ = MANAGER_INDEX::SequentialProcess;
}

ProduceManager::~ProduceManager()
{
}

void ProduceManager::Init()
{
	for (auto obj: List_) {
		obj->Init();
	}
}

void ProduceManager::Load()
{
	for (auto obj : List_) {
		obj->Load();
	}
}

void ProduceManager::OnLoadEnd()
{
	for (auto positioning_data : PRODUCE_POSITIONING_DATA) {
		if (positioning_data.ID == PRODUCE_OBJECT_ID::END)
			break;

		auto p_obj = CreateProduceObject(static_cast<int>(positioning_data.ID));
		p_obj->SetPos(positioning_data.Pos);
	}
}

void ProduceManager::Step()
{
	ThreadManager::WaitTask();

	for (auto itr = List_.begin(); itr != List_.end(); itr++) {
		(*itr)->Step();
	}
}

void ProduceManager::DeleteStep()
{
	for (auto itr = List_.begin(); itr != List_.end();) {
		auto produce = *itr;
		if (produce->RenderHasTag(RenderTags::isDeleted)) {
			MyFree(produce);
			itr = List_.erase(itr);
		}
		else itr++;
	}
}

void ProduceManager::UpDate()
{
	
}

void ProduceManager::FrontDraw()
{
	for (auto obj : List_) {
		obj->FrontDraw();
	}
}

void ProduceManager::Delete()
{
	for (auto obj : List_) {
		obj->Delete();
	}
}

void ProduceManager::Fin()
{
	for (auto itr = List_.begin(); itr != List_.end();) {
		auto obj = *itr;
		obj->Fin();
		MyFree(obj);
		itr = List_.erase(itr);
	}
}

Pointer<ProduceObjectBase> ProduceManager::CreateProduceObject(int id)
{
	Pointer<ProduceObjectBase> obj;
	switch (id)
	{
	case IPRODUCE_OBJECT_ID::ENEMY: obj = MyNew(ProduceEnemy, AllocType::Produce, Tags(MemTag::Produce)); break;
	}
	obj->Init(obj);
	//obj->Load();
	//obj->PushModelList();
	List_.push_back(obj);
	return obj;
}
