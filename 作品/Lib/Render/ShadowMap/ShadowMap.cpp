#include "ShadowMap.h"

namespace MyLib
{
	Vector ShadowMap::GlobalLight(0.001f, -1.f, 0.f);

	ShadowMap::ShadowMap()
	{
		Handle_ = -1;
		Index_ = -1;
	}

	ShadowMap::ShadowMap(int index, const ShadowMapData& data)
	{
		Index_ = index;
		// シャドウマップハンドルの作成
		Handle_ = MakeShadowMap(data.MapSize.x, data.MapSize.y);

		// シャドウマップが想定するライトの方向もセット
		SetShadowMapLightDirection(Handle_, data.LightDirection != Vector::ZEROVEC() ? data.LightDirection : GlobalLight);

		// シャドウマップに描画する範囲を設定
		DxLib::SetShadowMapDrawArea(Handle_, data.MinArea, data.MaxArea);
	}

	ShadowMap::~ShadowMap()
	{
		Fin();
	}

	void ShadowMap::Init()
	{
	}

	void ShadowMap::ShadowWriteStart()
	{
		// シャドウマップへの描画の準備
		ShadowMap_DrawSetup(Handle_);
	}

	void ShadowMap::ShadowWriteFin()
	{
		// シャドウマップへの描画を終了
		ShadowMap_DrawEnd();
	}

	void ShadowMap::ShadowDrawStart()
	{
		// 描画に使用するシャドウマップを設定
		SetUseShadowMap(Index_, Handle_);
	}

	void ShadowMap::ShadowDrawFin()
	{
		// 描画に使用するシャドウマップの設定を解除
		SetUseShadowMap(Index_, -1);
	}

	void ShadowMap::Delete()
	{
		DeleteShadowMap(Handle_);
	}

	void ShadowMap::Fin()
	{
		Delete();
	}

	void ShadowMap::SetShadowMapDrawArea(const Vector& minArea, const Vector& maxArea)
	{
		DxLib::SetShadowMapDrawArea(Handle_, minArea, maxArea);
	}

	void ShadowMap::SetLightDirection(const Vector& direction)
	{
		SetShadowMapLightDirection(Handle_, direction != Vector::ZEROVEC() ? direction : GlobalLight);
	}
}