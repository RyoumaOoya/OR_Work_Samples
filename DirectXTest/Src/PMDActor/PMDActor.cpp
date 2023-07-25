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
		ファイル名から拡張子を取得する
		引数: path 対象のパス文字列
		戻り値: 拡張子
	*/
	std::string GetExtension(const std::string& path)
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
	//マテリアル
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
	//GPUバッファ作成
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
	//マップとコピー	
	if (result = transformBuff_->Map(0, nullptr, (void**)&mappedTransform_); FAILED(result)) {
		assert(SUCCEEDED(result));
		return result;
	}
	*mappedTransform_ = transform_;

	//ビューの作成
	D3D12_DESCRIPTOR_HEAP_DESC transformDescHeapDesc = {};
	transformDescHeapDesc.NumDescriptors = 1;//とりあえずワールドひとつ
	transformDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	transformDescHeapDesc.NodeMask = 0;
	transformDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//デスクリプタヒープ種別
	//生成
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
	//PMDヘッダ構造体
	struct PMDHeader {
		float version; //例：00 00 80 3F == 1.00
		char model_name[20];//モデル名
		char comment[256];//モデルコメント
	};
	char signature[3];
	PMDHeader pmdheader = {};

	std::string strModelPath = path;

	autoClose fp = fopen(strModelPath.c_str(), "rb");
	if (fp == nullptr) {
		//エラー処理
		assert(0);
		return ERROR_FILE_NOT_FOUND;
	}
	fread(signature, sizeof(signature), 1, fp);
	fread(&pmdheader, sizeof(pmdheader), 1, fp);

	unsigned int vertNum;//頂点数
	fread(&vertNum, sizeof(vertNum), 1, fp);


#pragma pack(1)//ここから1バイトパッキング…アライメントは発生しない
	//PMDマテリアル構造体
	struct PMDMaterial {
		XMFLOAT3 diffuse; //ディフューズ色
		float alpha; // ディフューズα
		float specularity;//スペキュラの強さ(乗算値)
		XMFLOAT3 specular; //スペキュラ色
		XMFLOAT3 ambient; //アンビエント色
		unsigned char toonIdx; //トゥーン番号(後述)
		unsigned char edgeFlg;//マテリアル毎の輪郭線フラグ
		unsigned int indicesNum; //このマテリアルが割り当たるインデックス数
		char texFilePath[20]; //テクスチャファイル名(プラスアルファ…後述)
	};//70バイトのはず…でもパディングが発生するため72バイト
#pragma pack()//1バイトパッキング解除

	constexpr unsigned int pmdvertex_size = 38;//頂点1つあたりのサイズ
	std::vector<unsigned char> vertices(vertNum * pmdvertex_size);//バッファ確保
	fread(vertices.data(), vertices.size(), 1, fp);//一気に読み込み

	unsigned int indicesNum;//インデックス数
	fread(&indicesNum, sizeof(indicesNum), 1, fp);//

	//UPLOAD(確保は可能)
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


	vbView_.BufferLocation = vb_->GetGPUVirtualAddress();//バッファの仮想アドレス
	vbView_.SizeInBytes = static_cast<UINT>(vertices.size());//全バイト数
	vbView_.StrideInBytes = pmdvertex_size;//1頂点あたりのバイト数

	std::vector<unsigned short> indices(indicesNum);
	fread(indices.data(), indices.size() * sizeof(indices[0]), 1, fp);//一気に読み込み


	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto heapBuff = CD3DX12_RESOURCE_DESC::Buffer(indices.size() * sizeof(indices[0]));
		if (result = dx12_.Device()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &heapBuff, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(ib_.ReleaseAndGetAddressOf()));
			FAILED(result)) {
			return result;
		}
	}

	//作ったバッファにインデックスデータをコピー
	unsigned short* mappedIdx = nullptr;
	ib_->Map(0, nullptr, (void**)&mappedIdx);
	std::copy(indices.begin(), indices.end(), mappedIdx);
	ib_->Unmap(0, nullptr);


	//インデックスバッファビューを作成
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
	//コピー
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
		//トゥーンリソースの読み込み
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
		if (count(texFileName.begin(), texFileName.end(), '*') > 0) {//スプリッタがある
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
		//モデルとテクスチャパスからアプリケーションからのテクスチャパスを得る
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
