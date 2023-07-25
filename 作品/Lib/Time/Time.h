#pragma once
#include "DxLib.h"
#include "../typedef.h"

namespace MyLib
{
	//���Ԋ֌W(���Ԃ��v��Ȃ�)
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
		//�}�C�N���b�̃J�E���^���擾
		static inline u64 StartCountM() { return CurrentTime; }
		//�J�E���^���擾(double)
		static inline double StartCountSec() { return SecCurrent; }
		//�J�E���^���擾(float)
		static inline float StartCountSecf() { return static_cast<float>(SecCurrent); }

		//�J�E���^���玞�Ԃ��擾(�}�C�N���b)
		static inline u64 GetCountM(u64 count) { return CurrentTime - count; }
		//�J�E���^���玞�Ԃ��擾(double)
		static inline double GetCountSec(double sec) { return SecCurrent - sec; }
		//�J�E���^���玞�Ԃ��擾(float)
		static inline float GetCountSecf(float sec) { return static_cast<float>(SecCurrent) - sec; }
	private:
		static inline u64 CurrentTime = 0;
		static inline double SecCurrent = 0.0;
	};
}