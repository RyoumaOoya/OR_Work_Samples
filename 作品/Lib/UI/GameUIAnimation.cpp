#include "GameUIAnimation.h"
#include "../Math/MyMath.h"
#include "../Literal.h"
#include "../GameSystem/Allocator/Allocator.h"

namespace MyLib
{
	// UIアニメーション
	GameUIAnimation::GameUIAnimation()
	{
		play_flg_ = false;
	}

	GameUIAnimation::~GameUIAnimation()
	{
	}

	// 明滅アニメーション
	GameUIBlinkAnimation::GameUIBlinkAnimation()
	{
		speed_ = 0.0f;
		rad_ = 0.0f;
	}

	GameUIBlinkAnimation::~GameUIBlinkAnimation()
	{
	}

	void GameUIBlinkAnimation::Step()
	{
		u8 alpha = static_cast<u8>(MyMath::GetSin(0.0f, 255.0f, rad_));
		ui_->SetBlendData(BlendData(BLEND_MODE::ALPHA, alpha));
		rad_ += speed_;
	}

	// 浮遊アニメーション
	GameUIFloatingAnimation::GameUIFloatingAnimation()
	{
		speed_ = 0.0f;
		rad_ = 0.0f;
		center_pos_ = 0.0f;
		min_offset_ = 0.0f;
		max_offset_ = 0.0f;

	}

	GameUIFloatingAnimation::~GameUIFloatingAnimation()
	{
	}

	void GameUIFloatingAnimation::Step()
	{
		float offset = MyMath::GetSin(min_offset_, max_offset_, rad_);
		ui_->SetPos(Vector2D(ui_->GetPos().x, center_pos_ + offset));
		rad_ += speed_;
	}

	void GameUIFloatingAnimation::SetFloating(float center, float size)
	{
		center_pos_ = center;
		min_offset_ = -(size / 2.0f);
		max_offset_ = (size / 2.0f);
	}

	// スライディングアニメーション
	GameUISlidingAnimation::GameUISlidingAnimation()
	{
		start_pos_ = { 0.0f };
		move_ = { 0.0f };
		rad_ = 0.0f;
		speed_ = 0.0f;
	}

	GameUISlidingAnimation::~GameUISlidingAnimation()
	{
	}

	void GameUISlidingAnimation::Step()
	{
		if (rad_ > 2.0_PI_F) {
			rad_ = 0.0_PI_F;
			play_flg_ = false;
		}

		float scale = MyMath::GetSin(rad_);
		dVector offset = move_ * scale;
		dVector pos = start_pos_ + offset;

		ui_->SetPos(static_cast<Vector2D>(pos));

		rad_ += speed_;
	}

	// アニメーション管理
	std::vector<Pointer<GameUIAnimation>> GameUIAnimationManager::animations_;
	void GameUIAnimationManager::Init()
	{
	}

	void GameUIAnimationManager::Step()
	{
		for (auto& animation : animations_) {
			if (animation->IsPlay()) {
				animation->Step();
			}
		}
	}

	void GameUIAnimationManager::DeleteAnimation()
	{
		for (auto& animation : animations_) {
			MyFree(animation);
		}

		animations_.clear();
		animations_.shrink_to_fit();
	}

	// 明滅アニメーション再生
	void GameUIAnimationManager::PlayBlinkAnimation(const Pointer<GameUI>& ui, float speed)
	{
		Pointer<GameUIBlinkAnimation> anim = MyNew(GameUIBlinkAnimation, AllocType::UI, Tags(MemTag::UI));
		anim->SetUI(ui);
		anim->SetSpeed(speed);
		anim->Play();

		animations_.push_back(anim);
	}

	// 浮遊アニメーション再生
	void GameUIAnimationManager::PlayFloatingAnimation(const Pointer<GameUI>& ui, float speed, float floating_size)
	{
		Pointer<GameUIFloatingAnimation> anim = MyNew(GameUIFloatingAnimation, AllocType::UI, Tags(MemTag::UI));
		anim->SetUI(ui);
		anim->SetSpeed(speed);
		anim->SetFloating(ui->GetPos().y, floating_size);
		anim->Play();

		animations_.push_back(anim);
	}

	// スライディングアニメーション再生
	void GameUIAnimationManager::PlaySlidingAnimation(const Pointer<GameUI>& ui, const dVector& start_pos, const dVector& end_pos, float speed)
	{

		Pointer<GameUISlidingAnimation> anim = MyNew(GameUISlidingAnimation, AllocType::UI, Tags(MemTag::UI));
		anim->SetUI(ui);
		anim->SetSpeed(speed);
		dVector move = start_pos.Create(end_pos);
		anim->SetMove(move);
		anim->SetStartPos(start_pos);
		anim->Play();

		animations_.push_back(anim);
	}
}