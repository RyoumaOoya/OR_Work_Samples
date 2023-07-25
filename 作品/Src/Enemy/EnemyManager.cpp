#include "EnemyManager.h"
#include "EnemyBase.h"
#include "Enemy1.h"
#include "GameSystem/Thread/ThreadManager.h"
#include "Math/MyMath.h"

EnemyManager::EnemyManager()
{
	Index_ = MANAGER_INDEX::MultiThread;
}

EnemyManager::~EnemyManager()
{
	Fin();
}

void EnemyManager::Init()
{
#define ENEMY_PROTOTYPE(class_name) MyNew(class_name, AllocType::Prototype, Tags(MemTag::Prototype))
	Prototypes_.push_back(ENEMY_PROTOTYPE(Enemy1));
#undef ENEMY_PROTOTYPE
}

void EnemyManager::Load()
{
	for (auto prototype : Prototypes_) {
		prototype->Load();
	}
}

void EnemyManager::Step()
{
	EnemyListItr_ = List_.begin();
	static auto func = [this]() {
		decltype(EnemyListItr_) temp;
		//Pointer<EnemyBase> enemy;
		while (true)
		{
			{
				ScopedCS s_cs(&EnemyCS_);
				if (EnemyListItr_ == List_.end())
					return;

				//enemy = *EnemyListItr_;

				temp = EnemyListItr_++;
			}
			(*temp)->Step();
		}
	};

	ThreadManager::PushFunc(func, MIN(ThreadManager::GetThreadNum(), List_.size()));
}

void EnemyManager::DeleteStep()
{
	for (auto itr = List_.begin(); itr != List_.end();) {
		auto enemy = *itr;
		if (enemy->RenderHasTag(RenderTags::isDeleted)) {
			MyFree(enemy);
			itr = List_.erase(itr);
		}
		else itr++;
	}
}

void EnemyManager::ConflictStep()
{
	for (auto enemy : List_) {
		enemy->ConflictStep();
	}
}

void EnemyManager::UpDate()
{
	EnemyListItr_ = List_.begin();
	static auto func = [this]() {
		decltype(EnemyListItr_) temp;
		//Pointer<EnemyBase> enemy;
		while (true)
		{
			{
				ScopedCS s_cs(&EnemyCS_);
				if (EnemyListItr_ == List_.end())
					return;

				//enemy = *EnemyListItr_;

				temp = EnemyListItr_++;
			}
			(*temp)->UpDate();
		}
	};

	ThreadManager::PushFunc(func, MIN(ThreadManager::GetThreadNum(), List_.size()));
}

void EnemyManager::FrontDraw()
{
	DrawFormatString(0, 3_ROW, color::Black, "enemy = %d", List_.size());
}

void EnemyManager::Delete()
{
	for (auto itr = List_.begin(); itr != List_.end();) {
		MyFree(*itr);
		itr = List_.erase(itr);
	}
}

void EnemyManager::Fin()
{
	Delete();
	for (auto itr = Prototypes_.begin(); itr != Prototypes_.end();) {
		MyFree(*itr);
		itr = Prototypes_.erase(itr);
	}
}

Pointer<EnemyBase> EnemyManager::CreateEntity(EnemyID ID)
{
	auto ptr = Prototypes_[static_cast<int>(ID)]->Clone();
	List_.push_back(ptr);
	return ptr;
}

EnemyList& EnemyManager::GetEnemyList()
{
	return List_;
}
