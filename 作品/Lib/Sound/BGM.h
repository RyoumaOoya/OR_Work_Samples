#pragma once
#include "SoundBase.h"

namespace MyLib
{
	//BGM—p
	class BGM : public SoundBase
	{
		friend class SoundManager;
		friend class Allocator;
	protected:
		BGM();
		BGM(int id);
		~BGM()override;
	public:
		Pointer<SoundBase> Clone()override { return Pointer<SoundBase>(); };

		void Load()override;
	private:
	};
}