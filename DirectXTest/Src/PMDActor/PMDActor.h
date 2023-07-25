#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include <string>

class Dx12Wrapper;
class PMDRenderer;
class PMDActor
{
	friend PMDRenderer;
private:
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	//�V�F�[�_���ɓ�������}�e���A���f�[�^
	struct MaterialForHlsl {
		DirectX::XMFLOAT3	diffuse;	//�f�B�t���[�Y�F
		float				alpha;		// �f�B�t���[�Y��
		DirectX::XMFLOAT3	specular;	//�X�y�L�����F
		float				specularity;//�X�y�L�����̋���(��Z�l)
		DirectX::XMFLOAT3	ambient;	//�A���r�G���g�F
	};
	//����ȊO�̃}�e���A���f�[�^
	struct AdditionalMaterial {
		std::string texPath; //�e�N�X�`���t�@�C���p�X
		int			toonIdx; //�g�D�[���ԍ�
		bool		edgeFlg; //�}�e���A�����̗֊s���t���O
	};
	//�܂Ƃ߂�����
	struct Material {
		unsigned int		indicesNum;//�C���f�b�N�X��
		MaterialForHlsl		material;
		AdditionalMaterial	additional;
	};

	struct Transform {
		//�����Ɏ����Ă�XMMATRIX�����o��16�o�C�g�A���C�����g�ł��邽��
		//Transform��new����ۂɂ�16�o�C�g���E�Ɋm�ۂ���
		//void* operator new(size_t size);
		DirectX::XMMATRIX world;
	};

public:
	PMDActor(const char* filepath, PMDRenderer& renderer);
	~PMDActor();
	///�N���[���͒��_����у}�e���A���͋��ʂ̃o�b�t�@������悤�ɂ���
	PMDActor* Clone();
	void Update();
	void Draw();

private:
	//�ǂݍ��񂾃}�e���A�������ƂɃ}�e���A���o�b�t�@���쐬
	HRESULT CreateMaterialData();
	//�}�e���A�����e�N�X�`���̃r���[���쐬
	HRESULT CreateMaterialAndTextureView();
	//���W�ϊ��p�r���[�̐���
	HRESULT CreateTransformView();
	//PMD�t�@�C���̃��[�h
	HRESULT LoadPMDFile(const char* path);
private:
	PMDRenderer& renderer_;
	Dx12Wrapper& dx12_;

	ComPtr<ID3D12DescriptorHeap> materialHeap_ = nullptr;//�}�e���A���q�[�v(5�Ԃ�)

	//���_�֘A
	ComPtr<ID3D12Resource>		vb_ = nullptr;
	ComPtr<ID3D12Resource>		ib_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW	vbView_ = {};
	D3D12_INDEX_BUFFER_VIEW		ibView_ = {};

	ComPtr<ID3D12Resource>		transformMat_ = nullptr;	//���W�ϊ��s��(���̓��[���h�̂�)
	ComPtr<ID3D12DescriptorHeap>transformHeap_ = nullptr;	//���W�ϊ��q�[�v

	Transform				transform_;
	Transform*				mappedTransform_ = nullptr;
	ComPtr<ID3D12Resource>	transformBuff_ = nullptr;

	//�}�e���A���֘A
	std::vector<Material>				materials_;
	ComPtr<ID3D12Resource>				materialBuff_ = nullptr;
	std::vector<ComPtr<ID3D12Resource>> textureResources_;
	std::vector<ComPtr<ID3D12Resource>> sphResources_;
	std::vector<ComPtr<ID3D12Resource>> spaResources_;
	std::vector<ComPtr<ID3D12Resource>> toonResources_;
	float								angle_;//�e�X�g�pY����]
};