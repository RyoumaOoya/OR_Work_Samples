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
	ID3D12Resource* CreateWhiteTexture();//���e�N�X�`���̐���
	ID3D12Resource* CreateBlackTexture();//���e�N�X�`���̐���
	ID3D12Resource* CreateGrayGradationTexture();//�O���[�e�N�X�`���̐���

	//�p�C�v���C��������
	HRESULT CreateGraphicsPipelineForPMD();
	//���[�g�V�O�l�`��������
	HRESULT CreateRootSignature();

	bool CheckShaderCompileResult(HRESULT result, ID3DBlob* error = nullptr);
private:
	Dx12Wrapper& dx12_;

	ComPtr<ID3D12PipelineState> pipeline_ = nullptr;		//PMD�p�p�C�v���C��
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;	//PMD�p���[�g�V�O�l�`��
};