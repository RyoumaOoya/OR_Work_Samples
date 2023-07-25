#include "ManagerManager.h"
#include "GameSystem/Allocator/Allocator.h"
#include "GameSystem/Thread/ThreadManager.h"
#include "Render/Render.h"

float ManagerManager::DeleteTime_ = 0.f;
std::vector<Pointer<ManagerBase>> ManagerManager::Managers_;

namespace {
	constexpr float DELETE_INTERVAL = 1.f;
}

void ManagerManager::Init()
{
	for (auto manager : Managers_) {
		manager->Init();
	}
}

void ManagerManager::Load()
{
	for (auto manager : Managers_) {
		manager->Load();
	}
}

void ManagerManager::OnLoadEnd()
{
	for (auto mng : Managers_) {
		mng->OnLoadEnd();
	}
}

void ManagerManager::Step()
{
	auto itr = Managers_.begin();

	for (;itr != Managers_.end(); itr++) {
		auto manager = *itr;
		if (manager->Index_ != MANAGER_INDEX::MultiThread) {
			break;
		}
		ThreadManager::PushFunc([manager]() mutable {manager->Step(); });
	}
	ThreadManager::WaitTask();
	for (; itr != Managers_.end(); itr++) {
		(*itr)->Step();
	}
}

void ManagerManager::ConflictStep()
{
	ThreadManager::WaitTask();
	for (auto manager : Managers_) {
		manager->ConflictStep();
	}
	for (auto itr = Managers_.begin(); itr != Managers_.end(); itr++) {
		auto manager = *itr;
		ThreadManager::PushFunc([manager]() mutable {manager->DeleteStep(); });
	}
	ThreadManager::WaitTask();
}

void ManagerManager::Update()
{
	for (auto itr = Managers_.begin(); itr != Managers_.end(); itr++) {
		auto manager = *itr;
		ThreadManager::PushFunc([manager]() mutable {manager->UpDate(); });
	}
	ThreadManager::WaitTask();
}

void ManagerManager::ShadowDraw()
{
}

void ManagerManager::Draw()
{
	/*for (auto manager : Managers_) {
		manager->Draw();
	}*/
	Render::Draw();
}

void ManagerManager::FrontDraw()
{
	for (auto manager : Managers_) {
		manager->FrontDraw();
	}
}

void ManagerManager::Delete()
{
	ThreadManager::WaitTask();
	for (auto manager : Managers_) {
		manager->Delete();
	}
}

void ManagerManager::Fin()
{
	ThreadManager::WaitTask();
	for (auto itr = Managers_.begin(); itr != Managers_.end();) {
		{
			auto ptr = *itr;
			ptr->Fin();
			MyFree(ptr); 
		}
		itr = Managers_.erase(itr);
	}
}
