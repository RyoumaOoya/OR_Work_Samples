#include "Dx12Wrapper.h"
#include <cassert>
#include <d3dx12.h>
#include "../Application/Application.h"

#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

namespace {
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	/*
		モデルのパスとテクスチャのファイルパスから合成パスを得る
		引数: modelPath アプリケーションから見たpmdモデルのパス, texPath PMDモデルから見たテクスチャのパス
		戻り値: アプリケーションから見たテクスチャのパス
	*/
	std::string GetTexturePathFromModelAndTexPath(const std::string& modelPath, const char* texPath)
	{
		int pathIndex1 = static_cast<int>(modelPath.rfind('/'));
		int pathIndex2 = static_cast<int>(modelPath.rfind('\\'));
		auto folderPath = modelPath.substr(0, max(pathIndex1, pathIndex2) + 1);
		return folderPath + texPath;
	}
	/*
		std::stringからstd::wstringを得る
		引数: str マルチバイト文字列
		戻り値: 変換されたワイド文字列
	*/
	std::wstring GetWideStringFromString(const std::string& str)
	{
		auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
		std::wstring wstr;
		wstr.resize(num1);
		auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);
		assert(num1 == num2);
		return wstr;
	}
	/*
		ファイル名から拡張子を取得する
		引数: path 対象のパス文字列
		戻り値: 拡張子
	*/
	std::string GetExtension(const std::string& path)
	{
		auto idx = path.rfind('.');
		return path.substr(idx + 1, path.length() - idx - 1);
	
	}/*
		ファイル名から拡張子を取得する
		引数: path 対象のパス文字列
		戻り値: 拡張子
	*/
	std::wstring GetExtension(const std::wstring& path)
	{
		auto idx = path.rfind('.');
		return path.substr(idx + 1, path.length() - idx - 1);
	}
	/*
		テクスチャのパスをセパレータ文字で分離する
		引数: path 対象のパス文字列, splitter 区切り文字
		戻り値: 分離前後の文字列ペア
	*/
	std::pair<std::string, std::string> SplitFileName(const std::string& path, const char splitter = '*')
	{
		auto idx = path.find(splitter);
		return std::pair<std::string, std::string>(path.substr(0, idx), path.substr(idx + 1, path.length() - idx - 1));
	}
	///デバッグレイヤーを有効にする
	void EnableDebugLayer() {
		ComPtr<ID3D12Debug> debugLayer = nullptr;
		auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
		debugLayer->EnableDebugLayer();
	}
}

Dx12Wrapper::Dx12Wrapper(HWND hwnd) :backColor(0.7f, 0.7f, 0.7f, 1.f)
{
#ifdef _DEBUG
	//デバッグレイヤーをオンに
	EnableDebugLayer();
#endif
	auto app = Application::GetInstance();
	winSize_ = app->GetWindowSize();
	//DirectX12関連初期化
	if (FAILED(InitializeDXGIDevice())) {
		assert(0);
		return;
	}
	if (FAILED(InitializeCommand())) {
		assert(0);
		return;
	}
	if (FAILED(CreateSwapChain(hwnd))) {
		assert(0);
		return;
	}
	if (FAILED(CreateFinalRenderTargets())) {
		assert(0);
		return;
	}
	if (FAILED(CreateSceneView())) {
		assert(0);
		return;
	}
	//テクスチャローダー関連初期化
	CreateTextureLoaderTable();

	//深度バッファ作成
	if (FAILED(CreateDepthStencilView())) {
		assert(0);
		return;
	}
	//フェンスの作成
	if (FAILED(dev_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence_.ReleaseAndGetAddressOf())))) {
		assert(0);
		return;
	}
}

Dx12Wrapper::~Dx12Wrapper()
{
}

void Dx12Wrapper::Update()
{
}

void Dx12Wrapper::BeginDraw()
{
	//DirectX処理
	//バックバッファのインデックスを取得
	auto bbIdx = swapchain_->GetCurrentBackBufferIndex();
	{
		auto BarrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(backBuffers_[bbIdx], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmdList_->ResourceBarrier(1, &BarrierDesc);
	}

	//レンダーターゲットを指定
	auto rtvH = rtvHeaps_->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIdx * dev_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//深度バッファ指定
	auto dsvH = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	cmdList_->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	cmdList_->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	//画面クリア
	cmdList_->ClearRenderTargetView(rtvH, backColor, 0, nullptr);

	cmdList_->RSSetViewports(1, viewport_.get());
	cmdList_->RSSetScissorRects(1, scissorrect_.get());
}

void Dx12Wrapper::EndDraw()
{
	{
		auto BarrierDesc = CD3DX12_RESOURCE_BARRIER::Transition(backBuffers_[swapchain_->GetCurrentBackBufferIndex()], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		cmdList_->ResourceBarrier(1, &BarrierDesc);
	}
	//命令のクローズ
	cmdList_->Close();
	//コマンドリストの実行
	{
		ID3D12CommandList* cmdlists[] = { cmdList_.Get() };
		cmdQueue_->ExecuteCommandLists(1, cmdlists);
	}
	////待ち
	cmdQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_) {
		auto event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	cmdAllocator_->Reset();//キューをクリア
	cmdList_->Reset(cmdAllocator_.Get(), nullptr);//再びコマンドリストをためる準備
}

ComPtr<ID3D12Resource> Dx12Wrapper::GetTextureByPath(const char* texpath)
{
	return CreateTextureFromFile(texpath);
}

ComPtr<ID3D12Device> Dx12Wrapper::Device()
{
	return dev_;
}

ComPtr<ID3D12GraphicsCommandList> Dx12Wrapper::CommandList()
{
	return cmdList_;
}

ComPtr<IDXGISwapChain4> Dx12Wrapper::Swapchain()
{
	return swapchain_;
}

void Dx12Wrapper::SetScene()
{
	//現在のシーン(ビュープロジェクション)をセット
	ID3D12DescriptorHeap* sceneheaps[] = { sceneDescHeap_.Get() };
	cmdList_->SetDescriptorHeaps(1, sceneheaps);
	cmdList_->SetGraphicsRootDescriptorTable(0, sceneDescHeap_->GetGPUDescriptorHandleForHeapStart());
}

HRESULT Dx12Wrapper::CreateFinalRenderTargets()
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	HRESULT result;
	result = swapchain_->GetDesc1(&desc);

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビューなので当然RTV
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2;//表裏の２つ
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;//特に指定なし

	if (result = dev_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(rtvHeaps_.ReleaseAndGetAddressOf())); FAILED(result)) {
		return result;
	}

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	
	if (result = swapchain_->GetDesc(&swcDesc); FAILED(result)) {
		return result;
	}
	backBuffers_.resize(swcDesc.BufferCount, nullptr);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps_->GetCPUDescriptorHandleForHeapStart();

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};

	rtvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
	for (UINT i = 0; i < swcDesc.BufferCount; ++i) {
		if (result = swapchain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i])); FAILED(result)) {
			return result;
		}		
		rtvDesc.Format = backBuffers_[i]->GetDesc().Format;
		dev_->CreateRenderTargetView(backBuffers_[i], &rtvDesc, handle);
		handle.ptr += dev_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	viewport_.reset(new CD3DX12_VIEWPORT(backBuffers_[0]));
	scissorrect_.reset(new CD3DX12_RECT(0, 0, desc.Width, desc.Height));
	return result;
}

HRESULT Dx12Wrapper::CreateDepthStencilView()
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	HRESULT result;
	if (result = swapchain_->GetDesc1(&desc); FAILED(result)) {
		return result;
	}
	swapchain_->GetDesc1(&desc);
	//深度バッファ設定	
	auto depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, desc.Width, desc.Height, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	D3D12_HEAP_PROPERTIES depthHeapProp = {};
	depthHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	depthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	depthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.f;
	depthClearValue.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	result = dev_->CreateCommittedResource(&depthHeapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &depthResDesc, 
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(depthBuffer_.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		return result;
	}
	

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	if (result = dev_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dsvHeap_.ReleaseAndGetAddressOf())); FAILED(result)) {
		return result;
	}

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION::D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;

	dev_->CreateDepthStencilView(depthBuffer_.Get(), &dsvDesc, dsvHeap_->GetCPUDescriptorHandleForHeapStart());

	return result;
}

HRESULT Dx12Wrapper::CreateSwapChain(const HWND& hwnd)
{
	RECT rc = {};
	::GetWindowRect(hwnd, &rc);

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = winSize_.cx;
	swapchainDesc.Height = winSize_.cy;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = false;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	return dxgiFactory_->CreateSwapChainForHwnd(cmdQueue_.Get(), hwnd, &swapchainDesc, nullptr, nullptr, (IDXGISwapChain1**)swapchain_.ReleaseAndGetAddressOf());
}

HRESULT Dx12Wrapper::InitializeDXGIDevice()
{
	//DirectX12まわり初期化
	//フィーチャレベル列挙
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	HRESULT result;
	if (result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(dxgiFactory_.ReleaseAndGetAddressOf())); FAILED(result)) {
		if (result = CreateDXGIFactory2(0, IID_PPV_ARGS(dxgiFactory_.ReleaseAndGetAddressOf())); FAILED(result)) {
			return result;
		}
	}
	std::vector <IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0; dxgiFactory_->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
		adapters.push_back(tmpAdapter);
	}
	for (auto adpt : adapters) {
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"NVIDIA") != std::string::npos) {
			tmpAdapter = adpt;
			break;
		}
	}

	result = S_FALSE;
	//Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL featureLevel;
	for (auto l : levels) {
		if (SUCCEEDED(D3D12CreateDevice(tmpAdapter, l, IID_PPV_ARGS(dev_.ReleaseAndGetAddressOf())))) {
			featureLevel = l;
			result = S_OK;
			break;
		}
	}
	tmpAdapter->Release();
	return result;
}

HRESULT Dx12Wrapper::InitializeCommand()
{
	HRESULT result;
	if (result = dev_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocator_.ReleaseAndGetAddressOf())); FAILED(result)) {
		return result;
	}
	if (result = dev_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator_.Get(), nullptr, IID_PPV_ARGS(cmdList_.ReleaseAndGetAddressOf())); FAILED(result)) {
		return result;
	}

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;//タイムアウトなし
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;//プライオリティ特に指定なし
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;//ここはコマンドリストと合わせてください

	return  dev_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(cmdQueue_.ReleaseAndGetAddressOf()));//コマンドキュー生成
}

HRESULT Dx12Wrapper::CreateSceneView()
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	auto result = swapchain_->GetDesc1(&desc);

	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapBuff = CD3DX12_RESOURCE_DESC::Buffer((sizeof(SceneData) + 0xff) & ~0xff);

	//定数バッファ作成

	if (result = dev_->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &heapBuff, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(sceneConstBuff_.ReleaseAndGetAddressOf()));
		FAILED(result)) {
		return result;
	}

	mappedSceneData_ = nullptr;//マップ先を示すポインタ
	if (result = sceneConstBuff_->Map(0, nullptr, (void**)&mappedSceneData_); FAILED(result)) {
		return result;
	}

	XMFLOAT3 eye(0, 15, -15);
	XMFLOAT3 target(0, 15, 0);
	XMFLOAT3 up(0, 1, 0);
	mappedSceneData_->view = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	mappedSceneData_->proj = XMMatrixPerspectiveFovLH(XM_PIDIV4,/*画角は45°*/
		static_cast<float>(desc.Width) / static_cast<float>(desc.Height),/*アス比*/0.1f,/*近い方*/1000.0f/*遠い方*/);
	mappedSceneData_->eye = eye;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NodeMask = 0;//マスクは0
	descHeapDesc.NumDescriptors = 1;//
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//デスクリプタヒープ種別

	if (result = dev_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(sceneDescHeap_.ReleaseAndGetAddressOf())); FAILED(result)) {
		return result;
	}

	////デスクリプタの先頭ハンドルを取得しておく
	auto heapHandle = sceneDescHeap_->GetCPUDescriptorHandleForHeapStart();

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = sceneConstBuff_->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = static_cast<UINT>(sceneConstBuff_->GetDesc().Width);
	//定数バッファビューの作成
	dev_->CreateConstantBufferView(&cbvDesc, heapHandle);
	return result;
}

void Dx12Wrapper::CreateTextureLoaderTable()
{
	loadLambdaTable_["sph"] = loadLambdaTable_["spa"] = loadLambdaTable_["bmp"] = loadLambdaTable_["png"] = loadLambdaTable_["jpg"] =
		[](const std::wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromWICFile(path.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, meta, img);
	};

	loadLambdaTable_["tga"] = [](const std::wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromTGAFile(path.c_str(), meta, img);
	};
	loadLambdaTable_["dds"] = [](const std::wstring& path, TexMetadata* meta, ScratchImage& img)->HRESULT {
		return LoadFromDDSFile(path.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, meta, img);
	};
}

ComPtr<ID3D12Resource> Dx12Wrapper::CreateTextureFromFile(const char* texpath)
{
	static std::map<std::string, ComPtr<ID3D12Resource>> resourceTable;
	if (auto itr = resourceTable.find(texpath); itr != resourceTable.end()) {
		return itr->second;
	}

	TexMetadata metadata = {};
	ScratchImage scratchImg = {};
	ComPtr<ID3D12Resource> texbuff = nullptr;
	HRESULT result;

	if (auto func = loadLambdaTable_.find(GetExtension(texpath)); func != std::end(loadLambdaTable_)) {
		if (result = (*func).second(GetWideStringFromString(texpath), &metadata, scratchImg); FAILED(result)) {
			return nullptr;
		}
	}
	else {
		return nullptr;
	}	

	{
		auto img = scratchImg.GetImage(0, 0, 0);
		auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0);
		auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(metadata.format, metadata.width, static_cast<UINT>(metadata.height), static_cast<UINT16>(metadata.arraySize), static_cast<UINT16>(metadata.mipLevels));
				
		if (result = dev_->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resDesc, 
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&texbuff));
			FAILED(result)) {
			return nullptr;
		}

		if (result = texbuff->WriteToSubresource(0, nullptr, img->pixels, static_cast<UINT>(img->rowPitch), static_cast<UINT>(img->slicePitch)); FAILED(result)) {
			return nullptr;
		}
	}
	resourceTable[texpath] = texbuff;
	return texbuff;
}
