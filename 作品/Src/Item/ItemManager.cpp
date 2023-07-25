#include "ItemManager.h"
#include "GameSystem/Thread/CriticalSection.h"
#include "GameSystem/Thread/ThreadManager.h"
#include "Math/MyMath.h"

ItemManager::ItemManager()
{
	Index_ = MANAGER_INDEX::MultiThread;
}

ItemManager::~ItemManager()
{
	Fin();
}

void ItemManager::Init()
{
	Items_.clear();
	auto ptr = Prototype_;
	for (int i = 0; i < IITEM_ID::Num; i++, ptr++) {
		*ptr = MyNew(Item, AllocType::Prototype, TagResult({ MemTag::Prototype }), static_cast<ITEM_ID>(i));
	}
}

void ItemManager::Load()
{
	auto ptr = Prototype_;
	for (int i = 0; i < IITEM_ID::Num; i++, ptr++) {
		(*ptr)->Load();
	}
}

void ItemManager::Step()
{
	ItemListItr_ = Items_.begin();
	auto func = [this]() {
		while (true)
		{
			Pointer<Item> item;
			{
				ScopedCS s_cs(&ItemCS_);
				if (ItemListItr_ == Items_.end())
					return;

				item = *ItemListItr_;

				++ItemListItr_;
			}
			if (item->IsActive())
				item->Step();
		}
	};

	ThreadManager::PushFunc(func, MIN(ThreadManager::GetThreadNum(), Items_.size()));
}

void ItemManager::DeleteStep()
{
	for (auto itr = Items_.begin(); itr != Items_.end();) {
		auto item = *itr;
		if (item->RenderHasTag(RenderTags::isDeleted)) {
			MyFree(item);
			itr = Items_.erase(itr);
		}
		else itr++;
	}
}

void ItemManager::UpDate()
{
	ItemListItr_ = Items_.begin();
	auto func = [this] () {
		while (true)
		{
			Pointer<Item> item;
			{
				ScopedCS s_cs(&ItemCS_);
				if (ItemListItr_ == Items_.end())
					return;

				item = *ItemListItr_;
				++ItemListItr_;
			}
			item->UpDate();
		}
	};

	ThreadManager::PushFunc(func, MIN(ThreadManager::GetThreadNum(), Items_.size()));
}

void ItemManager::FrontDraw()
{
}

void ItemManager::Delete()
{
	for (auto ptr : Items_)
		ptr->Delete();
}

void ItemManager::Fin()
{
	for (auto itr = Items_.begin(); itr != Items_.end();) {
		MyFree(*itr);
		itr = Items_.erase(itr);
	}
	auto ptr = Prototype_;
	for (int i = 0; i < IITEM_ID::Num; i++, ptr++) {
		if (*ptr) {
			MyFree(*ptr);
		}
	}
}

void ItemManager::AddQueueObjectPointer(std::queue<Pointer<Object>>* queue)
{
	for (auto& item : Items_) {
		queue->push(item);
	}
}

Pointer<Item> ItemManager::CreateItem(ITEM_ID ID, Vector Pos)
{
	Pointer<Item> item = Prototype_[static_cast<int>(ID)]->Clone();

	item->Init(item);
	item->SetPos(Pos);

	Items_.push_back(item);

	return item;
}
