#pragma once
#include "../../typedef.h"
#include "../Thread/ThreadManager.h"
//�t���[�����[�g
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
		//������
		static void Init();
		//�t���[�����[�g�����Ԃ��o�߂�����
		static bool isProgress();
		//�t���[�����[�g�ɕ����Ď��s����鏈��
		static void Step();
		//�`�揈��
		static void Draw();

		static void Fin();

		//�O��̃t���[������o�߂����b�����擾
		static double GetDelteTime();
		//�O��̃t���[������o�߂����b�����擾
		static float GetDelteTimef();

		static void UpdateCurrentTime();

		//�����������g�p���Ă��邩
		static bool GetVSyncFlag() { return isVSync; }
		//����������ݒ�
		static void SetVSync(bool vSync) { isVSync = vSync; mWaitDraw = (isVSync) ? &EndWait : &NotVSyncWaitDraw; SetWaitVSyncFlag(isVSync); }
	private:
		static void NotVSyncWaitDraw();
		static void EndWait();
	private:
		static inline void(*mWaitDraw)() = nullptr;
		static inline Thread UpdateCurrentTimeThread;
		static inline std::atomic<u64> CurrentTime = 0;	//���݂̎���
		static inline u64 ExecutionStartTime = 0;		//���s�J�n����
		static inline u64 LastTime = 0;					//�O��̃t���[�����s���̎���
		static inline u64 LastDrawTime = 0;				//�O��̃t���[�����s���̎���
		static inline u64 UpDateFrameRateTime = 0;		//�t���[�����[�g���X�V��������
		static inline double DelteTime = 0.0;			//�O��̃t���[�����s�����牽�b�o�߂�����
		static inline float CalcFrameRate = 0.f;		//�t���[�����[�g�i�\���p�j
		static inline float DrawFrameRate = 0.f;		//�t���[�����[�g�i�\���p�j
		static inline u16 DrawCnt = 0;					//�`���
		static inline u16 FrameCnt = 0;					//�t���[���J�E���g�p
		static inline bool FinFlg = false;				//�I��Flag
		static inline bool isVSync = true;				//���������t���O

	public:
		static inline void(* const& WaitDraw)() = mWaitDraw;
	};
}