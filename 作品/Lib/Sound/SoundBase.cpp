#include "SoundBase.h"
#include "SoundParameter.h"
#include "GameSystem/Thread/ThreadManager.h"

namespace MyLib
{
	SoundBase::SoundBase()
	{
		Handle_ = -1;
		Volume_ = 255;
		ID_ = 0;
		State_ = 0;
	}

	SoundBase::~SoundBase()
	{
		Fin();
	}

	void SoundBase::Init()
	{
	}

	void SoundBase::Load(const char* path)
	{
		Handle_ = LoadSoundMem(path, 1);
	}

	void SoundBase::Load(int handle)
	{
		Handle_ = DuplicateSoundMem(handle, 1);
		WaitLoad();
	}

	void SoundBase::UpDate()
	{
		if (State_ == ISoundState::Fin) {
			Stop();
			return;
		}
		else
			State_ = CheckSoundMem(Handle_);
	}

	void SoundBase::Delete()
	{
		if (Handle_ != -1) {
			Stop();
			DeleteSoundMem(Handle_);
			Handle_ = -1;
		}
	}

	void SoundBase::Fin()
	{
		Delete();
	}

	void SoundBase::Play(int play_type)
	{
		PlaySoundMem(Handle_, play_type);
	}

	void SoundBase::Stop()
	{
		StopSoundMem(Handle_);
	}
}