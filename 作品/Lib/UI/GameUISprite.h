#pragma once
#include "GameUI.h"

namespace MyLib
{
	//UI�X�v���C�g�N���X
	class GameUISprite : public GameUI {
	public:
		GameUISprite();
		~GameUISprite();

	public:
		void Init(const Vector2D& in_pos, const Pointer<GameUI>& this_ptr) override;
		void Load(const char* file_path) override;
		void Step() override;
		void Draw() override;
		void Delete() override;
		void Fin() override;

	private:
		//char buffer[4] = { 0 };
	};
}

namespace Size {
	constexpr size_t GAME_UI_SPRITE = sizeof(MyLib::GameUISprite);
}