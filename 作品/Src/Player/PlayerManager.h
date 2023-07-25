#pragma once
#include "Player.h"
#include "Define.h"
#include "PlayerParameter.h"
#include "ManagerBase.h"
#include "GameSystem/Allocator/Allocator.h"
#include <queue>

#define GET_PLAYER_POS PlayerManager::GetInstance()->GetPlayer()->GetPos()

class PlayerManager: public SingletonBase<PlayerManager> {
	MANAGER;
private:
	PlayerManager();
	~PlayerManager();

public:
	void Init()override;
	void Load()override;
	void Step()override;
	void ConflictStep()override {}
	void UpDate()override;
	void FrontDraw()override;
	void Delete()override;
	void Fin()override;

	void AddQueueObjectPointer(std::queue<Pointer<Object>>* queue) { if (Player_)queue->push((Pointer<Object>)Player_); }

	Pointer<Player> CreatePlayer() { return (!Player_) ? Player_ = MyNew(Player, AllocType::Player, Tags(MemTag::Player)) : Player_; }
	Pointer<Player> GetPlayer() { return (Player_) ? Player_ : CreatePlayer(); }
	void DeletePlayer() { if (Player_)MyFree(Player_); }
private:
	Pointer<Player> Player_;
};

namespace Size {
	constexpr size_t PLAYER_MANAGER = sizeof(PlayerManager);
}