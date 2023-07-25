#include "GameUIManager.h"
#include "GameUIParameter.h"
#include "GameUIAnimation.h"

namespace MyLib
{
	GameUIManager::GameUIManager()
	{
		Index_ = MANAGER_INDEX::MultiThread;
	}

	GameUIManager::~GameUIManager()
	{
		Fin();
	}

	// UI����
	Pointer<GameUI> GameUIManager::CreateUI(int type)
	{
		Pointer<GameUI> ui;
		switch (type) {
		case (int)UI_TYPE::SPRITE: ui = (Pointer<GameUI>)MyNew(GameUISprite, AllocType::UI, Tags(MemTag::UI)); break;
		}

		ui_vec_.push_back(ui);
		return ui;
	}

	Pointer<GameUI> GameUIManager::CreateUI(int type, const Vector2D& in_pos, const char* file_path)
	{
		Pointer<GameUI> ui = CreateUI(type);
		ui->Init(in_pos, ui);
		ui->Load(file_path);

		ui->PushSpriteList();
		return ui;
	}

	void GameUIManager::Step()
	{
		for (auto& itr : ui_vec_) {
			itr->Step();
		}
	}

	void GameUIManager::FrontDraw()
	{
		for (auto& itr : ui_vec_) {
			itr->Draw();
		}
	}

	void GameUIManager::Fin()
	{
		DeleteAll();
	}

	// vector����UI�����ׂč폜
	void GameUIManager::DeleteAll()
	{
		// �܂�UI���폜���Ă���
		for (auto itr = ui_vec_.begin(); itr != ui_vec_.end(); itr++) {
			Pointer<GameUI> ui = (*itr);
			MyFree(ui);
		}
		// �z����폜����
		ui_vec_.clear();
		ui_vec_.shrink_to_fit();

		// UI�A�j���[�V�������폜
		GameUIAnimationManager::DeleteAnimation();
	}
}