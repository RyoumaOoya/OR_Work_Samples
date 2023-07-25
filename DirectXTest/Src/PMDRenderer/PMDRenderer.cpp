#include "PMDRenderer.h"
#include <d3dx12.h>
#include <cassert>
#include <d3dcompiler.h>
#include "../Dx12Wrapper/Dx12Wrapper.h"
#include <string>
#include <algorithm>

namespace {
	void PrintErrorBlob(ID3DBlob* blob) {
		assert(blob);
		std::string err;
		err.resize(blob->GetBufferSize());
		std::copy_n((char*)blob->GetBufferPointer(), err.size(), err.begin());
	}
}

PMDRenderer::PMDRenderer(Dx12Wrapper& dx12) :dx12_(dx12)
{
	assert(SUCCEEDED(CreateRootSignature()));
	assert(SUCCEEDED(CreateGraphicsPipelineForPMD()));

	CreateWhiteTexture();
	CreateBlackTexture();
	CreateGrayGradationTexture();
}

PMDRenderer::~PMDRenderer()
{
}

void PMDRenderer::Update()
{
}

void PMDRenderer::Draw()
{
}

ID3D12PipelineState* PMDRenderer::GetPipelineState()
{
	return pipeline_.Get();
}

ID3D12RootSignature* PMDRenderer::GetRootSignature()
{
	return rootSignature_.Get();
}

ID3D12Resource* PMDRenderer::CreateDefaultTexture(size_t width, size_t height)
{
	auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	ID3D12Resource* buff = nullptr;
	auto result = dx12_.Device()->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&buff));
	if (FAILED(result)) {
		assert(SUCCEEDED(result));
		return nullptr;
	}
	return buff;
}

ID3D12Resource* PMDRenderer::CreateWhiteTexture()
{
	static ComPtr<ID3D12Resource> tex = nullptr;
	if (tex) {
		return tex.Get();
	}

	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0);
	auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 4, 4);

	ID3D12Resource* whiteBuff = nullptr;
	if (FAILED(dx12_.Device()->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resDesc, 
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&whiteBuff)))) {
		return nullptr;
	}
	{
		std::vector<u_char> data(4 * 4 * 4);
		std::fill(data.begin(), data.end(), 0xff);

		if (FAILED(whiteBuff->WriteToSubresource(0, nullptr, data.data(), 4 * 4, static_cast<UINT>(data.size())))) {
			whiteBuff->Release();
			return nullptr;
		}
	}

	tex = whiteBuff;
	return whiteBuff;
}

ID3D12Resource* PMDRenderer::CreateBlackTexture()
{
	static ComPtr<ID3D12Resource> tex = nullptr;
	if (tex) {
		return tex.Get();
	}

	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0);
	auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 4, 4);

	ID3D12Resource* blackBuff = nullptr;
	if (FAILED(dx12_.Device()->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resDesc, 
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&blackBuff)))) {
		return nullptr;
	}
	{
		std::vector<u_char> data(4 * 4 * 4);
		std::fill(data.begin(), data.end(), 0x00);

		if (FAILED(blackBuff->WriteToSubresource(0, nullptr, data.data(), 4 * 4, static_cast<UINT>(data.size())))) {
			blackBuff->Release();
			return nullptr;
		}
	}

	tex = blackBuff;
	return blackBuff;
}

ID3D12Resource* PMDRenderer::CreateGrayGradationTexture()
{
	static ComPtr<ID3D12Resource> tex = nullptr;
	if (tex) {
		return tex.Get();
	}

	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0);
	auto resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 3, 256);

	ID3D12Resource* gradBuff = nullptr;
	if (FAILED(dx12_.Device()->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resDesc, 
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(&gradBuff)))) {
		return nullptr;
	}
	{
		std::vector<u_int> data(4 * 256);
		auto itr = data.begin();
		u_int c = 0xff;
		for (; itr != data.end(); itr += 4) {
			auto col = (c << 24) | RGB(c, c, c);
			std::fill(itr, itr + 4, col);
			--c;
		}

		if (FAILED(gradBuff->WriteToSubresource(0, nullptr, data.data(), static_cast<UINT>(4 * sizeof(u_int)), static_cast<UINT>(sizeof(u_int) * data.size())))) {
			gradBuff->Release();
			return nullptr;
		}
	}

	tex = gradBuff;
	return gradBuff;
}

HRESULT PMDRenderer::CreateGraphicsPipelineForPMD()
{
	ComPtr<ID3DBlob> vsBlob = nullptr;
	ComPtr<ID3DBlob> psBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT result;

	if (result = D3DCompileFromFile(L"Shader/VertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"BasicVS", "vs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, vsBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
		!CheckShaderCompileResult(result, errorBlob.Get())) {
		assert(0);
		return result;
	}

	//コンパイル定数:https://learn.microsoft.com/ja-jp/windows/win32/direct3dhlsl/d3dcompile-constants
	if (result = D3DCompileFromFile(L"Shader/PixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"BasicPS", "ps_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, psBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf()); 
		!CheckShaderCompileResult(result, errorBlob.Get())) {
		assert(0);
		return result;
	}

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		/*
			LPCSTR SemanticName; UINT SemanticIndex; DXGI_FORMAT Format; UINT InputSlot;
			UINT AlignedByteOffset; D3D12_INPUT_CLASSIFICATION InputSlotClass; UINT InstanceDataStepRate;
		*/
		//HLSLセマンティック インデックス	要素データのフォーマット		入力スロット	オフセット(自動定義)			識別値							
		{"POSITION",			0,	DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,	0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL",				0,	DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,	0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD",			0,	DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,		0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"BONE_NO",				0,	DXGI_FORMAT::DXGI_FORMAT_R16G16_UINT,		0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"WEIGHT",				0,	DXGI_FORMAT::DXGI_FORMAT_R8_UINT,			0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"EDGE_FLG",			0,	DXGI_FORMAT::DXGI_FORMAT_R8_UINT,			0,		D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	ComPtr<ID3DBlob> rootSigBlob = nullptr;
#pragma region SetGraphicsPipelineStateDesc
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	gpipeline.pRootSignature = rootSignature_.Get();

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get()); //頂点シェーダ設定	
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get()); //ピクセルシェーダ設定

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpipeline.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;

	gpipeline.DepthStencilState.DepthEnable = true;	//深度Bufferを使う
	gpipeline.DepthStencilState.StencilEnable = false;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK::D3D12_DEPTH_WRITE_MASK_ALL;//
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	//小さいほうを採用
	gpipeline.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//カットなし
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

	gpipeline.SampleDesc.Count = 1;
	gpipeline.SampleDesc.Quality = 0;
#pragma endregion	
	if (result = dx12_.Device()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipeline_.ReleaseAndGetAddressOf())); FAILED(result)) {
		assert(SUCCEEDED(result));
	}

	return result;
}

HRESULT PMDRenderer::CreateRootSignature()
{
	HRESULT result;
	CD3DX12_DESCRIPTOR_RANGE descTblRange[4] = {};
	descTblRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	//ビュープロジェクション用	
	descTblRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);	//ワールド、ボーン用	
	descTblRange[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);	//マテリアル用
	descTblRange[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 0);	//テクスチャ用

	CD3DX12_ROOT_PARAMETER rootparam[3] = {};
	rootparam[0].InitAsDescriptorTable(1, &descTblRange[0]);
	rootparam[1].InitAsDescriptorTable(1, &descTblRange[1]);
	rootparam[2].InitAsDescriptorTable(2, &descTblRange[2]);

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc[2] = {};
	samplerDesc[0].Init(0);
	samplerDesc[1].Init(1, D3D12_FILTER::D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init(3, rootparam, 2, samplerDesc, D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	if (result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, rootSigBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf()); 
		FAILED(result)) {
		return result;
	}
	if (result = dx12_.Device()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature_.ReleaseAndGetAddressOf()));
		FAILED(result)) {
		return result;
	}
	return result;
}

bool PMDRenderer::CheckShaderCompileResult(HRESULT result, ID3DBlob* error)
{
	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
			::OutputDebugStringA("ファイルが見当たりません");
		}
		else {
			std::string errstr;
			errstr.resize(error->GetBufferSize());
			std::copy_n((char*)error->GetBufferPointer(), error->GetBufferSize(), errstr.begin());
			errstr += "\n";
			OutputDebugStringA(errstr.c_str());
		}
		return false;
	}
	else {
		return true;
	}
}
