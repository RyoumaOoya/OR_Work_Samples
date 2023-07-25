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

	//DXライブラリの初期化
	if (DxLib_Init() == -1) {
		return false;
	}

	SetTransColor(0, 0, 0);

	SetDrawScreen(DX_SCREEN_BACK);	//描画するスクリーンを設定する

	//非同期読み込み設定
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

	//インプット初期化
	CInputManager::Init();
	//インプット設定		KeyBoard	Mouse	JoyPad
	CInputManager::SetInput(true, true, true);

	CInputManager::GetMouse()->DrawMousePointer(true);

	//Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	//Ｚバッファへの書き込みを有効にする
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
//				//エスケープキーが押されたら終了
//				isGameEnd = true;
//			}
//
//			//インプット更新
//			CInputManager::Step();
//
//			CTime::Step();
//			Message::Step();
//			ClearDrawScreen();
//			scene_manager->Loop();
//			scene_manager->Draw();
//
//			//CFrameRate::Draw();
//			////フリップ関数（ループの終わりに１度だけ行う）
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

	//使用したすべての画像を破棄
	InitGraph();

	//DXライブラリの後処理
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
	//一番最初に１回だけやる処理をここに書く

	switch (MessageBox(NULL, TEXT("フルスクリーンモード"), TEXT("画面設定"), MB_YESNOCANCEL))
	{
	case IDYES: {
		SetWindowSize({ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) });
		SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);
	}break;
	case IDCANCEL: {
		return false;
	}break;
	case IDNO:	//NO選択時はデフォルト処理に
	default: ChangeWindowMode(true); /*ウィンドウの状態を設定する*/break;
	}

	//ウィンドウサイズを変更
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
				//エスケープキーが押されたら終了
				break;
			}

			//インプット更新
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
