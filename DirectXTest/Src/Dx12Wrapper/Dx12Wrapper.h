#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <map>
#include <unordered_map>
#include <DirectXTex.h>
#include <wrl.h>
#include <string>
#include <functional>
#include <memory>
#include "../Color.h"

class Dx12Wrapper
{
	using LoadLambda_t = std::function<HRESULT(const std::wstring& path, DirectX::TexMetadata*, DirectX::ScratchImage&)>;
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	struct SceneData {
		DirectX::XMMATRIX view;//�r���[�s��
		DirectX::XMMATRIX proj;//�v���W�F�N�V�����s��
		DirectX::XMFLOAT3 eye; //���_���W
	};
public:
	Dx12Wrapper(HWND hwnd);
	~Dx12Wrapper();
public:
	void Update();
	void BeginDraw();
	void EndDraw();
	///�e�N�X�`���p�X����K�v�ȃe�N�X�`���o�b�t�@�ւ̃|�C���^��Ԃ�
	///@param texpath �e�N�X�`���t�@�C���p�X
	ComPtr<ID3D12Resource> GetTextureByPath(const char* texpath);

	ComPtr<ID3D12Device>				Device();		//�f�o�C�X
	ComPtr<ID3D12GraphicsCommandList>	CommandList();	//�R�}���h���X�g
	ComPtr<IDXGISwapChain4>				Swapchain();	//�X���b�v�`�F�C��
	void SetScene();
private:
	//�ŏI�I�ȃ����_�[�^�[�Q�b�g�̐���
	HRESULT	CreateFinalRenderTargets();
	//�f�v�X�X�e���V���r���[�̐���
	HRESULT CreateDepthStencilView();
	//�X���b�v�`�F�C���̐���
	HRESULT CreateSwapChain(const HWND& hwnd);
	//DXGI�܂�菉����
	HRESULT InitializeDXGIDevice();
	//�R�}���h�܂�菉����
	HRESULT InitializeCommand();
	//�r���[�v���W�F�N�V�����p�r���[�̐���
	HRESULT CreateSceneView();

	//�e�N�X�`�����[�_�e�[�u���̍쐬
	void CreateTextureLoaderTable();
	//�e�N�X�`��������e�N�X�`���o�b�t�@�쐬�A���g���R�s�[
	ComPtr<ID3D12Resource> CreateTextureFromFile(const char* texpath);
private:
	COLOR_F backColor;

	//���[�h�p�e�[�u��
	std::map<std::string, LoadLambda_t> loadLambdaTable_;
	//�e�N�X�`���e�[�u��
	std::unordered_map<std::string, ComPtr<ID3D12Resource>> textureTable_;

	SIZE winSize_;
	//DXGI�܂��
	ComPtr<IDXGIFactory4>	dxgiFactory_ = nullptr;	//DXGI�C���^�[�t�F�C�X
	ComPtr<IDXGISwapChain4> swapchain_ = nullptr;	//�X���b�v�`�F�C��

	//DirectX12�܂��
	ComPtr<ID3D12Device>				dev_ = nullptr;			//�f�o�C�X
	ComPtr<ID3D12CommandAllocator>		cmdAllocator_ = nullptr;//�R�}���h�A���P�[�^
	ComPtr<ID3D12GraphicsCommandList>	cmdList_ = nullptr;		//�R�}���h���X�g
	ComPtr<ID3D12CommandQueue>			cmdQueue_ = nullptr;	//�R�}���h�L���[

	//�\���Ɋւ��o�b�t�@����
	ComPtr<ID3D12Resource>			depthBuffer_ = nullptr;	//�[�x�o�b�t�@
	std::vector<ID3D12Resource*>	backBuffers_;			//�o�b�N�o�b�t�@(2�ȏ�c�X���b�v�`�F�C�����m��)
	ComPtr<ID3D12DescriptorHeap>	rtvHeaps_ = nullptr;	//�����_�[�^�[�Q�b�g�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>	dsvHeap_ = nullptr;		//�[�x�o�b�t�@�r���[�p�f�X�N���v�^�q�[�v
	std::unique_ptr<D3D12_VIEWPORT>	viewport_;				//�r���[�|�[�g
	std::unique_ptr<D3D12_RECT>		scissorrect_;			//�V�U�[��`

	//�V�[�����\������o�b�t�@�܂��
	ComPtr<ID3D12Resource>		sceneConstBuff_ = nullptr;
	SceneData*					mappedSceneData_ = nullptr;
	ComPtr<ID3D12DescriptorHeap>sceneDescHeap_ = nullptr;

	//�t�F���X
	ComPtr<ID3D12Fence> fence_ = nullptr;
	UINT64				fenceVal_ = 0;
};