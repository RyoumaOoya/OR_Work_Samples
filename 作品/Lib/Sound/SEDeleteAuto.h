#pragma once
#include "SoundEffect.h"

namespace MyLib
{
	//再生終了後に自動的に解放されるSE
	class SEDeleteAuto : public SoundEffect
	{
		friend class SoundManager;
		friend class Allocator;
	protected:
		SEDeleteAuto() = default;
		SEDeleteAuto(const SoundEffect& se);
		SEDeleteAuto(const SoundEffect& se, const Vector& pos);
		~SEDeleteAuto()override = default;

		void UpDate()override;
	public:
		void SetPos(const Vector& pos) { Pos_ = pos; }
	};
}