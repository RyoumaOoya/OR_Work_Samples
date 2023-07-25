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
			�V���h�E�}�b�v���쐬����
			����: data �V���h�E�}�b�v�̃f�[�^
			�߂�l: -1 = �G���[, 0 �` 2 = �V���h�E�}�b�v�̃C���f�b�N�X
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