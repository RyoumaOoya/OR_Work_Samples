#pragma once
#include <Windows.h>
namespace MyLib
{
	//Windows�̃C�x���g�����b�v����N���X
	class WinEvent
	{
	public:
		WinEvent();
		~WinEvent();

		//�C�x���g��True��
		void SetEvent();
		//�C�x���g��False��
		void ResetEvent();
		/*
			True�ɂȂ�܂őҋ@
			����: wait_time �҂����Ԃ̍ő�l
		*/
		void Wait(DWORD wait_time = INFINITE);
	private:
		HANDLE handle;
	};
}