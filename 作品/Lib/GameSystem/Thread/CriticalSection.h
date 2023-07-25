#pragma once
#include <Windows.h>
#include <memory>
namespace MyLib
{
	class CriticalSection
	{
	public:
		CriticalSection() { InitializeCriticalSection(&cs); }
		~CriticalSection() { DeleteCriticalSection(&cs); }

		void Enter() { atomic_thread_fence(std::memory_order::acquire); EnterCriticalSection(&cs); atomic_thread_fence(std::memory_order::acquire); }
		void Leave() { atomic_thread_fence(std::memory_order::acquire); LeaveCriticalSection(&cs); atomic_thread_fence(std::memory_order::acquire); }

		CRITICAL_SECTION* Get() { return &cs; }
	private:
		CRITICAL_SECTION cs;
		//DWORD th_id;
	};

	

	/*

	*/
	class ScopedCS {
	public:
		ScopedCS(CriticalSection* CS) :cs(CS) { cs->Enter(); }
		~ScopedCS() { cs->Leave(); }
	private:
		CriticalSection* cs;
	};
}
namespace Size {
	constexpr size_t CRITICAL_SECTION = sizeof(MyLib::CriticalSection);
}