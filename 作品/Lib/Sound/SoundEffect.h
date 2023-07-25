#pragma once
#include "SoundBase.h"
#include "../Math/MyMathData.h"
#include <functional>

namespace MyLib
{
	//サウンドエフェクト
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

		//座標を設定する関数を設定
		void SetPosCalc(const std::function<Vector()>& func) { PosCalc_ = func; }
		//座標を設定する関数を設定
		void SetPosCalc(std::function<Vector()>&& func) { PosCalc_ = func; }

		//音のなる範囲を設定
		void SetRadius(float radius) { Radius_ = radius; Set3DRadiusSoundMem(Radius_, Handle_); }
	protected:
		Vector Pos_;
		float Radius_;
		std::function<Vector()> PosCalc_;
	};
}