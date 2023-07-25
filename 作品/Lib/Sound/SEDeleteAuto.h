#pragma once
#include "SoundEffect.h"

namespace MyLib
{
	//Ä¶I—¹Œã‚É©“®“I‚É‰ğ•ú‚³‚ê‚éSE
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