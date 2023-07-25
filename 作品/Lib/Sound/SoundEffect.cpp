#include "SoundEffect.h"
#include "SoundParameter.h"
#include "GameSystem/Allocator/Allocator.h"
#include "SoundManager.h"
#include "../Math/MyMath.h"
namespace MyLib
{
	SoundEffect::SoundEffect()
	{
		Radius_ = 0.f;
	}

	SoundEffect::SoundEffect(int id)
	{
		Radius_ = 0.f;
		ID_ = id;
	}

	SoundEffect::~SoundEffect()
	{
	}

	Pointer<SoundBase> SoundEffect::Clone()
	{
		auto ptr = MyNew(SoundEffect, AllocType::Sound, Tags(), *this);
		ptr->SoundBase::Load(Handle_);
		ptr->SetVolume(Volume_);
		ptr->SetRadius(Radius_);

		return ptr;
	}

	void SoundEffect::Load()
	{
		SetCreate3DSoundFlag(TRUE);
		__super::Load(SE_PARANETER[ID_].Path.data());
		WaitLoad();
		SetVolume(SE_PARANETER[ID_].Volume);
		SetRadius(SE_PARANETER[ID_].Radius);
	}

	void SoundEffect::UpDate()
	{
		__super::UpDate();
		if (PosCalc_) {
			Pos_ = PosCalc_();
		}

		//パンの設定
		//DxLibの3Dサウンドだけだと左右が分かりづらいためパンを利用している
		{
			constexpr float ROTATION_MAX = 1.57079637f;
			static auto SEPanSetup = [](float y) {
				auto pan = static_cast<int>((y / ROTATION_MAX) * SOUND_PARANETER::SE_RANGE);
				return (MyMath::Abs(pan) >= SOUND_PARANETER::CENTER_OFFSET) ? pan : 0;
			};

			auto sound_manager = SoundManager::GetInstance();
			auto listener_pos = sound_manager->GetListenerPos();
			auto listener_y = listener_pos.GetFollowRotationXY(sound_manager->GetListenerFrontPos()).y;
			auto this_y = listener_pos.GetFollowRotationXY(Pos_).y;
			float y = this_y - listener_y;

			SetPanSoundMem(SEPanSetup(y), Handle_);
		}
		Set3DPositionSoundMem(Pos_, Handle_);
	}
}