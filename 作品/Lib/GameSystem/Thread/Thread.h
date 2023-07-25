#pragma once
#include <Windows.h>
#include <functional>
#include "../../Literal.h"

namespace MyLib
{
	//�߂�lvoid�^�֐��^
	typedef std::function<void()> VoidFunc;

	//���������������܂őҋ@����
	_inline void WaitConditionSatisfy(std::function<bool()> func) { while (func()) { Sleep(1); } }
	//���������������܂őҋ@����
	_inline void WaitConditionSatisfy(const bool* const b) { while (*b) { Sleep(1); } }

	class Thread
	{
	public:
		Thread();
		~Thread();
	public:
		//�X���b�h�̊J�n
		void Begin(const VoidFunc& func, unsigned int stack_size = 1_MB);
		//�X���b�h�̊J�n
		void Begin(VoidFunc&& func, unsigned int stack_size = 1_MB);
		//�X���b�h�̏I����҂�
		void join(DWORD wait_time = INFINITE);
		bool isActive() { return WaitForSingleObject(handle, 0) == WAIT_TIMEOUT; }
		//�D�悷��v���Z�b�T���w��
		void SetIdealProcessor(DWORD dwIdealProcessor) { SetThreadIdealProcessor(handle, dwIdealProcessor); }
		//�X���b�h����ݒ�
		void SetDescription(const char* description);
		//�X���b�h�̃n���h����ݒ�
		HANDLE GetHandle() { return handle; }
		void Suspend() { SuspendThread(handle); }
		void Resume() { ResumeThread(handle); }
		BOOL Close() { return CloseHandle(handle); }
		void End();
	private:
		HANDLE handle;
		unsigned int dwThread;
		//�֐��|�C���^���X���b�h�ɓn�����߂̕ϐ�
		VoidFunc func_;
	};
}

namespace Size {
	constexpr size_t THREAD_SIZE = sizeof(MyLib::Thread);
}