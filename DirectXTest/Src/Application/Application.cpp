#include "Application.h"
#include "../Dx12Wrapper/Dx12Wrapper.h"
#include "../PMDRenderer/PMDRenderer.h"
#include "../PMDActor/PMDActor.h"

namespace {
	constexpr size_t WINDOW_WIDTH = 1080;
	constexpr size_t WINDOW_HEIGHT = 720;

	//面倒だけど書かなあかんやつ
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg == WM_DESTROY) {//ウィンドウが破棄されたら呼ばれます
			PostQuitMessage(0);//OSに対して「もうこのアプリは終わるんや」と伝える
			return 0;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);//規定の処理を行う
	}
}

Application::Application()
{
	memset(&windowClass_, 0, sizeof(windowClass_));
}

Application::~Application()
{
	Terminate();
}

bool Application::Init()
{
	return Init("Model/巡音ルカ.pmd");
}

bool Application::Init(const char* path)
{
	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
	CreateGameWindow(hwnd_, windowClass_);

	//DirectX12ラッパー生成＆初期化
	dx12_.reset(new Dx12Wrapper(hwnd_));
	pmdRenderer_.reset(new PMDRenderer(*dx12_));
	pmdActor_.reset(new PMDActor(path, *pmdRenderer_));

	return true;
}

void Application::ShowWindow()
{
	::ShowWindow(hwnd_, SW_SHOW);//ウィンドウ表示
}

UINT Application::ProcessMessage()
{
	MSG msg = {};
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//全体の描画準備
	dx12_->BeginDraw();
	return msg.message;
}

void Application::SetPMDPipeline()
{
	//PMD用の描画パイプラインに合わせる
	dx12_->CommandList()->SetPipelineState(pmdRenderer_->GetPipelineState());
	//ルートシグネチャもPMD用に合わせる
	dx12_->CommandList()->SetGraphicsRootSignature(pmdRenderer_->GetRootSignature());
	dx12_->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dx12_->SetScene();
}

void Application::ScreenFlip()
{
	dx12_->EndDraw();
	//フリップ
	dx12_->Swapchain()->Present(1, 0);
}

void Application::Terminate()
{
	//もうクラス使わんから登録解除してや
	UnregisterClass(windowClass_.lpszClassName, windowClass_.hInstance);
}

SIZE Application::GetWindowSize()
{
	return SIZE(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Application::CreateGameWindow(HWND& hwnd, WNDCLASSEX& windowClass)
{
	HINSTANCE hInst = GetModuleHandle(nullptr);
	//ウィンドウクラス生成＆登録
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = (WNDPROC)WindowProcedure;//コールバック関数の指定
	windowClass.lpszClassName = _T("DirectXTest");//アプリケーションクラス名(適当でいいです)
	windowClass.hInstance = GetModuleHandle(0);//ハンドルの取得
	RegisterClassEx(&windowClass);//アプリケーションクラス(こういうの作るからよろしくってOSに予告する)

	RECT wrc = { 0,0, WINDOW_WIDTH, WINDOW_HEIGHT };//ウィンドウサイズを決める
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//ウィンドウのサイズはちょっと面倒なので関数を使って補正する
	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(windowClass.lpszClassName, _T("DX12リファクタリング"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, wrc.right - wrc.left, wrc.bottom - wrc.top, nullptr, nullptr, windowClass.hInstance, nullptr);
}
