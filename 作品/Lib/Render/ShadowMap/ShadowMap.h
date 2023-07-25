#pragma once
#include "../../Math/MyMathData.h"
#include "../../GameSystem/Allocator/Pointer.h"

namespace MyLib
{
	//DxLibのシャドウマップのラッパー
	class ShadowMap
	{
		friend class ShadowMapManager;
	private:
		static Vector GlobalLight;
	public:
		class ShadowMapData {
		public:
			constexpr ShadowMapData(iVector2D mapSize, Vector minArea, Vector maxArea, Vector lightDirection = Vector::ZEROVEC())
				:MapSize(mapSize), MinArea(minArea), MaxArea(maxArea), LightDirection(lightDirection) {}
			iVector2D MapSize;		//解像度
			Vector MinArea;			//範囲(始点)
			Vector MaxArea;			//範囲(終点)
			Vector LightDirection;	//ディレクショナルライト

		private:
			char buffer[4] = { 0 };
		};
	public:
		ShadowMap();
		ShadowMap(int index, const ShadowMapData& data);
		~ShadowMap();

		void Init();

		//影の書き込みを開始
		void ShadowWriteStart();
		//影の書き込みを終了
		void ShadowWriteFin();

		//影付き描画の開始
		void ShadowDrawStart();
		//影付き描画の終了
		void ShadowDrawFin();

		void Delete();
		void Fin();

		/*
			範囲設定
			引数: minArea 始点, maxArea 終点
		*/
		void SetShadowMapDrawArea(const Vector& minArea, const Vector& maxArea);
		//ディレクショナルライトの設定
		void SetLightDirection(const Vector& direction);
	private:
		int Handle_;
		int Index_;
	};

	/*
		コンストラクタでシャドウマップの書き込みを開始、
		デストラクタで確実にシャドウマップの書き込みを終了するクラス
	*/
	class ScopedShadowWrite {
	public:
		ScopedShadowWrite(const Pointer<ShadowMap>& shadowMap) :shadowMap(shadowMap) { Start(); }
		~ScopedShadowWrite() { Fin(); }

		void Start() { shadowMap->ShadowWriteStart(); }
		void Fin() { shadowMap->ShadowWriteFin(); }
	private:
		Pointer<ShadowMap> shadowMap;
	};

	/*
		コンストラクタで影付き描画を開始、
		デストラクタで確実に影付き描画を終了するクラス
	*/
	class ScopedShadowDraw {
	public:
		ScopedShadowDraw(const Pointer<ShadowMap>& shadowMap) :shadowMap(shadowMap) { Start(); }
		~ScopedShadowDraw() { Fin(); }

		void Start() { shadowMap->ShadowDrawStart(); }
		void Fin() { shadowMap->ShadowDrawFin(); }
	private:
		Pointer<ShadowMap> shadowMap;
	};
}

/*
	ShadowMap メモリイメージ

	ShadowMapData{
		MapSize			8byte
		MinArea			24byte
		MaxArea			24byte
		LightDirection	24byte
	}

	handle_	4byte	this_index_ 4byte
*/

namespace Size {
	constexpr size_t SHADOWMAP = sizeof(MyLib::ShadowMap);
	constexpr size_t SHADOWMAP_DATA = sizeof(MyLib::ShadowMap::ShadowMapData);
}