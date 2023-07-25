#pragma once
#include <Windows.h>
#include <functional>
#include "../../Literal.h"

namespace MyLib
{
	//戻り値void型関数型
	typedef std::function<void()> VoidFunc;

	//条件が満たされるまで待機する
	_inline void WaitConditionSatisfy(std::function<bool()> func) { while (func()) { Sleep(1); } }
	//条件が満たされるまで待機する
	_inline void WaitConditionSatisfy(const bool* const b) { while (*b) { Sleep(1); } }

	class Thread
	{
	public:
		Thread();
		~Thread();
	public:
		//スレッドの開始
		void Begin(const VoidFunc& func, unsigned int stack_size = 1_MB);
		//スレッドの開始
		void Begin(VoidFunc&& func, unsigned int stack_size = 1_MB);
		//スレッドの終了を待つ
		void join(DWORD wait_time = INFINITE);
		bool isActive() { return WaitForSingleObject(handle, 0) == WAIT_TIMEOUT; }
		//優先するプロセッサを指定
		void SetIdealProcessor(DWORD dwIdealProcessor) { SetThreadIdealProcessor(handle, dwIdealProcessor); }
		//スレッド名を設定
		void SetDescription(const char* description);
		//スレッドのハンドルを設定
		HANDLE GetHandle() { return handle; }
		void Suspend() { SuspendThread(handle); }
		void Resume() { ResumeThread(handle); }
		BOOL Close() { return CloseHandle(handle); }
		void End();
	private:
		HANDLE handle;
		unsigned int dwThread;
		//関数ポインタをスレッドに渡すための変数
		VoidFunc func_;
	};
}

namespace Size {
	constexpr size_t THREAD_SIZE = sizeof(MyLib::Thread);
}