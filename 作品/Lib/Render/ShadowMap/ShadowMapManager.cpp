#include "ShadowMapManager.h"
#include "DxLib.h"

namespace MyLib
{
	ShadowMapManager::ShadowMapManager()
	{
	}

	ShadowMapManager::~ShadowMapManager()
	{
		Fin();
	}

	int ShadowMapManager::CreateShadowMap(const ShadowMap::ShadowMapData& data)
	{
		for (int i = 0; i < 3; i++) {
			if (ShadowMap_[i]) continue;
			ShadowMap_[i] = MyNew(ShadowMap, AllocType::Library, Tags(MemTag::Library), i, data);
			return i;
		}

		return -1;
	}

	Pointer<ShadowMap> ShadowMapManager::GetShadowMap(_In_range_(0, 2)int index)
	{
		return ShadowMap_[index];
	}

	void ShadowMapManager::Delete()
	{
		for (int i = 0; i < 3; i++) {
			if (!ShadowMap_[i]) continue;

			MyFree(ShadowMap_[i]);
		}
	}

	void ShadowMapManager::Fin()
	{
		Delete();
	}
}