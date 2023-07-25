#pragma once
#include "Item.h"
#include "ItemParameter.h"
#include "ManagerBase.h"
#include <vector>
#include <queue>

class ItemManager :public SingletonBase<ItemManager>
{
	MANAGER;
private:
	ItemManager();
	~ItemManager();
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

	void AddQueueObjectPointer(std::queue<Pointer<Object>>* queue);

	Pointer<Item> CreateItem(ITEM_ID ID, Vector Pos = Vector(0.f, 0.f, 0.f));
private:
	PTR_LIST(Item) Items_;
	Pointer<Item> Prototype_[IITEM_ID::Num];
private:
	CriticalSection ItemCS_;
	decltype(Items_.begin()) ItemListItr_;
};

namespace Size {
	constexpr size_t ITEM_MANAGER = sizeof(ItemManager);
}