#include "Application/Application.h"
#include "PMDActor/PMDActor.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	auto app = Application::GetInstance();
	if (!app->Init("Model/初音ミク.pmd")) {
		return -1;
	}

	app->ShowWindow();
	{
		auto actor = app->GetActor();
		while (app->ProcessMessage() != WM_QUIT)
		{
			app->SetPMDPipeline();

			actor->Update();
			actor->Draw();
			app->ScreenFlip();
		}
	}

	app->DeleteInstance();
	return 0;

}

////ウィンドウ表示＆DirectX初期化
//#include <Windows.h>
//#include <tchar.h>
//#include <d3d12.h>
//#include <dxgi1_6.h>
//#include <vector>
//#include <DirectXMath.h>
//#include <d3dcompiler.h>
//#include "Common.h"
//#include <DirectXTex.h>
//#include <d3dx12.h>
//#include <map>
//#include <wrl.h>
//#ifdef _DEBUG
//#include <iostream>
//#endif
//#pragma comment(lib,"d3d12.lib")
//#pragma comment(lib,"dxgi.lib")
//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib,"DirectXTex.lib")
//
//#define ID_QUIT                         40001
//#define ID_SAVE                         40002
//#define ID_SAVEAS                       40003
//#define ID_SELECTALL                    40004
//
//using namespace DirectX;
//
//using namespace Microsoft::WRL;
//
//typedef std::function<HRESULT(const std::wstring&, TexMetadata*, ScratchImage&)> LoadLambda_t;
//
//class autoClose {
//public:
//	autoClose(FILE* fp) :fp(fp) {}
//	autoClose(_In_z_ char const* _FileName, _In_z_ char const* _Mode) {
//		fp = fopen(_FileName, _Mode);
//	}
//	~autoClose() { fclose(fp); }
//
//	operator bool() const { return fp; }
//	operator FILE* () const { return fp; }
//private:
//	FILE* fp;
//};
//
//struct Vertex {
//	constexpr Vertex() :pos(0.f, 0.f, 0.f), uv(0.f, 0.f) {}
//	constexpr Vertex(float x, float y, float z, float uvx, float uvy) : pos(x, y, z), uv(uvx, uvy) {}
//	constexpr Vertex(XMFLOAT3 p, XMFLOAT2 uv) : pos(p), uv(uv) {}
//
//	XMFLOAT3 pos;	//xyz座標
//	XMFLOAT2 uv;	//uv座標
//};
//
//struct TexRGBA {
//	UCHAR R, G, B, A;
//};
//
////面倒だけど書かなあかんやつ
//LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
//	if (msg == WM_DESTROY) {//ウィンドウが破棄されたら呼ばれます
//		PostQuitMessage(0);//OSに対して「もうこのアプリは終わるんや」と伝える
//		return 0;
//	}
//	return DefWindowProc(hwnd, msg, wparam, lparam);//規定の処理を行う
//}
//
///*
//	アライアメントに揃えたサイズを返す
//	引数: size 元のサイズ, alignment アライアメントサイズ
//	戻り値: アライアメントを揃えたサイズ
//*/
//size_t AlignmentedSize(size_t size, size_t alignment)
//{
//	auto remainder = size % alignment;
//	return (remainder == 0) ? size : size + (alignment - remainder);
//}
///*
//	モデルのパスとテクスチャのファイルパスから合成パスを得る
//	引数: modelPath アプリケーションから見たpmdモデルのパス, texPath PMDモデルから見たテクスチャのパス
//	戻り値: アプリケーションから見たテクスチャのパス
//*/
//std::string GetTexturePathFromModelAndTexPath(const std::string& modelPath, const char* texPath)
//{
//	int pathIndex1 = modelPath.rfind('/');
//	int pathIndex2 = modelPath.rfind('\\');
//	auto folderPath = modelPath.substr(0, max(pathIndex1, pathIndex2) + 1);
//	return folderPath + texPath;
//}
//
///*
//	std::stringからstd::wstringを得る
//	引数: str マルチバイト文字列
//	戻り値: 変換されたワイド文字列
//*/
//std::wstring GetWideStringFromString(const std::string& str)
//{
//	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
//
//	std::wstring wstr;
//	wstr.resize(num1);
//
//	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);
//
//	assert(num1 == num2);
//	return wstr;
//}
//
///*
//	ファイル名から拡張子を取得する
//	引数: path 対象のパス文字列
//	戻り値: 拡張子
//*/
//std::string GetExtension(const std::string& path)
//{
//	auto idx = path.rfind('.');
//	return path.substr(idx + 1, path.length() - idx - 1);
//}
//
///*
//	テクスチャのパスをセパレータ文字で分離する
//	引数: path 対象のパス文字列, splitter 区切り文字
//	戻り値: 分離前後の文字列ペア
//*/
//std::pair<std::string, std::string> SplitFileName(const std::string& path, const char splitter = '*')
//{
//	auto idx = path.find(splitter);
//
//	return std::pair<std::string, std::string>(path.substr(0, idx), path.substr(idx + 1, path.length() - idx - 1));
//}
//
//
//ID3D12Resource* LoadTextureFromFile(std::string& texPath);
//
//ID3D12Resource* CreateWhiteTexture();
//ID3D12Resource* CreateBlackTexture();
//ID3D12Resource* CreateGrayGradationTexture();
//
//constexpr UINT window_width = 1280;
//constexpr UINT window_height = 720;
//
//ComPtr<IDXGIFactory6> _dxgiFactory = nullptr;
//ComPtr<ID3D12Device> _dev = nullptr;
//ComPtr<ID3D12CommandAllocator> _cmdAllocator = nullptr;
//ComPtr<ID3D12GraphicsCommandList> _cmdList = nullptr;
//ComPtr<ID3D12CommandQueue> _cmdQueue = nullptr;
//ComPtr<IDXGISwapChain4> _swapchain = nullptr;
//ComPtr<ID3D12PipelineState> _pipelinestate = nullptr;
//std::map<std::string, LoadLambda_t> loadLambdaTable;
//
//ComPtr<ID3DBlob> vsBlob = nullptr;
//ComPtr<ID3DBlob> psBlob = nullptr;
//
////2D用画像Polygon
//struct GraphVertex2D {
//public:
//	constexpr GraphVertex2D() {}
//	constexpr GraphVertex2D(Vertex bl, Vertex lt, Vertex rb, Vertex rt) :bl(bl), lt(lt), rb(rb), rt(rt) {}
//	constexpr GraphVertex2D(XMFLOAT2 pos, float width, float height) {
//		bl.pos.x = pos.x; bl.pos.y = pos.y + height; bl.pos.z = 0.f;
//		bl.uv = XMFLOAT2(0.f, 1.f);
//		lt.pos.x = pos.x; lt.pos.y = pos.y; lt.pos.z = 0.f;
//		lt.uv = XMFLOAT2(0.f, 0.f);
//		rb.pos.x = pos.x + width; rb.pos.y = pos.y + height; rb.pos.z = 0.f;
//		rb.uv = XMFLOAT2(1.f, 1.f);
//		rt.pos.x = pos.x + width; rt.pos.y = pos.y; rt.pos.z = 0.f;
//		rt.uv = XMFLOAT2(1.f, 0.f);
//	}
//
//	Vertex bl;//左下
//	Vertex lt;//左上
//	Vertex rb;//右下
//	Vertex rt;//右上
//};
//
////3D用画像Polygon
//struct GraphVertex3D {
//public:
//	constexpr GraphVertex3D() {}
//	constexpr GraphVertex3D(Vertex bl, Vertex lt, Vertex rb, Vertex rt) :bl(bl), lt(lt), rb(rb), rt(rt) {}
//	constexpr GraphVertex3D(XMFLOAT2 pos, float width, float height) {
//		bl.pos.x = pos.x; bl.pos.y = pos.y; bl.pos.z = 0.f;
//		bl.uv = XMFLOAT2(0.f, 1.f);
//		lt.pos.x = pos.x; lt.pos.y = pos.y + height; lt.pos.z = 0.f;
//		lt.uv = XMFLOAT2(0.f, 0.f);
//		rb.pos.x = pos.x + width; rb.pos.y = pos.y; rb.pos.z = 0.f;
//		rb.uv = XMFLOAT2(1.f, 1.f);
//		rt.pos.x = pos.x + width; rt.pos.y = pos.y + height; rt.pos.z = 0.f;
//		rt.uv = XMFLOAT2(1.f, 0.f);
//	}
//
//	Vertex bl;//左下
//	Vertex lt;//左上
//	Vertex rb;//右下
//	Vertex rt;//右上
//};
//
//void EnableDebugLayer() {
//	ID3D12Debug* debugLayer = nullptr;
//	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)))) {
//		debugLayer->EnableDebugLayer();
//		debugLayer->Release();
//	}
//}
//
//#include<Windows.h>
//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
//	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
//	//MyLib::Common::InitRandom();
//
//	HINSTANCE hInst = GetModuleHandle(nullptr);
//	//ウィンドウクラス生成＆登録
//	WNDCLASSEX w = {};
//	w.cbSize = sizeof(WNDCLASSEX);
//	w.lpfnWndProc = (WNDPROC)WindowProcedure;//コールバック関数の指定
//	w.lpszClassName = _T("DirectXTest");//アプリケーションクラス名(適当でいいです)
//	w.hInstance = GetModuleHandle(0);//ハンドルの取得
//	RegisterClassEx(&w);//アプリケーションクラス(こういうの作るからよろしくってOSに予告する)
//
//	RECT wrc = { 0,0, window_width, window_height };//ウィンドウサイズを決める
//	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//ウィンドウのサイズはちょっと面倒なので関数を使って補正する
//	//ウィンドウオブジェクトの生成
//	HWND hwnd = CreateWindow(w.lpszClassName,//クラス名指定
//		_T("DX12テスト"),//タイトルバーの文字
//		WS_OVERLAPPEDWINDOW,//タイトルバーと境界線があるウィンドウです
//		CW_USEDEFAULT,//表示X座標はOSにお任せします
//		CW_USEDEFAULT,//表示Y座標はOSにお任せします
//		wrc.right - wrc.left,//ウィンドウ幅
//		wrc.bottom - wrc.top,//ウィンドウ高
//		nullptr,//親ウィンドウハンドル
//		nullptr,//メニューハンドル
//		w.hInstance,//呼び出しアプリケーションハンドル
//		nullptr);//追加パラメータ
//
//#ifdef _DEBUG
//	//デバッグレイヤーをオンに
//	EnableDebugLayer();
//#endif
//	//DirectX12まわり初期化
//	//フィーチャレベル列挙
//	D3D_FEATURE_LEVEL levels[] = {
//		D3D_FEATURE_LEVEL_12_1,
//		D3D_FEATURE_LEVEL_12_0,
//		D3D_FEATURE_LEVEL_11_1,
//		D3D_FEATURE_LEVEL_11_0,
//	};
//	if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(_dxgiFactory.ReleaseAndGetAddressOf())))) {
//		if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(_dxgiFactory.ReleaseAndGetAddressOf())))) {
//			return -1;
//		}
//	}
//	std::vector <IDXGIAdapter*> adapters;
//	IDXGIAdapter* tmpAdapter = nullptr;
//	for (int i = 0; _dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
//		adapters.push_back(tmpAdapter);
//	}
//	for (auto adpt : adapters) {
//		DXGI_ADAPTER_DESC adesc = {};
//		adpt->GetDesc(&adesc);
//		std::wstring strDesc = adesc.Description;
//		if (strDesc.find(L"NVIDIA") != std::string::npos) {
//			tmpAdapter = adpt;
//			break;
//		}
//	}
//
//	//Direct3Dデバイスの初期化
//	D3D_FEATURE_LEVEL featureLevel;
//	for (auto l : levels) {
//		if (D3D12CreateDevice(tmpAdapter, l, IID_PPV_ARGS(_dev.ReleaseAndGetAddressOf())) == S_OK) {
//			featureLevel = l;
//			break;
//		}
//	}
//
//	result = _dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_cmdAllocator.ReleaseAndGetAddressOf()));
//	result = _dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAllocator.Get(), nullptr, IID_PPV_ARGS(_cmdList.ReleaseAndGetAddressOf()));
//	//_cmdList->Close();
//	//コマンドキューの設定
//	{	
//		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
//		cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;//タイムアウトなし
//		cmdQueueDesc.NodeMask = 0;
//		cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;//プライオリティ特に指定なし
//		cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;//ここはコマンドリストと合わせてください
//		result = _dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(_cmdQueue.ReleaseAndGetAddressOf()));//コマンドキュー生成
//	}
//	//スワップチェーンの設定
//	{	
//		DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
//		swapchainDesc.Width = window_width;
//		swapchainDesc.Height = window_height;
//		swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//		swapchainDesc.Stereo = false;
//		swapchainDesc.SampleDesc.Count = 1;
//		swapchainDesc.SampleDesc.Quality = 0;
//		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//		swapchainDesc.BufferCount = 2;
//		swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
//		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//		swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
//		swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//
//		result = _dxgiFactory->CreateSwapChainForHwnd(_cmdQueue.Get(), hwnd, &swapchainDesc, nullptr, nullptr, (IDXGISwapChain1**)_swapchain.ReleaseAndGetAddressOf());
//	}
//
//	ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;
//	std::vector<ComPtr<ID3D12Resource>> _backBuffers;
//	//Heap関連
//	{	
//		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
//		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビューなので当然RTV
//		heapDesc.NodeMask = 0;
//		heapDesc.NumDescriptors = 2;//表裏の２つ
//		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;//特に指定なし
//		result = _dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(rtvHeaps.ReleaseAndGetAddressOf()));
//		DXGI_SWAP_CHAIN_DESC swcDesc = {};
//		result = _swapchain->GetDesc(&swcDesc);
//		_backBuffers.resize(swcDesc.BufferCount, nullptr);
//		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
//
//		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
//
//		rtvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
//		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
//		for (UINT i = 0; i < swcDesc.BufferCount; ++i) {
//			result = _swapchain->GetBuffer(i, IID_PPV_ARGS(_backBuffers[i].ReleaseAndGetAddressOf()));
//			_dev->CreateRenderTargetView(_backBuffers[i].Get(), &rtvDesc, handle);
//			handle.ptr += _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//		}
//	}
//
//	ComPtr<ID3D12Resource> depthBuffer = nullptr;
//	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
//	//深度バッファ設定
//	{
//		auto depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, window_width, window_height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
//		
//		D3D12_HEAP_PROPERTIES depthHeapProp = {};
//		depthHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
//		depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
//		depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
//
//		D3D12_CLEAR_VALUE depthClearValue = {};
//		depthClearValue.DepthStencil.Depth = 1.f;
//		depthClearValue.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
//
//		result = _dev->CreateCommittedResource(&depthHeapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
//			&depthResDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(depthBuffer.ReleaseAndGetAddressOf()));
//
//		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
//		dsvHeapDesc.NumDescriptors = 1;
//		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
//		
//		result = _dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dsvHeap.ReleaseAndGetAddressOf()));
//
//		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
//		dsvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
//		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;
//		dsvDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
//
//		_dev->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
//	}
//	ComPtr<ID3D12Fence> _fence = nullptr;
//	UINT64 _fenceVal = 0;
//	result = _dev->CreateFence(_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_fence.ReleaseAndGetAddressOf()));
//
//	//PMDヘッダ構造体
//	struct PMDHeader {
//		float version;			//例:00 00 80 3F == 1.00
//		char model_name[20];	//モデル名
//		char comment[256];		//モデルコメント
//	};
//	//PMD頂点構造体
//	struct PMDVertex {
//		XMFLOAT3 pos;
//		XMFLOAT3 normal;
//		XMFLOAT2 uv;
//		unsigned short boneNo[2];
//		unsigned char boneWeight;
//		unsigned char edgeFlg;
//	};
//
//#pragma pack(1)
//	//PMDマテリアル構造体
//	struct PMDMeterial {
//		XMFLOAT3 diffuse;	 //ディフューズカラー
//		float alpha;		 //ディフューズα
//		float specularity;	 //スペキュラの強さ
//		XMFLOAT3 specular;	 //スペキュラカラー
//		XMFLOAT3 ambient;	 //アンビエントカラー
//		u_char toonIdx;		 //トゥーン番号
//		u_char edgeFlg;		 //マテリアルごとの輪郭線フラグ
//		u_int indicesNum;	 //このマテリアルが割り当てられるインデックス数
//		char texFilePath[20];//テクスチャファイルパス+α
//	};
//#pragma pack()
//	struct MaterialForHlsl {
//		XMFLOAT3 diffuse;	 //ディフューズカラー
//		float alpha;		 //ディフューズα
//		XMFLOAT3 specular;	 //スペキュラカラー
//		float specularity;	 //スペキュラの強さ
//		XMFLOAT3 ambient;	 //アンビエントカラー
//	};
//	struct AdditionalMaterial {
//		std::string texPath;
//		int toonIdx;
//		bool edgeFlg;
//	};
//	struct Material {
//		unsigned int indicesNum;
//		MaterialForHlsl material;
//		AdditionalMaterial additional;
//	};
//
//	PMDHeader pmdheader;
//	std::vector<uint8_t> vertices;
//	std::vector<uint16_t> indices;
//	constexpr size_t pmdvertex_size = 38;
//	unsigned int vertnum = 0;
//	unsigned int indicesNum = 0;
//	u_int materialNum = 0;
//	std::vector<Material> materials;
//	std::vector<ComPtr<ID3D12Resource>> textureResources;
//	std::vector<ComPtr<ID3D12Resource>> sphResources;
//	std::vector<ComPtr<ID3D12Resource>> spaResources;
//	std::vector<ComPtr<ID3D12Resource>> toonResources;
//
//	loadLambdaTable["sph"] = loadLambdaTable["spa"] = loadLambdaTable["bmp"] = loadLambdaTable["png"] = loadLambdaTable["jpg"] = 
//		[](const std::wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
//		return LoadFromWICFile(path.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, meta, img);
//	};
//
//	loadLambdaTable["tga"] = [](const std::wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
//		return LoadFromTGAFile(path.c_str(), meta, img);
//	};
//	loadLambdaTable["dds"] = [](const std::wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
//		return LoadFromDDSFile(path.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, meta, img);
//	};
//
//	//std::string strModelPath = "Model/鏡音リン_act2.pmd";
//	//std::string strModelPath = "Model/初音ミクmetal.pmd";
//	std::string strModelPath = "Model/巡音ルカ.pmd";
//	if (autoClose fp = fopen(strModelPath.c_str(), "rb")) {
//		char signature[3];
//		fread(signature, sizeof(signature), 1, fp);
//		fread(&pmdheader, sizeof(pmdheader), 1, fp);
//
//		fread(&vertnum, sizeof(vertnum), 1, fp);
//
//		vertices.resize(vertnum * pmdvertex_size);
//
//		fread(vertices.data(), vertices.size(), 1, fp);
//
//		fread(&indicesNum, sizeof(indicesNum), 1, fp);
//
//		indices.resize(indicesNum);
//
//		fread(indices.data(), indices.size() * sizeof(indices[0]), 1, fp);
//
//		fread(&materialNum, sizeof(materialNum), 1, fp);
//
//		std::vector<PMDMeterial> pmdMaterials(materialNum);
//		fread(pmdMaterials.data(), pmdMaterials.size() * sizeof(PMDMeterial), 1, fp);
//
//		toonResources.resize(materialNum);
//
//		materials.resize(pmdMaterials.size());
//
//		textureResources.resize(materialNum);
//		sphResources.resize(materialNum);
//		spaResources.resize(materialNum);
//
//		for (int i = 0; i < pmdMaterials.size(); i++) {
//			std::string toonFilePath = "toon/";
//
//			char toonFileName[16];
//
//			sprintf(toonFileName, "toon%02d.bmp", pmdMaterials[i].toonIdx + 1);
//
//			toonFilePath += toonFileName;
//
//			toonResources[i] = LoadTextureFromFile(toonFilePath);
//
//			const auto& pmdmaterial = pmdMaterials[i];
//			auto& material = materials[i];
//			material.indicesNum = pmdmaterial.indicesNum;
//			material.material.diffuse = pmdmaterial.diffuse;
//			material.material.alpha = pmdmaterial.alpha;
//			material.material.specular = pmdmaterial.specular;
//			material.material.specularity = pmdmaterial.specularity;
//			material.material.ambient = pmdmaterial.ambient;
//
//			auto& texRes = textureResources[i];
//
//			std::string texFileName = pmdmaterial.texFilePath;
//			std::string sphFileName = "";
//			std::string spaFileName = "";
//			if (texFileName.length() == 0) {
//				texRes = nullptr;
//				continue;
//			}
//
//			if (std::count(texFileName.begin(), texFileName.end(), '*') > 0) {
//				auto namepair = SplitFileName(texFileName);
//				auto f_exe = GetExtension(namepair.first);					
//
//				if (f_exe == "sph") {
//					texFileName = namepair.second;
//					sphFileName = namepair.first;
//				}
//				else if (f_exe == "spa") {
//					texFileName = namepair.second;
//					spaFileName = namepair.first;
//				}
//				else {
//					texFileName = namepair.first;
//					auto s_exe = GetExtension(namepair.second);
//					if (s_exe == "sph") {
//						sphFileName = namepair.second;
//					}
//					else if (s_exe == "spa") {
//						spaFileName = namepair.second;
//					}
//				}
//			}
//
//			auto texFilePath = GetTexturePathFromModelAndTexPath(strModelPath, texFileName.c_str());
//
//			texRes = LoadTextureFromFile(texFilePath);
//
//			if (texFileName != "") {
//				auto texFilePath = GetTexturePathFromModelAndTexPath(strModelPath, texFileName.c_str());
//				textureResources[i] = LoadTextureFromFile(texFilePath);
//			}
//			if (sphFileName != "") {
//				auto sphFilePath = GetTexturePathFromModelAndTexPath(strModelPath, sphFileName.c_str());
//				sphResources[i] = LoadTextureFromFile(sphFilePath);
//			}
//			if (spaFileName != "") {
//				auto spaFilePath = GetTexturePathFromModelAndTexPath(strModelPath, spaFileName.c_str());
//				spaResources[i] = LoadTextureFromFile(spaFilePath);
//			}
//		}
//	}
//
//	ComPtr<ID3D12Resource> vertBuff = nullptr;
//	ComPtr<ID3D12Resource> ibxBuff = nullptr;
//	{
//		auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
//		auto heapdesc = CD3DX12_RESOURCE_DESC::Buffer(vertices.size());
//
//		result = _dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &heapdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(vertBuff.ReleaseAndGetAddressOf()));
//
//		auto idxdesc = CD3DX12_RESOURCE_DESC::Buffer(indices.size() * sizeof(indices[0]));
//		result = _dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &idxdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(ibxBuff.ReleaseAndGetAddressOf()));
//	}
//	{	//頂点Bufferの設定
//		uint8_t* vertMap = nullptr;
//		result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//		std::copy(std::begin(vertices), std::end(vertices), vertMap);
//		vertBuff->Unmap(0, nullptr);
//	}
//	{
//		uint16_t* mappedIdx = nullptr;
//		ibxBuff->Map(0, nullptr, (void**)&mappedIdx);
//		std::copy(std::begin(indices), std::end(indices), mappedIdx);
//		ibxBuff->Unmap(0, nullptr);
//	}
//	ComPtr<ID3D12Resource> materialBuff = nullptr;
//	//要修正
//	constexpr auto materialBuffSize = (sizeof(MaterialForHlsl) + 0xff) & ~0xff;
//	{
//		auto pro = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
//		auto buf = CD3DX12_RESOURCE_DESC::Buffer(materialBuffSize * materialNum);
//		result = _dev->CreateCommittedResource(&pro, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &buf, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(materialBuff.ReleaseAndGetAddressOf()));
//	
//		char* mapMaterial = nullptr;
//		result = materialBuff->Map(0, nullptr, (void**)&mapMaterial);
//		for (auto& m : materials) {
//			*((MaterialForHlsl*)mapMaterial) = m.material;
//			mapMaterial += materialBuffSize;
//		}
//		materialBuff->Unmap(0, nullptr);
//	}
//	ComPtr<ID3D12DescriptorHeap> materialDescHeap = nullptr;
//	{
//		D3D12_DESCRIPTOR_HEAP_DESC matDescHeapDesc = {};
//
//		matDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//		matDescHeapDesc.NodeMask = 0;
//		matDescHeapDesc.NumDescriptors = materialNum * 5;
//		matDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//
//		result = _dev->CreateDescriptorHeap(&matDescHeapDesc, IID_PPV_ARGS(materialDescHeap.ReleaseAndGetAddressOf()));
//
//		D3D12_CONSTANT_BUFFER_VIEW_DESC matCBVDesc = {};
//
//		matCBVDesc.BufferLocation = materialBuff->GetGPUVirtualAddress();
//		matCBVDesc.SizeInBytes = materialBuffSize;
//
//		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//
//		srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//		srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
//		srvDesc.Texture2D.MipLevels = 1;
//
//		auto matDescHeapH = materialDescHeap->GetCPUDescriptorHandleForHeapStart();
//
//		auto incSize = _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//		for (u_int i = 0; i < materialNum; i++) {
//			_dev->CreateConstantBufferView(&matCBVDesc, matDescHeapH);
//
//			matDescHeapH.ptr += incSize;
//			matCBVDesc.BufferLocation += materialBuffSize;
//
//			auto& texRes = textureResources[i];
//			if (texRes == nullptr) {
//				auto whiteTex = CreateWhiteTexture();
//				srvDesc.Format = whiteTex->GetDesc().Format;
//				_dev->CreateShaderResourceView(whiteTex, &srvDesc, matDescHeapH);
//			}
//			else {
//				srvDesc.Format = texRes->GetDesc().Format;
//				_dev->CreateShaderResourceView(texRes.Get(), &srvDesc, matDescHeapH);
//			}
//			matDescHeapH.ptr += incSize;
//
//			auto& sphRes = sphResources[i];
//			if (sphRes == nullptr) {
//				auto whiteTex = CreateWhiteTexture();
//				srvDesc.Format = whiteTex->GetDesc().Format;
//				_dev->CreateShaderResourceView(whiteTex, &srvDesc, matDescHeapH);
//			}
//			else {
//				srvDesc.Format = sphRes->GetDesc().Format;
//				_dev->CreateShaderResourceView(sphRes.Get(), &srvDesc, matDescHeapH);
//			}
//			matDescHeapH.ptr += incSize;
//						
//			auto& spaRes = spaResources[i];
//			if (spaRes == nullptr) {
//				auto blackTex = CreateBlackTexture();
//				srvDesc.Format = blackTex->GetDesc().Format;
//				_dev->CreateShaderResourceView(blackTex, &srvDesc, matDescHeapH);
//			}
//			else {
//				srvDesc.Format = spaRes->GetDesc().Format;
//				_dev->CreateShaderResourceView(spaRes.Get(), &srvDesc, matDescHeapH);
//			}
//			matDescHeapH.ptr += incSize;
//
//			auto& toonRes = toonResources[i];
//			if (!toonRes) {
//				auto gradTex = CreateGrayGradationTexture();
//				srvDesc.Format = gradTex->GetDesc().Format;
//				_dev->CreateShaderResourceView(gradTex, &srvDesc, matDescHeapH);
//			}
//			else {
//				srvDesc.Format = toonRes->GetDesc().Format;
//				_dev->CreateShaderResourceView(toonRes.Get(), &srvDesc, matDescHeapH);
//			}
//			matDescHeapH.ptr += incSize;
//		}
//	}
//
//	//頂点Bufferビュー
//	D3D12_VERTEX_BUFFER_VIEW vbView = {};
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	vbView.SizeInBytes = static_cast<UINT>(vertices.size());
//	vbView.StrideInBytes = (pmdvertex_size);
//
//	D3D12_INDEX_BUFFER_VIEW ibView = {};
//	ibView.BufferLocation = ibxBuff->GetGPUVirtualAddress();
//	ibView.Format = DXGI_FORMAT_R16_UINT;
//	ibView.SizeInBytes = static_cast<UINT>(indices.size() * sizeof(indices[0]));
//
//	ComPtr<ID3DBlob> errorBlob = nullptr;
//
//	//コンパイル定数:https://learn.microsoft.com/ja-jp/windows/win32/direct3dhlsl/d3dcompile-constants
//	result = D3DCompileFromFile(L"Shader/PixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
//		"BasicPS", "ps_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, psBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
//
//	result = D3DCompileFromFile(L"Shader/VertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
//		"BasicVS", "vs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, vsBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
//
//	CD3DX12_ROOT_PARAMETER rootparam[2] = {};
//	//要修正
//	CD3DX12_DESCRIPTOR_RANGE descTblRange[3] = {};
//	CD3DX12_STATIC_SAMPLER_DESC samplerDesc[2] = {};
//	{
//		//座標変換用
//		descTblRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);		
//		//マテリアル用
//		descTblRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
//		//テクスチャ用
//		descTblRange[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 0);
//
//		rootparam[0].InitAsDescriptorTable(1, &descTblRange[0]);
//		rootparam[1].InitAsDescriptorTable(2, &descTblRange[1]);
//
//		samplerDesc[0].Init(0);
//		samplerDesc[1].Init(1, D3D12_FILTER::D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
//	}
//
//	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
//		/*
//			LPCSTR SemanticName; UINT SemanticIndex; DXGI_FORMAT Format; UINT InputSlot;
//			UINT AlignedByteOffset; D3D12_INPUT_CLASSIFICATION InputSlotClass; UINT InstanceDataStepRate;
//		*/
//		//HLSLセマンティック インデックス	要素データのフォーマット		入力スロット	オフセット(自動定義)			識別値							
//		{"POSITION",			0,	DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,	0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
//		{"NORMAL",				0,	DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,	0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
//		{"TEXCOORD",			0,	DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,		0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
//		{"BONE_NO",				0,	DXGI_FORMAT::DXGI_FORMAT_R16G16_UINT,		0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
//		{"WEIGHT",				0,	DXGI_FORMAT::DXGI_FORMAT_R8_UINT,			0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
//		{"EDGE_FLG",			0,	DXGI_FORMAT::DXGI_FORMAT_R8_UINT,			0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
//	};
//
//	ComPtr<ID3D12RootSignature> rootsignature = nullptr;
//	ComPtr<ID3DBlob> rootSigBlob = nullptr;
//	{	//グラフィックパイプライン設定
//		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
//		gpipeline.pRootSignature = nullptr;//
//
//		//頂点シェーダ設定
//		gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
//		//ピクセルシェーダ設定
//		gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
//
//		gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
//
//		gpipeline.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
//
//		gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
//		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
//
//		gpipeline.DepthStencilState.DepthEnable = true;	//深度Bufferを使う
//		gpipeline.DepthStencilState.StencilEnable = false;
//
//		gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;//
//		gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	//小さいほうを採用
//
//		gpipeline.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
//
//		gpipeline.InputLayout.pInputElementDescs = inputLayout;
//		gpipeline.InputLayout.NumElements = _countof(inputLayout);
//
//		gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//カットなし
//		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//
//		gpipeline.NumRenderTargets = 1;
//		gpipeline.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
//
//		gpipeline.SampleDesc.Count = 1;
//		gpipeline.SampleDesc.Quality = 0;
//
//		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
//		rootSignatureDesc.Init(2, rootparam, 2, samplerDesc, D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
//
//		result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, rootSigBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
//		result = _dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootsignature.ReleaseAndGetAddressOf()));
//
//		gpipeline.pRootSignature = rootsignature.Get();
//		result = _dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(_pipelinestate.ReleaseAndGetAddressOf()));
//	}
//
//	CD3DX12_VIEWPORT viewport(_backBuffers[0].Get());
//
//	CD3DX12_RECT scissorrect(0, 0, window_width, window_height);
//	
//	auto worldMat = XMMatrixRotationY(XM_PIDIV4);
//	float angle = 0.f;
//	XMFLOAT3 eye(0, 15, -12.5);
//	XMFLOAT3 target(0, 12.5, 0);
//	XMFLOAT3 up(0, 1, 0);
//
//	auto viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
//
//	auto projMat = XMMatrixPerspectiveFovLH(XM_PIDIV2, static_cast<float>(window_width) / static_cast<float>(window_height), 1.f, 100.f);
//
//	/*XMMATRIX mat = XMMatrixIdentity();
//	mat.r[0].m128_f32[0] = 2.0f / window_width;
//	mat.r[1].m128_f32[1] = -2.0f / window_height;
//	mat.r[3].m128_f32[0] = -1.0f;
//	mat.r[3].m128_f32[1] = 1.0f;*/
//	ComPtr<ID3D12Resource> constbuff = nullptr;
//	struct MatricesData {
//		XMMATRIX world;
//		XMMATRIX view;
//		XMMATRIX proj;
//		XMFLOAT3 eye;//視点座標
//	};
//
//	{
//		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
//		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatricesData) + 0xff) & ~0xff);
//		result = _dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(constbuff.ReleaseAndGetAddressOf()));
//	}
//
//	MatricesData* mapMat = nullptr;
//	result = constbuff->Map(0, nullptr, (void**)&mapMat);
//	mapMat->world = worldMat;
//	mapMat->view = projMat;
//	mapMat->proj = viewMat;
//	mapMat->eye = eye;
//	ComPtr<ID3D12DescriptorHeap> basicDescHeap = nullptr;
//	{
//		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
//		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
//		descHeapDesc.NodeMask = 0;//マスクは0
//		descHeapDesc.NumDescriptors = 1;//CBV1つ
//		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//デスクリプタヒープ種別
//		result = _dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(basicDescHeap.ReleaseAndGetAddressOf()));//生成
//
//		////デスクリプタの先頭ハンドルを取得しておく
//		auto basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();
//		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
//		cbvDesc.BufferLocation = constbuff->GetGPUVirtualAddress();
//		cbvDesc.SizeInBytes = static_cast<UINT>(constbuff->GetDesc().Width);
//		//定数バッファビューの作成
//		_dev->CreateConstantBufferView(&cbvDesc, basicHeapHandle);
//	}
//	//gpipeline.BlendState.
//	ShowWindow(hwnd, SW_SHOW);//ウィンドウ表示
//
//	MSG msg = {};
//	while (true) {
//		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		//もうアプリケーションが終わるって時にmessageがWM_QUITになる
//		if (msg.message == WM_QUIT) {
//			break;
//		}
//
//		//DirectX処理
//		//バックバッファのインデックスを取得
//		auto bbIdx = _swapchain->GetCurrentBackBufferIndex();
//
//		{
//			auto BarrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(_backBuffers[bbIdx].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
//			_cmdList->ResourceBarrier(1, &BarrierDesc);
//		}
//
//		_cmdList->SetPipelineState(_pipelinestate.Get());
//
//		//レンダーターゲットを指定
//		auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
//		rtvH.ptr += bbIdx * _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//		auto dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
//		_cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
//
//		//画面クリア
//		float clearColor[] = { 0.7f,0.7f,0.7f,1.0f };//黄色
//		_cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
//		_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
//
//		_cmdList->SetGraphicsRootSignature(rootsignature.Get());
//		_cmdList->RSSetViewports(1, &viewport);
//		_cmdList->RSSetScissorRects(1, &scissorrect);
//
//		angle += 0.01f;
//		worldMat = XMMatrixRotationY(angle);
//		mapMat->world = worldMat;
//		mapMat->view = viewMat;
//		mapMat->proj = projMat;
//
//		_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//		_cmdList->IASetVertexBuffers(0, 1, &vbView);
//		_cmdList->IASetIndexBuffer(&ibView);
//
//		_cmdList->SetGraphicsRootSignature(rootsignature.Get());
//
//		//_cmdList->DrawIndexedInstanced(indicesNum, 1, 0, 0, 0);
//		//WVP変換行列
//		_cmdList->SetDescriptorHeaps(1, basicDescHeap.GetAddressOf());
//		_cmdList->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());
//
//		_cmdList->SetDescriptorHeaps(1, materialDescHeap.GetAddressOf());
//
//		auto materialH = materialDescHeap->GetGPUDescriptorHandleForHeapStart();
//		unsigned int idxOffset = 0;
//
//		//auto cbvsrvIncSize = _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 5;
//		static const auto cbvssrvIncSize = _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 5;
//		for (auto& m : materials) {
//			_cmdList->SetGraphicsRootDescriptorTable(1, materialH);
//			_cmdList->DrawIndexedInstanced(m.indicesNum, 1, idxOffset, 0, 0);
//			materialH.ptr += cbvssrvIncSize;
//			idxOffset += m.indicesNum;
//		}
//
//		/*BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
//		BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;*/
//		{
//			auto BarrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(_backBuffers[bbIdx].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
//			_cmdList->ResourceBarrier(1, &BarrierDesc);
//		}
//
//		//命令のクローズ
//		_cmdList->Close();
//
//		//コマンドリストの実行
//		ID3D12CommandList* cmdlists[] = { _cmdList.Get()};
//		_cmdQueue->ExecuteCommandLists(1, cmdlists);
//		////待ち
//		_cmdQueue->Signal(_fence.Get(), ++_fenceVal);
//
//		if (_fence->GetCompletedValue() != _fenceVal) {
//			auto event = CreateEvent(nullptr, false, false, nullptr);
//			_fence->SetEventOnCompletion(_fenceVal, event);
//			WaitForSingleObject(event, INFINITE);
//			CloseHandle(event);
//		}
//
//		//フリップ
//		_swapchain->Present(1, 0);
//
//		_cmdAllocator->Reset();//キューをクリア
//		_cmdList->Reset(_cmdAllocator.Get(), nullptr);//再びコマンドリストをためる準備
//	}
//	//もうクラス使わんから登録解除してや
//	UnregisterClass(w.lpszClassName, w.hInstance);
//	return 0;
//}
//
//ID3D12Resource* LoadTextureFromFile(std::string& texPath)
//{
//	static std::map<std::string, ComPtr<ID3D12Resource>> resourceTable;
//
//	if (auto itr = resourceTable.find(texPath); itr != resourceTable.end()) {
//		return itr->second.Get();
//	}
//
//	TexMetadata metadata = {};
//	ScratchImage scratchImg = {};
//	ID3D12Resource* texbuff = nullptr;
//
//	HRESULT result;
//
//	if (auto func = loadLambdaTable.find(GetExtension(texPath)); func != std::end(loadLambdaTable)) {
//		result = (*func).second(GetWideStringFromString(texPath), &metadata, scratchImg);
//	}
//	else {
//		goto Fail;
//	}
//
//	if (FAILED(result)) {
//		goto Fail;
//	}
//
//	{
//		auto img = scratchImg.GetImage(0, 0, 0);
//
//		auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0);
//
//		auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(metadata.format, metadata.width, static_cast<UINT>(metadata.height), static_cast<UINT16>(metadata.arraySize), static_cast<UINT16>(metadata.mipLevels));
//		
//		result = _dev->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&texbuff));
//
//		if (FAILED(result)) {
//			goto Fail;
//		}
//
//		result = texbuff->WriteToSubresource(0, nullptr, img->pixels, static_cast<UINT>(img->rowPitch), static_cast<UINT>(img->slicePitch));
//
//		if (FAILED(result)) {
//			goto Fail;
//		}
//	}
//
//	resourceTable[texPath] = texbuff;
//	return texbuff;
//
//Fail:	//失敗時の処理
//	return nullptr;
//}
//
//ID3D12Resource* CreateWhiteTexture()
//{
//	static ComPtr<ID3D12Resource> tex = nullptr;
//	if (tex) {
//		return tex.Get();
//	}
//
//	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0);
//
//	auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 4, 4);
//
//	ID3D12Resource* whiteBuff = nullptr;
//
//	auto result = _dev->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&whiteBuff));
//
//	if (FAILED(result)) {
//		goto Fail;
//	}
//
//	{
//		std::vector<u_char> data(4 * 4 * 4);
//		std::fill(data.begin(), data.end(), 0xff);
//
//		result = whiteBuff->WriteToSubresource(0, nullptr, data.data(), 4 * 4, static_cast<UINT>(data.size()));
//	}
//
//	tex = whiteBuff;
//	return whiteBuff;
//Fail:	//失敗時の処理
//	return nullptr;
//}
//
//ID3D12Resource* CreateBlackTexture()
//{
//	static ComPtr<ID3D12Resource> tex = nullptr;
//	if (tex) {
//		return tex.Get();
//	}
//
//	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0);
//
//	auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 4, 4);
//
//	ID3D12Resource* blackBuff = nullptr;
//
//	auto result = _dev->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&blackBuff));
//
//	if (FAILED(result)) {
//		goto Fail;
//	}
//
//	{
//		std::vector<u_char> data(4 * 4 * 4);
//		std::fill(data.begin(), data.end(), 0x00);
//
//		result = blackBuff->WriteToSubresource(0, nullptr, data.data(), 4 * 4, static_cast<UINT>(data.size()));
//	}
//
//	tex = blackBuff;
//	return blackBuff;
//Fail:	//失敗時の処理
//	return nullptr;
//}
//
//ID3D12Resource* CreateGrayGradationTexture()
//{
//	static ComPtr<ID3D12Resource> tex = nullptr;
//	if (tex) {
//		return tex.Get();
//	}
//
//	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0);
//
//	auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 3, 256);
//
//	ID3D12Resource* gradBuff = nullptr;
//
//	auto result = _dev->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&gradBuff));
//
//	if (FAILED(result)) {
//		goto Fail;
//	}
//
//	{
//		std::vector<u_int> data(4 * 256);
//		auto itr = data.begin();
//		u_int c = 0xff;
//		for (; itr != data.end(); itr += 4) {
//			auto col = (c << 24) | RGB(c, c, c);
//			std::fill(itr, itr + 4, col);
//			--c;
//		}
//
//		result = gradBuff->WriteToSubresource(0, nullptr, data.data(), static_cast<UINT>(4 * sizeof(u_int)), static_cast<UINT>(sizeof(u_int) * data.size()));
//	}
//
//	tex = gradBuff;
//	return gradBuff;
//Fail:	//失敗時の処理
//	return nullptr;
//}
