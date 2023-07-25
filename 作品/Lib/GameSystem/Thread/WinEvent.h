#pragma once
#include <Windows.h>
namespace MyLib
{
	//Windowsのイベントをラップするクラス
	class WinEvent
	{
	public:
		WinEvent();
		~WinEvent();

		//イベントをTrueに
		void SetEvent();
		//イベントをFalseに
		void ResetEvent();
		/*
			Trueになるまで待機
			引数: wait_time 待ち時間の最大値
		*/
		void Wait(DWORD wait_time = INFINITE);
	private:
		HANDLE handle;
	};
}