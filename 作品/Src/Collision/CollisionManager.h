#pragma once
// コリジョン管理クラス
#include "Collision.h"
#include "CollisionParameter.h"
#include "SphereCollision.h"
#include "BoxCollision.h"
#include "VectorCollision.h"
#include "ModelCollision.h"
#include "GameSystem/Allocator/Allocator.h"
#include "ManagerBase.h"
#include "Define.h"
#include <list>

constexpr size_t COLL_TYPE_BIT_SIZE = 2;

class CollisionManager :public SingletonBase<CollisionManager>{
	MANAGER;
private:
	CollisionManager();
	~CollisionManager();
public:
	void Init()override;
	void Load()override {}
	void Step()override;
	void DeleteStep()override;
	void ConflictStep()override {}
	void UpDate()override;
	void FrontDraw()override;
	void Delete()override;
	void Fin()override;

public:
	/*
		コリジョンの生成
		引数: shape 形, category 種類
		戻り値: コリジョンのポインタ
	*/
	Pointer<Collision> CreateCollision(int shape, int category);
	/*
		コリジョンの生成
		引数: shape 形, category 種類, args コンストラクタ引数
		戻り値: コリジョンのポインタ
	*/
	template<class... Args>
	Pointer<Collision> CreateCollision(int shape, int category, const Args&... args) {
		Pointer<Collision> collision;
		switch (shape) {
		case ICollisionShape::SPHERE:	collision = MyNew(SphereCollision, AllocType::Collision, Tags(MemTag::Collision), args...); break;
		case ICollisionShape::BOX:		collision = MyNew(BoxCollision, AllocType::Collision, Tags(MemTag::Collision), args...);	break;
		case ICollisionShape::VECTOR:	collision = MyNew(VectorCollision, AllocType::Collision, Tags(MemTag::Collision), args...); break;
		case ICollisionShape::MODEL:	collision = MyNew(ModelCollision, AllocType::Collision, Tags(MemTag::Collision), args...);	break;
		}

		if (collision) {
			collision->SetState(static_cast<int>(CollisionState::ACTIVE));
			Collisions_[category].push_back(collision);
		}
		return collision;
	}

public:
	void CheckCollision();	// 当たり判定チェック

	std::list<Pointer<Collision>>* GetCollisionList() { return Collisions_; }
private:
	CollisionType isCollision(int a, int b);
public:
	void PushCollision(int a, int b);
	void CalcCollision(int a, int b);
private:
	static CollisionType CollisionTypeList_[(int)CollisionCategory::NUM][(int)CollisionCategory::NUM];	//当たり判定の情報
	PTR_LIST(Collision) Collisions_[(int)CollisionCategory::NUM];	// 管理リスト

private:
	//マルチスレッド用変数
	CriticalSection CollCS_;
	decltype(Collisions_[0].begin()) CollListItr_[ICollisionCategory::NUM];
};

namespace Size {
	constexpr size_t COLLISION_MANAGER = sizeof(CollisionManager);
}