#include "Application.h"
#include "../Dx12Wrapper/Dx12Wrapper.h"
#include "../PMDRenderer/PMDRenderer.h"
#include "../PMDActor/PMDActor.h"

namespace {
	constexpr size_t WINDOW_WIDTH = 1080;
	constexpr size_t WINDOW_HEIGHT = 720;

	//�ʓ|�����Ǐ����Ȃ�������
	LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg == WM_DESTROY) {//�E�B���h�E���j�����ꂽ��Ă΂�܂�
			PostQuitMessage(0);//OS�ɑ΂��āu�������̃A�v���͏I�����v�Ɠ`����
			return 0;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);//�K��̏������s��
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
	return Init("Model/�������J.pmd");
}

bool Application::Init(const char* path)
{
	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
	CreateGameWindow(hwnd_, windowClass_);

	//DirectX12���b�p�[������������
	dx12_.reset(new Dx12Wrapper(hwnd_));
	pmdRenderer_.reset(new PMDRenderer(*dx12_));
	pmdActor_.reset(new PMDActor(path, *pmdRenderer_));

	return true;
}

void Application::ShowWindow()
{
	::ShowWindow(hwnd_, SW_SHOW);//�E�B���h�E�\��
}

UINT Application::ProcessMessage()
{
	MSG msg = {};
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//�S�̂̕`�揀��
	dx12_->BeginDraw();
	return msg.message;
}

void Application::SetPMDPipeline()
{
	//PMD�p�̕`��p�C�v���C���ɍ��킹��
	dx12_->CommandList()->SetPipelineState(pmdRenderer_->GetPipelineState());
	//���[�g�V�O�l�`����PMD�p�ɍ��킹��
	dx12_->CommandList()->SetGraphicsRootSignature(pmdRenderer_->GetRootSignature());
	dx12_->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dx12_->SetScene();
}

void Application::ScreenFlip()
{
	dx12_->EndDraw();
	//�t���b�v
	dx12_->Swapchain()->Present(1, 0);
}

void Application::Terminate()
{
	//�����N���X�g��񂩂�o�^�������Ă�
	UnregisterClass(windowClass_.lpszClassName, windowClass_.hInstance);
}

SIZE Application::GetWindowSize()
{
	return SIZE(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Application::CreateGameWindow(HWND& hwnd, WNDCLASSEX& windowClass)
{
	HINSTANCE hInst = GetModuleHandle(nullptr);
	//�E�B���h�E�N���X�������o�^
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = (WNDPROC)WindowProcedure;//�R�[���o�b�N�֐��̎w��
	windowClass.lpszClassName = _T("DirectXTest");//�A�v���P�[�V�����N���X��(�K���ł����ł�)
	windowClass.hInstance = GetModuleHandle(0);//�n���h���̎擾
	RegisterClassEx(&windowClass);//�A�v���P�[�V�����N���X(���������̍�邩���낵������OS�ɗ\������)

	RECT wrc = { 0,0, WINDOW_WIDTH, WINDOW_HEIGHT };//�E�B���h�E�T�C�Y�����߂�
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//�E�B���h�E�̃T�C�Y�͂�����Ɩʓ|�Ȃ̂Ŋ֐����g���ĕ␳����
	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(windowClass.lpszClassName, _T("DX12���t�@�N�^�����O"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, wrc.right - wrc.left, wrc.bottom - wrc.top, nullptr, nullptr, windowClass.hInstance, nullptr);
}
