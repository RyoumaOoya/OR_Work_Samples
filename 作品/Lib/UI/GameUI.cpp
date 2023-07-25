#include "GameUI.h"
#include "../GameSystem/GameSystem.h"
#include "Render/Render.h"

namespace MyLib
{
	GameUI::GameUI() :CurrentTarget(false)
	{
		RenderObj_.ExRate_ = 1.f;
		GraphSize_ = RenderObj_.GetGraphSize();
	}

	GameUI::~GameUI()
	{
	}

	void GameUI::SetWindowSize()
	{
		const auto WINDOW_SIZE = GameSystem::GetWindowSize();
		SetExRate(1.f / (GraphSize_.x / (float)WINDOW_SIZE.x));
	}
	void GameUI::PushSpriteList()
	{
		if (This_) {
			Render::PushSpriteList(This_, &RenderObj_);
		}
	}
	void GameUI::EreseSpriteList()
	{
		Render::EreseSpriteList(This_, &RenderObj_);
	}
}