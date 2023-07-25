#pragma once
#include "../Math/MyMath.h"
namespace MyLib
{
	class Interpolation
	{
	public:
		enum class KIND
		{
			NONE = -1,

			LERP,			// 線形補間
			EASE_IN,		// イーズイン(二次関数補間)
			EASE_OUT,		// イーズアウト(二次関数補間)
			EASE_IN_OUT,	// イーズインアウト(三次関数補間)

			NUM,
		};
	public:
		Interpolation() :Kind_(KIND::NONE), TotalTime_(1.f) {}
		Interpolation(KIND kind, float totaltime) :Kind_(kind), TotalTime_(totaltime) {}
		~Interpolation() {}
		/*
			補間処理
			引数: inT 現在時間, start inT / TotalTime_ == 0の時の数値, end inT / TotalTime_ == 1の時の処理
			戻り値: start～endの間
		*/
		template<class T>
		T Interpolate(float inT, const T& start, const T& end) const {
			float t = inT / TotalTime_;

			//tの範囲は通常「0～1」の間になるため、範囲外になった場合は制限する
			if (t < 0.0f) t = 0.0f;
			else if (t > 1.0f) t = 1.0f;

			float v = Interpolate(t);
			T calc1 = start * (1.f - v);
			T calc2 = end * v;

			return calc1 + calc2;
		}

		void SetTotalTime(float t) { TotalTime_ = t; }
		void SetKind(KIND k) { Kind_ = k; }
	private:
		float Interpolate(float t) const { return ((Kind_ <= KIND::NONE || Kind_ >= KIND::NUM)) ? 0.f : (*InterpolateFunc[static_cast<int>(Kind_)])(t); }
	private:
		static constexpr float (*InterpolateFunc[(int)KIND::NUM])(float) = {
			[](float t) { return t; },										//線形補間
			[](float t) { return MyMath::Squared(t); },						//イーズイン(徐々に加速)
			[](float t) { return t * (2.f - t); },							//イーズアウト(徐々に減速)
			[](float t) { return MyMath::Squared(t) * (3.f - 2.f * t); }	//イーズイン・イーズアウト(徐々に加速してから減速)
		};
	private:
		KIND Kind_;
		float TotalTime_;
	};
}
