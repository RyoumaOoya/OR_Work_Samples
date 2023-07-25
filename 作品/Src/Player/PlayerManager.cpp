#include "PlayerManager.h"
#include "Input/InputManager.h"

PlayerManager::PlayerManager() :Player_()
{
	Index_ = MANAGER_INDEX::SequentialProcess;
}

PlayerManager::~PlayerManager()
{
	Fin();
}

void PlayerManager::Init()
{
	CreatePlayer();
	if (Player_)
		Player_->Init(Player_);
}

void PlayerManager::Load()
{
	if (Player_)
		Player_->Load();
}

void PlayerManager::Step()
{
	if (Player_)
		Player_->Step();
}

void PlayerManager::UpDate()
{
	if (Player_)
		Player_->UpDate();
}

void PlayerManager::FrontDraw()
{
	if (Player_)
		Player_->FrontDraw();
}

void PlayerManager::Delete()
{
	if (Player_)
		Player_->Delete();
}

void PlayerManager::Fin()
{
	if (Player_) {
		Player_->Fin();
		DeletePlayer();
	}
}
