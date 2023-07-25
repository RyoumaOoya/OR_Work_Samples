#include "thread.h"
#include <process.h>
#include "DxLib.h"

namespace MyLib
{
	Thread::Thread()
	{
		handle = nullptr;
		dwThread = 0;
	}

	Thread::~Thread()
	{
	}

	void Thread::Begin(const VoidFunc& func, unsigned int stack_size)
	{
		func_ = func;
		handle = (HANDLE)_beginthreadex(NULL, stack_size, [](PVOID func) { auto f = *reinterpret_cast<VoidFunc*>(func); f(); return 0U; }, &func_, 0, &dwThread);
	}

	void Thread::Begin(VoidFunc&& func, unsigned int stack_size)
	{
		func_ = func;
		handle = (HANDLE)_beginthreadex(NULL, stack_size, [](PVOID func) { auto f = *reinterpret_cast<VoidFunc*>(func); f(); return 0U; }, &func_, 0, &dwThread);
	}

	void Thread::join(DWORD wait_time)
	{
		WaitForSingleObject(handle, wait_time);
	}

	void Thread::SetDescription(const char* description)
	{
		WCHAR str[128] = { 0 };
		ConvertStringCharCodeFormat(DX_CHARCODEFORMAT_SHIFTJIS, description, DX_CHARCODEFORMAT_UTF16LE, str);
		str[127] = 0;
		SetThreadDescription(handle, str);
	}

	void Thread::End()
	{
		CloseHandle(handle);
	}
}