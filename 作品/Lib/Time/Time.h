#pragma once
#include "DxLib.h"
#include "../typedef.h"

namespace MyLib
{
	//時間関係(時間を計るなど)
	class CTime
	{
	public:
		CTime() = delete;
		~CTime() = delete;
	public:
		static inline void Step() {
			CurrentTime = ConvSysPerformanceCountToMicroSeconds(GetNowSysPerformanceCount());
			SecCurrent = CurrentTime * 0.000001;
		}
		//マイクロ秒のカウンタを取得
		static inline u64 StartCountM() { return CurrentTime; }
		//カウンタを取得(double)
		static inline double StartCountSec() { return SecCurrent; }
		//カウンタを取得(float)
		static inline float StartCountSecf() { return static_cast<float>(SecCurrent); }

		//カウンタから時間を取得(マイクロ秒)
		static inline u64 GetCountM(u64 count) { return CurrentTime - count; }
		//カウンタから時間を取得(double)
		static inline double GetCountSec(double sec) { return SecCurrent - sec; }
		//カウンタから時間を取得(float)
		static inline float GetCountSecf(float sec) { return static_cast<float>(SecCurrent) - sec; }
	private:
		static inline u64 CurrentTime = 0;
		static inline double SecCurrent = 0.0;
	};
}