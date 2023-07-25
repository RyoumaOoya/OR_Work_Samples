#pragma once
#include "../../Math/Interpolation.h"
namespace MyLib
{
	enum class FADE_STATUS :int {
		In,
		Out,
		End,
	};
	//フェードイン・アウト描画クラス
	class Fade {
	public:
		Fade() = delete;
		~Fade() = delete;
	public:
		static void Init();
		static void Step();
		static void Draw();
		static void Fin();

		/*
			フェードイン処理の開始
			引数: time 時間(秒), kind 補完の種類
		*/
		static void FadeIn(float time = 1.2, Interpolation::KIND kind = Interpolation::KIND::EASE_IN);
		/*
			フェードアウト処理の開始
			引数: time 時間(秒), kind 補完の種類
		*/
		static void FadeOut(float time = 1.2, Interpolation::KIND kind = Interpolation::KIND::EASE_OUT);
		//フェードが終わっているか
		static bool IsFadeEnd() { return Status == FADE_STATUS::End; }

	private:
		static inline float Param = 0.f;
		static inline FADE_STATUS Status = FADE_STATUS::End;
		static inline float Time = 0.f;
		static inline Interpolation Interpolation;
	};
}