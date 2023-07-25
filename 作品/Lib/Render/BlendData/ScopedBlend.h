#pragma once
#include "BlendData.h"

namespace MyLib
{
	/*
		コンストラクタでブレンドモードを変えて
		デストラクタで確実にブレンドモードを初期化することで
		スコープでブレンド設定を変えられるようにするクラス
	*/
	class ScopedBlend
	{
	public:
		ScopedBlend(int BlendMode, int BlendParam) { SetDrawBlendMode(BlendMode, BlendParam); }
		ScopedBlend(BlendData blendData) { SetDrawBlendMode(static_cast<int>(blendData.Mode), blendData.Param); }
		~ScopedBlend() { SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); }

		void SetBlendMode(int BlendMode, int BlendParam) { SetDrawBlendMode(BlendMode, BlendParam); }
	};
}