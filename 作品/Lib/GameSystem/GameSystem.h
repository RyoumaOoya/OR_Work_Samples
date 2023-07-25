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

	//モニターサイズを取得する
	static iVector2D GetMonitorSize() { return iVector2D(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)); }
	//ウィンドウサイズを取得する
	static iVector2D GetWindowSize() { return WindowSize; }
	//ウィンドウサイズを設定する
	static void SetWindowSize(iVector2D size) { WindowSize = size; SetGraphMode(); }
	static void SetGraphMode() { ::SetGraphMode(WindowSize.x, WindowSize.y, 32, 144); }

	static bool OnInitStart();

	//システム情報を取得
	static SYSTEM_INFO GetSystemInfo() {
		SYSTEM_INFO SystemInfo = { 0 };
		::GetSystemInfo(&SystemInfo);
		return SystemInfo;
	}

	static void StepFunc();
	static void DrawFunc();
	//動作が不安定になるためいったん使わないようにする
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

