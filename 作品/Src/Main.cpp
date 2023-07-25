#include "GameSystem/GameSystem.h"
/*
	レンダリングを別スレッドで行うと確率（おそらく競合問題）で例外が出るため、
	現状はステップ実行後にレンダリングを行う方式にしておく
*/
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	if (GameSystem::Init()) {
		GameSystem::StepFunc();

		GameSystem::Fin();
	}
	return 0;
}
