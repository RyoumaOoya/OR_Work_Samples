#pragma once
#include "Object/Object.h"
#include "Src/Collision/CollisionManager.h"
#include "ItemParameter.h"

class Item:public Object
{
public:
	Item(ITEM_ID ID);
	~Item() override;

	void OnCollisionBegin(Pointer<Object> other) override;
	void OnCollisionEnter(Pointer<Object> other) override;
	void OnCollisionEnd(Pointer<Object> other) override;

	Pointer<Item> Clone() const;
public:
	void Init() override;
	void Init(const Pointer<Item>& this_ptr);
	void Load() override;
	void Step() override;
	void Delete() override;
	void Fin() override;

	ITEM_ID GetID() const{ return ID_; }
	Pointer<SphereCollision> GetCollision() const { return Collision_; }
	void SetMove(Vector m) { Move_ = m; }
	auto GetMove() const { return Move_; }
private:
	void Death();
private:
	Pointer<SphereCollision> Collision_;
	Vector Move_;

	ITEM_ID ID_;
	float Time_;
};

namespace Size {
	constexpr size_t ITEM = sizeof(Item);
}