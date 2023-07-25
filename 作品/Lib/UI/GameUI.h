#pragma once
#include "../Math/MyMathData.h"
#include "../GameSystem/Allocator/Pointer.h"
#include "../Render/RenderObject/RenderObject2D.h"

namespace MyLib
{
	//UIのベースとなるクラス
	class GameUI {
	public:
		GameUI();
		virtual ~GameUI();

	public:
		virtual void Init(const Vector2D& in_pos, const Pointer<GameUI>& this_ptr) = 0;
		virtual void Load(const char* file_path) = 0;
		virtual void Step() = 0;
		virtual void Draw() = 0;
		virtual void Delete() = 0;
		virtual void Fin() = 0;

		void SetPos(const Vector2D& in_pos) { RenderObj_.Pos_ = in_pos; }
		//void SetSize(const Vector2D& in_size) { size_ = in_size; }
		void SetExRate(const float ex_rate) { RenderObj_.ExRate_ = ex_rate; }
		void SetBlendData(BlendData blend_data) { RenderObj_.BlendData_ = blend_data; }

		Vector2D GetPos() const { return RenderObj_.Pos_; }
		iVector2D GetSize() const { return GraphSize_; }
		float GetScale() const { return RenderObj_.ExRate_; }

		void SetWindowSize();	//画像をWindowサイズに合わせる
		void SetThisPtr(Pointer<GameUI> ptr) { This_ = ptr; }
		void PushSpriteList();
		void EreseSpriteList();

		inline bool isCurrentTaraget() { return CurrentTarget; }
		inline void SetCurrentTaraget(bool b) { CurrentTarget = b; }
	protected:
		Pointer<GameUI> This_;
		RenderObject2D RenderObj_;
		iVector2D GraphSize_;
		bool CurrentTarget;
	};
}
namespace Size {
	constexpr size_t GAME_UI = sizeof(MyLib::GameUI);
}