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
		//���݂̎������擾
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
		//�O��̃t���[�����[�g�X�V����̌o�ߎ��Ԃ����߂�
		u64 nDifTime = ExecutionStartTime - UpDateFrameRateTime;
		//�f���^�t���[���X�V
		DelteTime = ConvNanosecondToSeconds(CurrentTime - LastTime);

		//�t���[�������J�E���g
		FrameCnt++;

		//�t���[�����s���̎��Ԃ��X�V
		LastTime = ExecutionStartTime;

		//�O��̃t���[�����[�g�X�V����
		//�P�b�ȏ�o�߂��Ă�����t���[�����[�g���X�V����
		if (nDifTime > ConvSecondsToNanoSecond(1))
		{
			//�t���[�����[�g�����߂�
			//���z�ǂ���Ȃ� 60000 / 1000 �� 60 �ƂȂ�
			CalcFrameRate = static_cast<float>(static_cast<double>(FrameCnt) * ConvSecondsToNanoSecond(1) / nDifTime);
			DrawFrameRate = static_cast<float>(static_cast<double>(DrawCnt) * ConvSecondsToNanoSecond(1) / nDifTime);

			//�t���[���J�E���g���N���A
			FrameCnt = 0;
			DrawCnt = 0;

			//�t���[�����[�g�X�V���Ԃ��X�V
			UpDateFrameRateTime = ExecutionStartTime;
		}
	}

	void FrameRate::Draw()
	{
		typedef Common C;
		const auto WINDOW_SIZE = GameSystem::GetWindowSize();
		DrawFormatString(WINDOW_SIZE.x - 230, WINDOW_SIZE.y - (2_ROW), Constant::Color(255, 30, 30), "FPS[����:%.2f �\��:%.2f]", CalcFrameRate, DrawFrameRate);
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