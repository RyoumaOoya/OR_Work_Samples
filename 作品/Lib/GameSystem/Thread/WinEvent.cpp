#include "WinEvent.h"
namespace MyLib
{
	WinEvent::WinEvent()
	{
		handle = CreateEvent(nullptr, false, false, nullptr);
	}

	WinEvent::~WinEvent()
	{
		CloseHandle(handle);
	}

	void WinEvent::SetEvent()
	{
		::SetEvent(handle);
	}

	void WinEvent::ResetEvent()
	{
		::ResetEvent(handle);
	}

	void WinEvent::Wait(DWORD wait_time)
	{
		WaitForSingleObject(handle, wait_time);
	}
}