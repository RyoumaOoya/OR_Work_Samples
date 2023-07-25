#pragma once
#include "WinEvent.h"
#include <atomic>
namespace MyLib
{
	class Look {
	public:
		Look() { readHandle.SetEvent(); writeHandle.SetEvent(); referenceCounter = 0; }
		~Look() {}

		void ReadLock() { readHandle.Wait(); if (referenceCounter == 0) writeHandle.ResetEvent(); std::atomic_thread_fence(std::memory_order_seq_cst); ++referenceCounter; }
		void ReadFree() { --referenceCounter; std::atomic_thread_fence(std::memory_order_seq_cst);  if (referenceCounter == 0) writeHandle.SetEvent(); }
		void WriteLock() { while (referenceCounter != 0) Sleep(1); readHandle.ResetEvent(); }
		void WriteFree() { readHandle.SetEvent(); }

	private:
		std::atomic<size_t> referenceCounter;
		WinEvent readHandle, writeHandle;
	};

	class ReadLock {
	public:
		ReadLock(const Look* Look) :look(const_cast<MyLib::Look*>(Look)) { look->ReadLock(); }
		~ReadLock() { look->ReadFree(); }
	private:
		Look* look;
	};

	class WriteLock {
	public:
		WriteLock(const Look* Look) :look(const_cast<MyLib::Look*>(Look)) { look->WriteLock(); }
		~WriteLock() { look->WriteFree(); }
	private:
		Look* look;
	};
}
namespace Size {
	constexpr auto LOOK_SIZE = sizeof(MyLib::Look);
}