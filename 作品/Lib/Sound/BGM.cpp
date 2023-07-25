#include "BGM.h"
#include "SoundParameter.h"

namespace MyLib
{
	BGM::BGM()
	{
	}

	BGM::BGM(int id)
	{
		ID_ = id;
	}

	BGM::~BGM()
	{
	}

	void BGM::Load()
	{
		SetCreate3DSoundFlag(FALSE);
		__super::Load(BGM_PARANETER[ID_].Path.data());
		WaitLoad();
		SetVolume(BGM_PARANETER[ID_].Volume);
		SetCreate3DSoundFlag(TRUE);
	}
}