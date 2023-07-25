#pragma once
#include "../../typedef.h"
#include "../../Define.h"
#include "../../Math/MyMathData.h"

namespace DxLib
{
	static int DrawRotaGraphFast(MyLib::iVector2D Pos, float ExRate, float Angle, int GrHandle, int TransFlag, int ReverseXFlag DEFAULTPARAM(= FALSE), int ReverseYFlag DEFAULTPARAM(= FALSE))	// 画像の回転描画( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	{
		return DrawRotaGraphFast(Pos.x, Pos.y, ExRate, Angle, GrHandle, TransFlag, ReverseXFlag, ReverseYFlag);
	}
}
namespace MyLib
{
	enum class RenderTags : u8
	{
		isWriting,		//書き込み中
		StaticShadow,	//スタート時のみ影を書き込む設定
		ShadowWrite,	//影の書き込みを行うか
		NotShadowDraw,	//このオブジェクトに対して影を描画するか？
		isDraw,			//通常の描画を行うか(一時的な透明化や影のみ描画するときなどに仕様)
		FrontDraw,
		isReverseX,
		isReverseY,

		isDeleteTarget,	//Render側でリストから削除するフラグ
		isDeleted,		//削除済みフラグ

		NUM
	};

	static_assert((int)RenderTags::NUM < 64, "Only 64 RenderTags can be defined");
	ENUM_OPERATORS(RenderTags, u8)
}