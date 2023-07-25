#pragma once
#include "Allocator/Allocator.h"
#include "../Math/MyMathData.h"
#include "Thread/ThreadManager.h"
#include "../Define.h"
using namespace MyLib;

class GameSystem
{
	GameSystem() = delete;
public:
	static bool Init();
	static void Fin();

	//���j�^�[�T�C�Y���擾����
	static iVector2D GetMonitorSize() { return iVector2D(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)); }
	//�E�B���h�E�T�C�Y���擾����
	static iVector2D GetWindowSize() { return WindowSize; }
	//�E�B���h�E�T�C�Y��ݒ肷��
	static void SetWindowSize(iVector2D size) { WindowSize = size; SetGraphMode(); }
	static void SetGraphMode() { ::SetGraphMode(WindowSize.x, WindowSize.y, 32, 144); }

	static bool OnInitStart();

	//�V�X�e�������擾
	static SYSTEM_INFO GetSystemInfo() {
		SYSTEM_INFO SystemInfo = { 0 };
		::GetSystemInfo(&SystemInfo);
		return SystemInfo;
	}

	static void StepFunc();
	static void DrawFunc();
	//���삪�s����ɂȂ邽�߂�������g��Ȃ��悤�ɂ���
	static void StartStepThread() {
		StepThread.Begin([]() { StepFunc(); });
		StepThread.SetDescription("StepThread");
	}
private:
	static inline HANDLE MainThreadHandle = nullptr;
	static inline iVector2D WindowSize;
	static inline Thread StepThread;
	static inline bool GameEnd = false;
};

