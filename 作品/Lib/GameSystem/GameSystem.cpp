#include "GameSystem.h"
#include "Common/Common.h"
#include "Allocator/Allocator.h"
#include "Thread/ThreadManager.h"
#include "Src/Scene/SceneManager.h"
#include "Src/Scene/SceneParameter.h"
#include "FrameRate/FrameRate.h"
#include "../Input/InputManager.h"
#include "../Time/Time.h"
#include "../Message/Message.h"
#include "../Render/Render.h"
#include "../Render/Fade/Fade.h"

bool GameSystem::Init()
{
	if (!OnInitStart()) {
		return false;
	}

	//DX���C�u�����̏�����
	if (DxLib_Init() == -1) {
		return false;
	}

	SetTransColor(0, 0, 0);

	SetDrawScreen(DX_SCREEN_BACK);	//�`�悷��X�N���[����ݒ肷��

	//�񓯊��ǂݍ��ݐݒ�
	SetUseASyncLoadFlag(TRUE);
	SetASyncLoadThreadNum(4);

	Allocator::Init();
	ThreadManager::Init();
	Render::Init();

	auto scene_manager = SceneManager::CreateInstance();
	scene_manager->Init();
	scene_manager->StartFirstScene(ISCENE_ID::TATLE);

	FrameRate::Init();

	Common::InitRandom();

	//�C���v�b�g������
	CInputManager::Init();
	//�C���v�b�g�ݒ�		KeyBoard	Mouse	JoyPad
	CInputManager::SetInput(true, true, true);

	CInputManager::GetMouse()->DrawMousePointer(true);

	//�y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	//�y�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(TRUE);

	Allocator::Load();

	return true;
}

//void GameStstem::Loop()
//{
//	auto scene_manager = SceneManager::CreateInstance();
//	while (ProcessMessage() != -1 && !isGameEnd)
//	{
//		if (CFrameRate::isProgress())
//		{
//			CFrameRate::Step();
//			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
//			{
//				//�G�X�P�[�v�L�[�������ꂽ��I��
//				isGameEnd = true;
//			}
//
//			//�C���v�b�g�X�V
//			CInputManager::Step();
//
//			CTime::Step();
//			Message::Step();
//			ClearDrawScreen();
//			scene_manager->Loop();
//			scene_manager->Draw();
//
//			//CFrameRate::Draw();
//			////�t���b�v�֐��i���[�v�̏I���ɂP�x�����s���j
//			//ScreenFlip();
//		}
//	}
//}

void GameSystem::Fin()
{
	GameEnd = true;
	StepThread.join();
	StepThread.End();
	SceneManager::GetInstance()->Fin();
	CInputManager::Fin();
	FrameRate::Fin();
	SceneManager::DeleteInstance();
	ThreadManager::Fin();

	Allocator::Clear(AllocType::Manager);

	Allocator::Fin();

	//�g�p�������ׂẲ摜��j��
	InitGraph();

	//DX���C�u�����̌㏈��
	DxLib_End();
}

bool GameSystem::OnInitStart()
{
	MainThreadHandle = GetCurrentThread();
	SetThreadIdealProcessor(MainThreadHandle, 0);

	WindowSize(1280, 720);

	SetDoubleStartValidFlag(TRUE);
	SetAlwaysRunFlag(TRUE);
	//-----------------------------------------
	//��ԍŏ��ɂP�񂾂���鏈���������ɏ���

	switch (MessageBox(NULL, TEXT("�t���X�N���[�����[�h"), TEXT("��ʐݒ�"), MB_YESNOCANCEL))
	{
	case IDYES: {
		SetWindowSize({ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) });
		SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);
	}break;
	case IDCANCEL: {
		return false;
	}break;
	case IDNO:	//NO�I�����̓f�t�H���g������
	default: ChangeWindowMode(true); /*�E�B���h�E�̏�Ԃ�ݒ肷��*/break;
	}

	//�E�B���h�E�T�C�Y��ύX
	SetGraphMode();

	return true;
}

void GameSystem::StepFunc()
{
	auto scene_manager = SceneManager::GetInstance();
	while (ProcessMessage() != -1 && !GameEnd)
	{
		if (FrameRate::isProgress())
		{
			FrameRate::Step();
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			{
				//�G�X�P�[�v�L�[�������ꂽ��I��
				break;
			}

			//�C���v�b�g�X�V
			CInputManager::MainStep();
			CInputManager::Step();

			CTime::Step();
			Message::Step();
			scene_manager->Loop();
		DrawFunc();
		}
	}

	GameEnd = true;
}

void GameSystem::DrawFunc()
{
	auto scene_manager = SceneManager::GetInstance();

	/*while (!isGameEnd)
	{*/
		//CFrameRate::WaitDraw();
		ClearDrawScreen();
		Render::Draw();
		scene_manager->Draw();
		Allocator::Draw();
		Fade::Draw();
		FrameRate::Draw();
		Message::Draw();
		ScreenFlip();
	//}
}
