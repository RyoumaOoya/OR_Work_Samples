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
		// �V���h�E�}�b�v�n���h���̍쐬
		Handle_ = MakeShadowMap(data.MapSize.x, data.MapSize.y);

		// �V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
		SetShadowMapLightDirection(Handle_, data.LightDirection != Vector::ZEROVEC() ? data.LightDirection : GlobalLight);

		// �V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
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
		// �V���h�E�}�b�v�ւ̕`��̏���
		ShadowMap_DrawSetup(Handle_);
	}

	void ShadowMap::ShadowWriteFin()
	{
		// �V���h�E�}�b�v�ւ̕`����I��
		ShadowMap_DrawEnd();
	}

	void ShadowMap::ShadowDrawStart()
	{
		// �`��Ɏg�p����V���h�E�}�b�v��ݒ�
		SetUseShadowMap(Index_, Handle_);
	}

	void ShadowMap::ShadowDrawFin()
	{
		// �`��Ɏg�p����V���h�E�}�b�v�̐ݒ������
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