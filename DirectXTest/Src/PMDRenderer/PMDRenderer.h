#pragma once
#include <d3d12.h>
#include <vector>
#include <wrl.h>
#include <memory>

class Dx12Wrapper;
class PMDActor;
class PMDRenderer
{
	friend PMDActor;
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	PMDRenderer(Dx12Wrapper& dx12);
	~PMDRenderer();
	void Update();
	void Draw();
	ID3D12PipelineState* GetPipelineState();
	ID3D12RootSignature* GetRootSignature();
private:
	ID3D12Resource* CreateDefaultTexture(size_t width, size_t height);
	ID3D12Resource* CreateWhiteTexture();//白テクスチャの生成
	ID3D12Resource* CreateBlackTexture();//黒テクスチャの生成
	ID3D12Resource* CreateGrayGradationTexture();//グレーテクスチャの生成

	//パイプライン初期化
	HRESULT CreateGraphicsPipelineForPMD();
	//ルートシグネチャ初期化
	HRESULT CreateRootSignature();

	bool CheckShaderCompileResult(HRESULT result, ID3DBlob* error = nullptr);
private:
	Dx12Wrapper& dx12_;

	ComPtr<ID3D12PipelineState> pipeline_ = nullptr;		//PMD用パイプライン
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;	//PMD用ルートシグネチャ
};