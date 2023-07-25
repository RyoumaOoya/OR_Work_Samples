#pragma once
#include "../../typedef.h"
#include "../Thread/ThreadManager.h"
//フレームレート
namespace MyLib
{
	class FrameRate
	{
	public:
		FrameRate() = delete;
		~FrameRate() = delete;
	public:
		static constexpr u64 FRAME_TIME = ConvSecondsToNanoSecond(1) / 60;
		static constexpr u64 DRAW_FRAME_TIME = ConvSecondsToNanoSecond(1) / 60;
	public:
		//初期化
		static void Init();
		//フレームレート分時間が経過したか
		static bool isProgress();
		//フレームレートに併せて実行される処理
		static void Step();
		//描画処理
		static void Draw();

		static void Fin();

		//前回のフレームから経過した秒数を取得
		static double GetDelteTime();
		//前回のフレームから経過した秒数を取得
		static float GetDelteTimef();

		static void UpdateCurrentTime();

		//垂直同期を使用しているか
		static bool GetVSyncFlag() { return isVSync; }
		//垂直同期を設定
		static void SetVSync(bool vSync) { isVSync = vSync; mWaitDraw = (isVSync) ? &EndWait : &NotVSyncWaitDraw; SetWaitVSyncFlag(isVSync); }
	private:
		static void NotVSyncWaitDraw();
		static void EndWait();
	private:
		static inline void(*mWaitDraw)() = nullptr;
		static inline Thread UpdateCurrentTimeThread;
		static inline std::atomic<u64> CurrentTime = 0;	//現在の時間
		static inline u64 ExecutionStartTime = 0;		//実行開始時間
		static inline u64 LastTime = 0;					//前回のフレーム実行時の時間
		static inline u64 LastDrawTime = 0;				//前回のフレーム実行時の時間
		static inline u64 UpDateFrameRateTime = 0;		//フレームレートを更新した時間
		static inline double DelteTime = 0.0;			//前回のフレーム実行時から何秒経過したか
		static inline float CalcFrameRate = 0.f;		//フレームレート（表示用）
		static inline float DrawFrameRate = 0.f;		//フレームレート（表示用）
		static inline u16 DrawCnt = 0;					//描画回数
		static inline u16 FrameCnt = 0;					//フレームカウント用
		static inline bool FinFlg = false;				//終了Flag
		static inline bool isVSync = true;				//垂直同期フラグ

	public:
		static inline void(* const& WaitDraw)() = mWaitDraw;
	};
}