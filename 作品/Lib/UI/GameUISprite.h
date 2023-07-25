#pragma once
#include "GameUI.h"

namespace MyLib
{
	//UIスプライトクラス
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