#include "SEDeleteAuto.h"
#include "SoundParameter.h"

namespace MyLib
{
	SEDeleteAuto::SEDeleteAuto(const SoundEffect& se) :SoundEffect(se)
	{
		PosCalc_ = nullptr;
	}

	SEDeleteAuto::SEDeleteAuto(const SoundEffect& se, const Vector& pos) : SEDeleteAuto(se)
	{
		SetPos(pos);
	}

	void SEDeleteAuto::UpDate()
	{
		__super::UpDate();
		if (State_ != ISoundState::Fin)
			State_ = (CheckSoundMem(Handle_) == 1) ? ISoundState::Play : ISoundState::Fin;
	}
}