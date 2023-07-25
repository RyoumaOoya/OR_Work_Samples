#include "PMDActor.h"
#include "../PMDRenderer/PMDRenderer.h"
#include "../Dx12Wrapper/Dx12Wrapper.h"
#include <d3dx12.h>

using namespace DirectX;

namespace {
	class autoClose {
	public:
		autoClose(FILE* fp) :fp(fp) {}
		autoClose(_In_z_ char const* _FileName, _In_z_ char const* _Mode) {
			fp = fopen(_FileName, _Mode);
		}
		~autoClose() { fclose(fp); }

		operator bool() const { return fp; }
		operator FILE* () const { return fp; }
	private:
		FILE* fp;
	};
	/*
		���f���̃p�X�ƃe�N�X�`���̃t�@�C���p�X���獇���p�X�𓾂�
		����: modelPath �A�v���P�[�V�������猩��pmd���f���̃p�X, texPath PMD���f�����猩���e�N�X�`���̃p�X
		�߂�l: �A�v���P�[�V�������猩���e�N�X�`���̃p�X
	*/
	std::string GetTexturePathFromModelAndTexPath(const std::string& modelPath, const char* texPath)
	{
		int pathIndex1 = static_cast<int>(modelPath.rfind('/'));
		int pathIndex2 = static_cast<int>(modelPath.rfind('\\'));
		auto folderPath = modelPath.substr(0, max(pathIndex1, pathIndex2) + 1);
		return folderPath + texPath;
	}
	/*
		�t�@�C��������g���q���擾����
		����: path �Ώۂ̃p�X������
		�߂�l: �g���q
	*/
	std::string GetExtension(const std::string& path)
	{
		auto idx = path.rfind('.');
		return path.substr(idx + 1, path.length() - idx - 1);
	}
	/*
		�e�N�X�`���̃p�X���Z�p���[�^�����ŕ�������
		����: path �Ώۂ̃p�X������, splitter ��؂蕶��
		�߂�l: �����O��̕�����y�A
	*/
	std::pair<std::string, std::string> SplitFileName(const std::string& path, const char splitter = '*')
	{
		auto idx = path.find(splitter);

		return std::pair<std::string, std::string>(path.substr(0, idx), path.substr(idx + 1, path.length() - idx - 1));
	}
}

PMDActor::PMDActor(const char* filepath, PMDRenderer& renderer) :renderer_(renderer), dx12_(renderer.dx12_), angle_(0.f)
{
	transform_.world = XMMatrixIdentity();
	LoadPMDFile(filepath);
	CreateTransformView();
	CreateMaterialData();
	CreateMaterialAndTextureView();
}

PMDActor::~PMDActor()
{
}

PMDActor* PMDActor::Clone()
{
    return nullptr;
}

void PMDActor::Update()
{
	angle_ += 0.03f;
	mappedTransform_->world = XMMatrixRotationY(angle_);
}

void PMDActor::Draw()
{
	dx12_.CommandList()->IASetVertexBuffers(0, 1, &vbView_);
	dx12_.CommandList()->IASetIndexBuffer(&ibView_);

	ID3D12DescriptorHeap* transheaps[] = { transformHeap_.Get() };
	dx12_.CommandList()->SetDescriptorHeaps(1, transheaps);
	dx12_.CommandList()->SetGraphicsRootDescriptorTable(1, transformHeap_->GetGPUDescriptorHandleForHeapStart());

	ID3D12DescriptorHeap* mdh[] = { materialHeap_.Get() };
	//�}�e���A��
	dx12_.CommandList()->SetDescriptorHeaps(1, mdh);

	auto materialH = materialHeap_->GetGPUDescriptorHandleForHeapStart();
	unsigned int idxOffset = 0;

	auto cbvsrvIncSize = dx12_.Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 5;
	for (auto& m : materials_) {
		dx12_.CommandList()->SetGraphicsRootDescriptorTable(2, materialH);
		dx12_.CommandList()->DrawIndexedInstanced(m.indicesNum, 1, idxOffset, 0, 0);
		materialH.ptr += cbvsrvIncSize;
		idxOffset += m.indicesNum;
	}
}

HRESULT PMDActor::CreateMaterialData()
{
	constexpr auto materialBuffSize = (sizeof(MaterialForHlsl) + 0xff) & ~0xff;

	auto pro = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto buf = CD3DX12_RESOURCE_DESC::Buffer(materialBuffSize * materials_.size());
	if (auto result = dx12_.Device()->CreateCommittedResource(&pro, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &buf,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(materialBuff_.ReleaseAndGetAddressOf()));
		FAILED(result)) {
		return result;
	}

	char* mapMaterial = nullptr;
	if (auto result = materialBuff_->Map(0, nullptr, (void**)&mapMaterial);
		FAILED(result)) {
		return result;
	}

	for (auto& m : materials_) {
		*((MaterialForHlsl*)mapMaterial) = m.material;
		mapMaterial += materialBuffSize;
	}

	materialBuff_->Unmap(0, nullptr);

	return S_OK;
}

HRESULT PMDActor::CreateMaterialAndTextureView()
{
	D3D12_DESCRIPTOR_HEAP_DESC matDescHeapDesc = {};
	matDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	matDescHeapDesc.NodeMask = 0;
	matDescHeapDesc.NumDescriptors = static_cast<UINT>(materials_.size() * 5);
	matDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	if (auto result = dx12_.Device()->CreateDescriptorHeap(&matDescHeapDesc, IID_PPV_ARGS(materialHeap_.ReleaseAndGetAddressOf()));
		FAILED(result)) {
		return result;
	}

	constexpr auto materialBuffSize = (sizeof(MaterialForHlsl) + 0xff) & ~0xff;
	D3D12_CONSTANT_BUFFER_VIEW_DESC matCBVDesc = {};
	matCBVDesc.BufferLocation = materialBuff_->GetGPUVirtualAddress();
	matCBVDesc.SizeInBytes = materialBuffSize;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	auto matDescHeapH = materialHeap_->GetCPUDescriptorHandleForHeapStart();
	auto incSize = dx12_.Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (u_int i = 0; i < materials_.size(); i++) {
		dx12_.Device()->CreateConstantBufferView(&matCBVDesc, matDescHeapH);
		matDescHeapH.ptr += incSize;
		matCBVDesc.BufferLocation += materialBuffSize;

		auto& texRes = textureResources_[i];
		if (texRes == nullptr) {
			auto whiteTex = renderer_.CreateWhiteTexture();
			srvDesc.Format = whiteTex->GetDesc().Format;
			dx12_.Device()->CreateShaderResourceView(whiteTex, &srvDesc, matDescHeapH);
		}
		else {
			srvDesc.Format = texRes->GetDesc().Format;
			dx12_.Device()->CreateShaderResourceView(texRes.Get(), &srvDesc, matDescHeapH);
		}
		matDescHeapH.ptr += incSize;

		auto& sphRes = sphResources_[i];
		if (sphRes == nullptr) {
			auto whiteTex = renderer_.CreateWhiteTexture();
			srvDesc.Format = whiteTex->GetDesc().Format;
			dx12_.Device()->CreateShaderResourceView(whiteTex, &srvDesc, matDescHeapH);
		}
		else {
			srvDesc.Format = sphRes->GetDesc().Format;
			dx12_.Device()->CreateShaderResourceView(sphRes.Get(), &srvDesc, matDescHeapH);
		}
		matDescHeapH.ptr += incSize;

		auto& spaRes = spaResources_[i];
		if (spaRes == nullptr) {
			auto blackTex = renderer_.CreateBlackTexture();
			srvDesc.Format = blackTex->GetDesc().Format;
			dx12_.Device()->CreateShaderResourceView(blackTex, &srvDesc, matDescHeapH);
		}
		else {
			srvDesc.Format = spaRes->GetDesc().Format;
			dx12_.Device()->CreateShaderResourceView(spaRes.Get(), &srvDesc, matDescHeapH);
		}
		matDescHeapH.ptr += incSize;

		auto& toonRes = toonResources_[i];
		if (!toonRes) {
			auto gradTex = renderer_.CreateGrayGradationTexture();
			srvDesc.Format = gradTex->GetDesc().Format;
			dx12_.Device()->CreateShaderResourceView(gradTex, &srvDesc, matDescHeapH);
		}
		else {
			srvDesc.Format = toonRes->GetDesc().Format;
			dx12_.Device()->CreateShaderResourceView(toonRes.Get(), &srvDesc, matDescHeapH);
		}
		matDescHeapH.ptr += incSize;
	}
    return S_OK;
}

HRESULT PMDActor::CreateTransformView()
{
	HRESULT result;
	//GPU�o�b�t�@�쐬
	constexpr auto buffSize = static_cast<UINT>(sizeof(Transform) + 0xff) & ~0xff;
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto heapBuff = CD3DX12_RESOURCE_DESC::Buffer(buffSize);

		if (result = dx12_.Device()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &heapBuff, 
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(transformBuff_.ReleaseAndGetAddressOf()));
			FAILED(result)) {
			assert(SUCCEEDED(result));
			return result;
		}
	}
	//�}�b�v�ƃR�s�[	
	if (result = transformBuff_->Map(0, nullptr, (void**)&mappedTransform_); FAILED(result)) {
		assert(SUCCEEDED(result));
		return result;
	}
	*mappedTransform_ = transform_;

	//�r���[�̍쐬
	D3D12_DESCRIPTOR_HEAP_DESC transformDescHeapDesc = {};
	transformDescHeapDesc.NumDescriptors = 1;//�Ƃ肠�������[���h�ЂƂ�
	transformDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	transformDescHeapDesc.NodeMask = 0;
	transformDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//�f�X�N���v�^�q�[�v���
	//����
	if (result = dx12_.Device()->CreateDescriptorHeap(&transformDescHeapDesc, IID_PPV_ARGS(transformHeap_.ReleaseAndGetAddressOf())); FAILED(result)) {
		assert(SUCCEEDED(result));
		return result;
	}

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = transformBuff_->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = buffSize;
	dx12_.Device()->CreateConstantBufferView(&cbvDesc, transformHeap_->GetCPUDescriptorHandleForHeapStart());

	return S_OK;
}

HRESULT PMDActor::LoadPMDFile(const char* path)
{
	HRESULT result;
	//PMD�w�b�_�\����
	struct PMDHeader {
		float version; //��F00 00 80 3F == 1.00
		char model_name[20];//���f����
		char comment[256];//���f���R�����g
	};
	char signature[3];
	PMDHeader pmdheader = {};

	std::string strModelPath = path;

	autoClose fp = fopen(strModelPath.c_str(), "rb");
	if (fp == nullptr) {
		//�G���[����
		assert(0);
		return ERROR_FILE_NOT_FOUND;
	}
	fread(signature, sizeof(signature), 1, fp);
	fread(&pmdheader, sizeof(pmdheader), 1, fp);

	unsigned int vertNum;//���_��
	fread(&vertNum, sizeof(vertNum), 1, fp);


#pragma pack(1)//��������1�o�C�g�p�b�L���O�c�A���C�����g�͔������Ȃ�
	//PMD�}�e���A���\����
	struct PMDMaterial {
		XMFLOAT3 diffuse; //�f�B�t���[�Y�F
		float alpha; // �f�B�t���[�Y��
		float specularity;//�X�y�L�����̋���(��Z�l)
		XMFLOAT3 specular; //�X�y�L�����F
		XMFLOAT3 ambient; //�A���r�G���g�F
		unsigned char toonIdx; //�g�D�[���ԍ�(��q)
		unsigned char edgeFlg;//�}�e���A�����̗֊s���t���O
		unsigned int indicesNum; //���̃}�e���A�������蓖����C���f�b�N�X��
		char texFilePath[20]; //�e�N�X�`���t�@�C����(�v���X�A���t�@�c��q)
	};//70�o�C�g�̂͂��c�ł��p�f�B���O���������邽��72�o�C�g
#pragma pack()//1�o�C�g�p�b�L���O����

	constexpr unsigned int pmdvertex_size = 38;//���_1������̃T�C�Y
	std::vector<unsigned char> vertices(vertNum * pmdvertex_size);//�o�b�t�@�m��
	fread(vertices.data(), vertices.size(), 1, fp);//��C�ɓǂݍ���

	unsigned int indicesNum;//�C���f�b�N�X��
	fread(&indicesNum, sizeof(indicesNum), 1, fp);//

	//UPLOAD(�m�ۂ͉\)
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto heapBuff = CD3DX12_RESOURCE_DESC::Buffer(vertices.size());

		if (result = dx12_.Device()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &heapBuff, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(vb_.ReleaseAndGetAddressOf()));
			FAILED(result)) {
			return result;
		}
	}
	

	unsigned char* vertMap = nullptr;
	result = vb_->Map(0, nullptr, (void**)&vertMap);
	std::copy(vertices.begin(), vertices.end(), vertMap);
	vb_->Unmap(0, nullptr);


	vbView_.BufferLocation = vb_->GetGPUVirtualAddress();//�o�b�t�@�̉��z�A�h���X
	vbView_.SizeInBytes = static_cast<UINT>(vertices.size());//�S�o�C�g��
	vbView_.StrideInBytes = pmdvertex_size;//1���_������̃o�C�g��

	std::vector<unsigned short> indices(indicesNum);
	fread(indices.data(), indices.size() * sizeof(indices[0]), 1, fp);//��C�ɓǂݍ���


	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto heapBuff = CD3DX12_RESOURCE_DESC::Buffer(indices.size() * sizeof(indices[0]));
		if (result = dx12_.Device()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &heapBuff, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(ib_.ReleaseAndGetAddressOf()));
			FAILED(result)) {
			return result;
		}
	}

	//������o�b�t�@�ɃC���f�b�N�X�f�[�^���R�s�[
	unsigned short* mappedIdx = nullptr;
	ib_->Map(0, nullptr, (void**)&mappedIdx);
	std::copy(indices.begin(), indices.end(), mappedIdx);
	ib_->Unmap(0, nullptr);


	//�C���f�b�N�X�o�b�t�@�r���[���쐬
	ibView_.BufferLocation = ib_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = static_cast<UINT>(indices.size() * sizeof(indices[0]));

	unsigned int materialNum;
	fread(&materialNum, sizeof(materialNum), 1, fp);
	materials_.resize(materialNum);
	textureResources_.resize(materialNum);
	sphResources_.resize(materialNum);
	spaResources_.resize(materialNum);
	toonResources_.resize(materialNum);

	std::vector<PMDMaterial> pmdMaterials(materialNum);
	fread(pmdMaterials.data(), pmdMaterials.size() * sizeof(PMDMaterial), 1, fp);
	//�R�s�[
	for (int i = 0; i < pmdMaterials.size(); ++i) {
		materials_[i].indicesNum = pmdMaterials[i].indicesNum;
		materials_[i].material.diffuse = pmdMaterials[i].diffuse;
		materials_[i].material.alpha = pmdMaterials[i].alpha;
		materials_[i].material.specular = pmdMaterials[i].specular;
		materials_[i].material.specularity = pmdMaterials[i].specularity;
		materials_[i].material.ambient = pmdMaterials[i].ambient;
		materials_[i].additional.toonIdx = pmdMaterials[i].toonIdx;
	}

	for (int i = 0; i < pmdMaterials.size(); ++i) {
		//�g�D�[�����\�[�X�̓ǂݍ���
		char toonFilePath[32];
		sprintf(toonFilePath, "toon/toon%02d.bmp", pmdMaterials[i].toonIdx + 1);
		toonResources_[i] = dx12_.GetTextureByPath(toonFilePath);

		if (strlen(pmdMaterials[i].texFilePath) == 0) {
			textureResources_[i] = nullptr;
			continue;
		}

		std::string texFileName = pmdMaterials[i].texFilePath;
		std::string sphFileName = "";
		std::string spaFileName = "";
		if (count(texFileName.begin(), texFileName.end(), '*') > 0) {//�X�v���b�^������
			auto namepair = SplitFileName(texFileName);
			if (GetExtension(namepair.first) == "sph") {
				texFileName = namepair.second;
				sphFileName = namepair.first;
			}
			else if (GetExtension(namepair.first) == "spa") {
				texFileName = namepair.second;
				spaFileName = namepair.first;
			}
			else {
				texFileName = namepair.first;
				if (GetExtension(namepair.second) == "sph") {
					sphFileName = namepair.second;
				}
				else if (GetExtension(namepair.second) == "spa") {
					spaFileName = namepair.second;
				}
			}
		}
		else {
			if (GetExtension(pmdMaterials[i].texFilePath) == "sph") {
				sphFileName = pmdMaterials[i].texFilePath;
				texFileName = "";
			}
			else if (GetExtension(pmdMaterials[i].texFilePath) == "spa") {
				spaFileName = pmdMaterials[i].texFilePath;
				texFileName = "";
			}
			else {
				texFileName = pmdMaterials[i].texFilePath;
			}
		}
		//���f���ƃe�N�X�`���p�X����A�v���P�[�V��������̃e�N�X�`���p�X�𓾂�
		if (texFileName != "") {
			auto texFilePath = GetTexturePathFromModelAndTexPath(strModelPath, texFileName.c_str());
			textureResources_[i] = dx12_.GetTextureByPath(texFilePath.c_str());
		}
		if (sphFileName != "") {
			auto sphFilePath = GetTexturePathFromModelAndTexPath(strModelPath, sphFileName.c_str());
			sphResources_[i] = dx12_.GetTextureByPath(sphFilePath.c_str());
		}
		if (spaFileName != "") {
			auto spaFilePath = GetTexturePathFromModelAndTexPath(strModelPath, spaFileName.c_str());
			spaResources_[i] = dx12_.GetTextureByPath(spaFilePath.c_str());
		}
	}

	return result;
}
