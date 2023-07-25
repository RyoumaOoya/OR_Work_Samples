#include "SoundManager.h"
#include "SoundEffect.h"
#include "SoundParameter.h"
#include "BGM.h"
#include "SEDeleteAuto.h"
#include "../GameSystem/Thread/ThreadManager.h"
#include "../Math/MyMath.h"
namespace MyLib
{
	SoundManager::SoundManager()
	{
		Index_ = MANAGER_INDEX::Last;
	}

	SoundManager::~SoundManager()
	{
		Fin();
	}

	void SoundManager::Init()
	{
		E_FOR(SE_ID) {
			SE_Prototypes_.push_back(MyNew(SoundEffect, AllocType::Prototype, Tags(), index));
		}
		for (auto se : SE_Prototypes_) {
			se->Init();
		}
	}

	void SoundManager::Load()
	{
		for (auto se : SE_Prototypes_) {
			se->Load();
		}
	}

	void SoundManager::UpDate()
	{
		SoundItr_ = Sounds_.begin();
		static 	auto func = [this]() {
			while (true)
			{
				Pointer<SoundBase> sound;
				bool del_flg = false;
				{
					ScopedCS s_cs(&CS_);
					if (SoundItr_ == Sounds_.end())
						return;

					sound = *SoundItr_;

					if (sound->GetState() == ISoundState::Fin) {
						del_flg = true;
						SoundItr_ = Sounds_.erase(SoundItr_);
					}
					else
						++SoundItr_;
				}
				if (del_flg) {
					MyFree(sound);
					continue;
				}
				sound->UpDate();
			}
		};

		if (ListenerPosAndFrontCalc_) {
			ListenerPosAndFrontCalc_(&ListenerPos_, &ListenerFrontPos_);
			Set3DSoundListenerPosAndFrontPos_UpVecY(ListenerPos_, ListenerFrontPos_);
		}

		ThreadManager::PushFunc(func, MIN(ThreadManager::GetThreadNum(), Sounds_.size()));
		if (BGM_)
			BGM_->UpDate();
	}

	void SoundManager::Delete()
	{
		for (auto itr = Sounds_.begin(); itr != Sounds_.end();) {
			MyFree(*itr);
			itr = Sounds_.erase(itr);
		}
		if (BGM_)
			MyFree(BGM_);
	}

	void SoundManager::Fin()
	{
		Delete();
		for (auto itr = SE_Prototypes_.begin(); itr != SE_Prototypes_.end();) {
			MyFree(*itr);

			itr = SE_Prototypes_.erase(itr);
		}
	}

	Pointer<SoundBase> SoundManager::CreateSEHandle(int id)
	{
		auto ptr = SE_Prototypes_[id]->Clone();
		Sounds_.push_back(ptr);
		return ptr;
	}

	Pointer<SoundBase> SoundManager::PlaySE(int id, Vector pos, u8 volume, float radius)
	{
		auto ptr = MyNew(SEDeleteAuto, AllocType::Sound, Tags(), *static_cast<Pointer<SoundEffect>>(SE_Prototypes_[id]), pos);

		if (volume != 0)
			ptr->SetVolume(volume);

		if (radius != 0.f)
			ptr->SetRadius(radius);

		ptr->Play();

		auto base_ptr = Pointer<SoundBase>(ptr);
		Sounds_.push_back(base_ptr);
		return Pointer<SoundBase>(base_ptr);
	}

	bool SoundManager::isBGMPlay()
	{
		return BGM_->GetState() == ISoundState::Play;
	}

	void SoundManager::BGMPlay()
	{
		BGM_->Play();
	}

	void SoundManager::BGMStop()
	{
		BGM_->Stop();
	}

	void SoundManager::ChangeBGM(int bgm_id, bool play)
	{
		if (BGM_)
			MyFree(BGM_);

		BGM_ = MyNew(BGM, AllocType::Sound, Tags(), bgm_id);
		BGM_->Init();
		BGM_->Load();
		if (play)
			BGMPlay();
	}
}