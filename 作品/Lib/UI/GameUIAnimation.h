#pragma once
#include "GameUI.h"
#include "DxLib.h"
#include <vector>
#include "../GameSystem/Allocator/Pointer.h"

namespace MyLib
{
	// �A�j���[�V�����x�[�X�N���X
	class GameUIAnimation {
	public:
		GameUIAnimation();
		~GameUIAnimation();
	public:
		virtual void Step() = 0;
	public:
		bool IsPlay() const { return play_flg_; }

		void SetUI(const Pointer<GameUI>& ui) { ui_ = ui; }
		void Play() { play_flg_ = true; }
		void Stop() { play_flg_ = false; }
	protected:
		bool play_flg_;
	private:
#ifdef _X86_
		char buffer[3] = { 0 };
#elif _M_X64
		char buffer[7] = { 0 };
#endif // _X86_
	protected:
		Pointer<GameUI> ui_;
	};

	// ���ŃA�j���[�V����
	class GameUIBlinkAnimation : public GameUIAnimation {
	public:
		GameUIBlinkAnimation();
		~GameUIBlinkAnimation();
	public:
		void Step() override;
	public:
		void SetSpeed(float speed) { speed_ = speed; }
	private:
		float speed_;
		float rad_;
	};

	// �㉺���V�A�j���[�V����
	class GameUIFloatingAnimation : public GameUIAnimation {
	public:
		GameUIFloatingAnimation();
		~GameUIFloatingAnimation();
	public:
		void Step() override;
	public:
		void SetSpeed(float speed) { speed_ = speed; }
		void SetFloating(float center, float size);
	private:
		float speed_;
		float rad_;
		float center_pos_;
		float min_offset_;
		float max_offset_;
	private:
		char buffer[4] = { 0 };
	};

	// �X���C�h�ړ��A�j���[�V����
	class GameUISlidingAnimation : public GameUIAnimation {
	public:
		GameUISlidingAnimation();
		~GameUISlidingAnimation();
	public:
		void Step() override;
		void SetSpeed(float speed) { speed_ = speed; }
		void SetMove(const dVector& move) { move_ = move; }
		void SetStartPos(const dVector& pos) { start_pos_ = pos; }
	private:
		dVector start_pos_;
		dVector move_;
		float rad_;
		float speed_;
	};

	// UI�A�j���[�V�����Ǘ�
	class GameUIAnimationManager {
	public:
		static void Init();
		static void Step();
		static void DeleteAnimation();
	public:
		// �e�A�j���[�V�����Đ�
		static void PlayBlinkAnimation(const Pointer<GameUI>& ui, float speed);
		static void PlayFloatingAnimation(const Pointer<GameUI>& ui, float speed, float floating_size);
		static void PlaySlidingAnimation(const Pointer<GameUI>& ui, const dVector& start_pos, const dVector& end_pos, float speed);
	private:
		static std::vector<Pointer<GameUIAnimation>> animations_;
	};
}
namespace Size {
	constexpr size_t GAME_UI_ANIMATION = sizeof(MyLib::GameUIAnimation);
	constexpr size_t GAME_UI_BLINK_ANIMATION = sizeof(MyLib::GameUIBlinkAnimation);
	constexpr size_t GAME_UI_FLOATING_ANIMATION = sizeof(MyLib::GameUIFloatingAnimation);
	constexpr size_t GAME_UI_SLIDING_ANIMATION = sizeof(MyLib::GameUISlidingAnimation);
	constexpr size_t GAME_UI_ANIMATION_MANAGER = sizeof(MyLib::GameUIAnimationManager);
}