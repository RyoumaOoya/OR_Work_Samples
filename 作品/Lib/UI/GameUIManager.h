#pragma once
#include "GameUISprite.h"
#include <vector>
#include "../ManagerBase.h"
#include "../GameSystem/Allocator/Allocator.h"

namespace MyLib
{
	//UIä«óùópÉNÉâÉX
	class GameUIManager :public SingletonBase<GameUIManager> {
		MANAGER;
	private:
		GameUIManager();
		~GameUIManager()override;
	public:
		Pointer<GameUI> CreateUI(int type);
		Pointer<GameUI> CreateUI(int type, const Vector2D& in_pos, const char* file_path);

		void Init()override {}
		void Load()override {}
		void Step()override;
		void ConflictStep()override {}
		void UpDate()override {}
		void FrontDraw()override;
		void Delete()override {}
		void Fin()override;
		void DeleteAll();

		void SetCurrentTarget(Pointer<GameUI> ptr) {
			//UIé©ëÃÇÕÇªÇÒÇ»Ç…êîÇ™ëùÇ¶Ç»Ç¢Ç∆âºíËÇµÇƒforï∂Ç≈âÒÇ∑
			for (auto ui : ui_vec_) {
				if (ui->isCurrentTaraget()) {
					ui->SetCurrentTaraget(false);
					break;
				}
			}
			ptr->SetCurrentTaraget(true);
		}
		void SetCurrentTarget(size_t index) {
			//UIé©ëÃÇÕÇªÇÒÇ»Ç…êîÇ™ëùÇ¶Ç»Ç¢Ç∆âºíËÇµÇƒforï∂Ç≈âÒÇ∑
			for (auto ui : ui_vec_) {
				if (ui->isCurrentTaraget()) {
					ui->SetCurrentTaraget(false);
					break;
				}
			}
			ui_vec_[index]->SetCurrentTaraget(true);
		}
	private:
		std::vector<Pointer<GameUI>> ui_vec_;
	};
}
namespace Size {
	constexpr size_t GAME_UI_MANAGER = sizeof(MyLib::GameUIManager);
}