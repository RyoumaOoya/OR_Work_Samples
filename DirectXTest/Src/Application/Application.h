#pragma once
#include <Windows.h>
#include <tchar.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <d3dx12.h>
#include <wrl.h>
#include <memory>

class Dx12Wrapper;
class PMDRenderer;
class PMDActor;

class Application
{
private:
	Application();
	~Application();

	Application(const Application&) = delete;
	void operator=(const Application&) = delete;
public:
	static Application* GetInstance() { return (instance) ? instance : instance = new Application; }
	static void DeleteInstance() { if (instance) delete instance; }
public:
	//初期化
	bool Init();
	bool Init(const char* path);

	void ShowWindow();

	UINT ProcessMessage();

	void SetPMDPipeline();

	void ScreenFlip();

	//後処理
	void Terminate();

	SIZE GetWindowSize();

	std::shared_ptr<PMDActor> GetActor() { return pmdActor_; }
private:

	//ゲーム用ウィンドウの生成
	void CreateGameWindow(HWND& hwnd, WNDCLASSEX& windowClass);
private:
	static inline Application* instance = nullptr;

	WNDCLASSEX windowClass_;
	HWND hwnd_;
	std::shared_ptr<Dx12Wrapper> dx12_;
	std::shared_ptr<PMDRenderer> pmdRenderer_;
	std::shared_ptr<PMDActor> pmdActor_;
};

