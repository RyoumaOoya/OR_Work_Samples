#include "DxLib.h"
#include "GameUISprite.h"
#include <assert.h>

namespace MyLib
{
	GameUISprite::GameUISprite()
	{
	}

	GameUISprite::~GameUISprite()
	{
		Fin();
	}

	void GameUISprite::Init(const Vector2D& in_pos, const Pointer<GameUI>& this_ptr)
	{
		SetThisPtr(this_ptr);
		RenderObj_.Pos_ = in_pos;
		RenderObj_.BlendData_ = BlendData(BLEND_MODE::ALPHA, 255);
	}

	void GameUISprite::Load(const char* file_path)
	{
		RenderObj_.Load(file_path);
		RenderObj_.WaitLoad();
		GraphSize_ = RenderObj_.GetGraphSize();
	}

	void GameUISprite::Step()
	{
		// スプライトの更新処理
	}

	void GameUISprite::Draw()
	{
	}

	void GameUISprite::Delete()
	{
		EreseSpriteList();
		RenderObj_.Delete();
	}

	void GameUISprite::Fin()
	{
	}
}