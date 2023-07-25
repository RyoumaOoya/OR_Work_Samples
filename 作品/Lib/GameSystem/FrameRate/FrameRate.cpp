#include "FrameRate.h"
#include "DxLib.h"
#include "../Common/Common.h"
#include "../../GameSystem/GameSystem.h"
#include "../../Constant/Constant.h"
#include "../../Literal.h"

namespace MyLib
{
	void FrameRate::Init()
	{
		FinFlg = false;
		CurrentTime = 0;
		UpdateCurrentTimeThread.Begin([]() { while (!FinFlg) { UpdateCurrentTime(); }});
		LastTime = 0;
		LastDrawTime = 0;
		DelteTime = 0.f;
		FrameCnt = 0;
		DrawCnt = 0;
		UpDateFrameRateTime = 0;
		CalcFrameRate = 0;
		DrawFrameRate = 0;
		FinFlg = false;
		SetVSync(isVSync);
	}

	bool FrameRate::isProgress()
	{
		//現在の時刻を取得
		ExecutionStartTime = CurrentTime;
		return (CurrentTime - LastTime >= FRAME_TIME);
	}

	void FrameRate::NotVSyncWaitDraw()
	{
		while (CurrentTime - LastDrawTime <= DRAW_FRAME_TIME)
		{
			Sleep(1);
		}
		EndWait();
	}

	void FrameRate::EndWait()
	{
		LastDrawTime = CurrentTime;
		++DrawCnt;
	}

	void FrameRate::Step()
	{
		//前回のフレームレート更新からの経過時間を求める
		u64 nDifTime = ExecutionStartTime - UpDateFrameRateTime;
		//デルタフレーム更新
		DelteTime = ConvNanosecondToSeconds(CurrentTime - LastTime);

		//フレーム数をカウント
		FrameCnt++;

		//フレーム実行時の時間を更新
		LastTime = ExecutionStartTime;

		//前回のフレームレート更新から
		//１秒以上経過していたらフレームレートを更新する
		if (nDifTime > ConvSecondsToNanoSecond(1))
		{
			//フレームレートを求める
			//理想どおりなら 60000 / 1000 で 60 となる
			CalcFrameRate = static_cast<float>(static_cast<double>(FrameCnt) * ConvSecondsToNanoSecond(1) / nDifTime);
			DrawFrameRate = static_cast<float>(static_cast<double>(DrawCnt) * ConvSecondsToNanoSecond(1) / nDifTime);

			//フレームカウントをクリア
			FrameCnt = 0;
			DrawCnt = 0;

			//フレームレート更新時間を更新
			UpDateFrameRateTime = ExecutionStartTime;
		}
	}

	void FrameRate::Draw()
	{
		typedef Common C;
		const auto WINDOW_SIZE = GameSystem::GetWindowSize();
		DrawFormatString(WINDOW_SIZE.x - 230, WINDOW_SIZE.y - (2_ROW), Constant::Color(255, 30, 30), "FPS[内部:%.2f 表示:%.2f]", CalcFrameRate, DrawFrameRate);
		DrawFormatString(WINDOW_SIZE.x - 153, WINDOW_SIZE.y - (1_ROW), Constant::Color(255, 30, 30), "DelteTime[%.4f]", DelteTime);
	}

	void FrameRate::Fin()
	{
		FinFlg = true;

		UpdateCurrentTimeThread.join();
	}

	double FrameRate::GetDelteTime()
	{
		return DelteTime;
	}

	float FrameRate::GetDelteTimef()
	{
		return static_cast<float>(DelteTime);
	}

	void FrameRate::UpdateCurrentTime()
	{
		CurrentTime = ConvSysPerformanceCountToNanoSeconds(GetNowSysPerformanceCount());
	}
}