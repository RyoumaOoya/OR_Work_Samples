#pragma once
#include "SoundBase.h"
#include "../Math/MyMathData.h"
#include <functional>

namespace MyLib
{
	//�T�E���h�G�t�F�N�g
	class SoundEffect :public SoundBase
	{
		friend class SoundManager;
		friend class Allocator;
	protected:
		SoundEffect();
		SoundEffect(int id);
		~SoundEffect()override;

	public:
		Pointer<SoundBase> Clone()override;

		void Load()override;
		virtual void UpDate()override;

		//���W��ݒ肷��֐���ݒ�
		void SetPosCalc(const std::function<Vector()>& func) { PosCalc_ = func; }
		//���W��ݒ肷��֐���ݒ�
		void SetPosCalc(std::function<Vector()>&& func) { PosCalc_ = func; }

		//���̂Ȃ�͈͂�ݒ�
		void SetRadius(float radius) { Radius_ = radius; Set3DRadiusSoundMem(Radius_, Handle_); }
	protected:
		Vector Pos_;
		float Radius_;
		std::function<Vector()> PosCalc_;
	};
}