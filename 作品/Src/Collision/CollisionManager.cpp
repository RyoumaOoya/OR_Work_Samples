#include "CollisionManager.h"
#include "Math/MyMath.h"
#include "GameSystem/FrameRate/FrameRate.h"
#include "GameSystem/Thread/ThreadManager.h"
#include <queue>
#include <array>
#include "CollisionParameter.h"

struct coll_ptr {
	coll_ptr() {}
	coll_ptr(const Pointer<Collision>& A, const Pointer<Collision>& B) :a(A), b(B) {};
	Pointer<Collision> a, b;
};

static moodycamel::ConcurrentQueue<coll_ptr> coll_ptr_que;

//外部ファイルからの読み込みを検討してもいい
CollisionType CollisionManager::CollisionTypeList_[(int)CollisionCategory::NUM][(int)CollisionCategory::NUM] =
{
	//	ステージ		(0b0000'0001) 
	{CollisionType::NOT_COLL, CollisionType::NOT_RACE, CollisionType::NOT_RACE, CollisionType::NOT_RACE, CollisionType::NOT_COLL},
	//	アイテム		(0b0000'0010)
	{CollisionType::NOT_RACE, CollisionType::NOT_COLL, CollisionType::NOT_RACE, CollisionType::NOT_COLL, CollisionType::NOT_COLL},
	//	プレイヤー		(0b0000'0100)
	{CollisionType::NOT_RACE, CollisionType::NOT_RACE, CollisionType::NOT_COLL, CollisionType::NOT_RACE, CollisionType::NOT_COLL},
	//	エネミー		(0b0001'0000)
	{CollisionType::NOT_RACE, CollisionType::NOT_COLL, CollisionType::NOT_RACE, CollisionType::NOT_COLL, CollisionType::NOT_RACE},
	//	BULLET			
	{CollisionType::NOT_COLL, CollisionType::NOT_COLL, CollisionType::NOT_COLL, CollisionType::NOT_RACE, CollisionType::NOT_COLL}
};

CollisionManager::CollisionManager()
{
	Index_ = MANAGER_INDEX::CollisionStep;
}

CollisionManager::~CollisionManager()
{
	Fin();
}

void CollisionManager::Init()
{
}

void CollisionManager::Step()
{
	ThreadManager::WaitTask();
#define COLL_FUNC(index) [this]() {\
		while (true)\
		{\
			Pointer<Collision> coll;\
			{\
				ScopedCS s_cs(&CollCS_);\
				if (CollListItr_[index] == Collisions_[index].end()) return;\
				coll = *CollListItr_[index];\
				++CollListItr_[index];\
			}\
			if(coll->isActive())\
				coll->Step();\
		}\
	}
	static const std::function<void()>func[ICollisionCategory::NUM] = { COLL_FUNC(0), COLL_FUNC(1), COLL_FUNC(2), COLL_FUNC(3), COLL_FUNC(4) };
#undef COLL_FUNC
	for (int i = 0; i < (int)CollisionCategory::NUM; i++) {
		CollListItr_[i] = Collisions_[i].begin();
		ThreadManager::PushFunc(func[i], MIN(ThreadManager::GetThreadNum(), Collisions_[i].size()));
	}

	ThreadManager::WaitTask();

	CheckCollision();
}

void CollisionManager::DeleteStep()
{
	//使い終わったコリジョンの削除
	for (int i = 0; i < ICollisionCategory::NUM; i++) {
		for (auto itr = Collisions_[i].begin(); itr != Collisions_[i].end();) {
			Pointer<Collision> collision = (*itr);
			if (!collision->isActive()) {
				MyFree(collision);
				itr = Collisions_[i].erase(itr);
			}
			else {
				++itr;
			}
		}
	}
}

void CollisionManager::UpDate()
{
#define COLL_FUNC(index) [this]() {\
		while (true)\
		{\
			Pointer<Collision> coll;\
			{\
				ScopedCS s_cs(&CollCS_);\
				if (CollListItr_[index] == Collisions_[index].end()) return;\
				coll = *CollListItr_[index];\
				++CollListItr_[index];\
			}\
			if(coll->isActive())\
				coll->UpDate();\
		}\
	}
	static const std::function<void()>func[ICollisionCategory::NUM] = { COLL_FUNC(0), COLL_FUNC(1), COLL_FUNC(2), COLL_FUNC(3), COLL_FUNC(4) };
#undef COLL_FUNC
	for (int i = 0; i < (int)CollisionCategory::NUM; i++) {
		CollListItr_[i] = Collisions_[i].begin();
		ThreadManager::PushFunc(func[i], MIN(ThreadManager::GetThreadNum(), Collisions_[i].size()));
	}
}

void CollisionManager::FrontDraw()
{
	for (int i = 0; i < ICollisionCategory::NUM; i++) {
		for (auto collision : Collisions_[i]) {
			if (collision->isActive())
				collision->Draw();
		}
	}
}

void CollisionManager::Delete()
{
	for (int i = 0; i < ICollisionCategory::NUM; i++) {
		for (auto itr = Collisions_[i].begin(); itr != Collisions_[i].end(); itr++) {
			MyFree(*itr);
			Collisions_[i].erase(itr);
		}
	}
}

void CollisionManager::Fin()
{
	Delete();
}

// コリジョン生成
Pointer<Collision> CollisionManager::CreateCollision(int shape, int category)
{
	Pointer<Collision> collision;
	switch (shape) {
	case ICollisionShape::SPHERE:	collision = MyNew(SphereCollision, AllocType::Collision, Tags(MemTag::Collision));	break;
	case ICollisionShape::BOX:		collision = MyNew(BoxCollision, AllocType::Collision, Tags(MemTag::Collision));		break;
	case ICollisionShape::VECTOR:	collision = MyNew(VectorCollision, AllocType::Collision, Tags(MemTag::Collision));	break;
	case ICollisionShape::MODEL:	collision = MyNew(ModelCollision, AllocType::Collision, Tags(MemTag::Collision));	break;
	}

	if (collision) {
		collision->SetState(static_cast<int>(CollisionState::ACTIVE));
		Collisions_[category].push_back(collision);
	}
	return collision;
}

// コリジョンチェック
void CollisionManager::CheckCollision()
{
	static auto check_func = [this]() {
		coll_ptr ptr; 
		while (true) {
			if (!coll_ptr_que.try_dequeue(ptr))
				break;
			if (ptr.a->CheckCollision(ptr.b)) {
				ptr.a->EnterCollision(ptr.b);
				ptr.a->EnterCollision(ptr.b);
			}
		}};
	static auto calc_func = [this]() {
		coll_ptr ptr; 
		while (true) {
			if (!coll_ptr_que.try_dequeue(ptr))
				break;
			//当たった状態を取得
			int HitType = static_cast<int>(ptr.a->CheckHitType(ptr.b));
			if (HitType <= static_cast<int>(Collision::HitType::NONE) || HitType >= static_cast<int>(Collision::HitType::NUM)) continue;

			//オーナー取得
			Pointer<Object> Owner = ptr.a->GetOwner();
			Pointer<Object> other = ptr.b->GetOwner();
			//オーナーがnullでない && HitTypeが有効値
			if (!Owner || !other) continue;

			static void (Object::*const Fanc[])(Pointer<Object>) = { &Object::OnCollisionBegin, &Object::OnCollisionEnter, &Object::OnCollisionEnd };
			(Owner->*Fanc[HitType - 1])(other);
			(other->*Fanc[HitType - 1])(Owner);
		}};

	for (int i = 0; i < ICollisionCategory::NUM; i++) {
		for (int k = i; k < ICollisionCategory::NUM; k++) {
			if (isCollision(i, k) != CollisionType::NOT_COLL) {
				PushCollision(i, k);
			}
		}
	}

	ThreadManager::PushFunc(check_func, ThreadManager::GetThreadNum());
	ThreadManager::WaitTask();
	for (int i = 0; i < ICollisionCategory::NUM; i++) {
		for (int k = i; k < ICollisionCategory::NUM; k++) {
			if (isCollision(i, k) == CollisionType::NOT_RACE) {
				PushCollision(i, k);
			}
		}
	}

	ThreadManager::PushFunc(calc_func, ThreadManager::GetThreadNum());
	ThreadManager::WaitTask();

	for (int i = 0; i < ICollisionCategory::NUM; i++) {
		for (int k = i; k < ICollisionCategory::NUM; k++) {
			if (isCollision(i, k) == CollisionType::CALC_RACE) {
				CalcCollision(i, k);
			}
		}
	}
}

CollisionType CollisionManager::isCollision(int a, int b)
{
	return CollisionTypeList_[a][b];
}

void CollisionManager::PushCollision(int a, int b)
{
	const auto a_collisions = Collisions_[a];
	const auto b_collisions = Collisions_[b];
	for (auto a_collision : a_collisions) {
		if (!a_collision->isCheck()) continue;
		for (auto b_collision : b_collisions) {
			if (!b_collision->isCheck()) continue;
			coll_ptr_que.enqueue(coll_ptr(a_collision, b_collision));
		}
	}
}

void CollisionManager::CalcCollision(int a, int b)
{
	const auto a_collisions = Collisions_[a];
	const auto b_collisions = Collisions_[b];
	for (auto collision : a_collisions) {
		if (!collision->isCheck()) continue;
		for (auto other_collision : b_collisions) {
			if (!other_collision->isCheck()) continue;

			//当たった状態を取得
			int HitType = (int)collision->CheckHitType(other_collision);
			if (HitType <= (int)Collision::HitType::NONE || HitType >= (int)Collision::HitType::NUM) continue;

			//オーナー取得
			Pointer<Object> Owner = collision->GetOwner();
			Pointer<Object> other = other_collision->GetOwner();
			//オーナーがnullでない && HitTypeが有効値
			if (!Owner || !other) continue;

			static void (Object::*const Fanc[])(Pointer<Object>) = { &Object::OnCollisionBegin, &Object::OnCollisionEnter, &Object::OnCollisionEnd };

			(Owner->*Fanc[HitType - 1])(other);
			(other->*Fanc[HitType - 1])(Owner);
		}
	}
}