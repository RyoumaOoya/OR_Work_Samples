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
	//シェーダ側に投げられるマテリアルデータ
	struct MaterialForHlsl {
		DirectX::XMFLOAT3	diffuse;	//ディフューズ色
		float				alpha;		// ディフューズα
		DirectX::XMFLOAT3	specular;	//スペキュラ色
		float				specularity;//スペキュラの強さ(乗算値)
		DirectX::XMFLOAT3	ambient;	//アンビエント色
	};
	//それ以外のマテリアルデータ
	struct AdditionalMaterial {
		std::string texPath; //テクスチャファイルパス
		int			toonIdx; //トゥーン番号
		bool		edgeFlg; //マテリアル毎の輪郭線フラグ
	};
	//まとめたもの
	struct Material {
		unsigned int		indicesNum;//インデックス数
		MaterialForHlsl		material;
		AdditionalMaterial	additional;
	};

	struct Transform {
		//内部に持ってるXMMATRIXメンバが16バイトアライメントであるため
		//Transformをnewする際には16バイト境界に確保する
		//void* operator new(size_t size);
		DirectX::XMMATRIX world;
	};

public:
	PMDActor(const char* filepath, PMDRenderer& renderer);
	~PMDActor();
	///クローンは頂点およびマテリアルは共通のバッファを見るようにする
	PMDActor* Clone();
	void Update();
	void Draw();

private:
	//読み込んだマテリアルをもとにマテリアルバッファを作成
	HRESULT CreateMaterialData();
	//マテリアル＆テクスチャのビューを作成
	HRESULT CreateMaterialAndTextureView();
	//座標変換用ビューの生成
	HRESULT CreateTransformView();
	//PMDファイルのロード
	HRESULT LoadPMDFile(const char* path);
private:
	PMDRenderer& renderer_;
	Dx12Wrapper& dx12_;

	ComPtr<ID3D12DescriptorHeap> materialHeap_ = nullptr;//マテリアルヒープ(5個ぶん)

	//頂点関連
	ComPtr<ID3D12Resource>		vb_ = nullptr;
	ComPtr<ID3D12Resource>		ib_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW	vbView_ = {};
	D3D12_INDEX_BUFFER_VIEW		ibView_ = {};

	ComPtr<ID3D12Resource>		transformMat_ = nullptr;	//座標変換行列(今はワールドのみ)
	ComPtr<ID3D12DescriptorHeap>transformHeap_ = nullptr;	//座標変換ヒープ

	Transform				transform_;
	Transform*				mappedTransform_ = nullptr;
	ComPtr<ID3D12Resource>	transformBuff_ = nullptr;

	//マテリアル関連
	std::vector<Material>				materials_;
	ComPtr<ID3D12Resource>				materialBuff_ = nullptr;
	std::vector<ComPtr<ID3D12Resource>> textureResources_;
	std::vector<ComPtr<ID3D12Resource>> sphResources_;
	std::vector<ComPtr<ID3D12Resource>> spaResources_;
	std::vector<ComPtr<ID3D12Resource>> toonResources_;
	float								angle_;//テスト用Y軸回転
};