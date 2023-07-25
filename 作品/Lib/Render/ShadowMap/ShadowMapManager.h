#pragma once
#include "../../Define.h"
#include "../../Math/MyMath.h"
#include "ShadowMap.h"
#include "../../GameSystem/Allocator/Allocator.h"

namespace MyLib
{
	class ShadowMapManager {
		SINGLETON(ShadowMapManager);
	private:
		ShadowMapManager();
		~ShadowMapManager();
	public:
		/*
			シャドウマップを作成する
			引数: data シャドウマップのデータ
			戻り値: -1 = エラー, 0 ～ 2 = シャドウマップのインデックス
		*/
		int CreateShadowMap(const ShadowMap::ShadowMapData& data);
		Pointer<ShadowMap> GetShadowMap(_In_range_(0, 2)int index);

		void Delete();
		void Fin();
	private:
		Pointer<ShadowMap> ShadowMap_[3];
	};
}
namespace Size {
	constexpr size_t SHADOWMAP_MANAGER = sizeof(MyLib::ShadowMapManager);
}