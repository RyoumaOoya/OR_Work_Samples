#include "GameSystem/GameSystem.h"
/*
	�����_�����O��ʃX���b�h�ōs���Ɗm���i�����炭�������j�ŗ�O���o�邽�߁A
	����̓X�e�b�v���s��Ƀ����_�����O���s�������ɂ��Ă���
*/
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	if (GameSystem::Init()) {
		GameSystem::StepFunc();

		GameSystem::Fin();
	}
	return 0;
}
