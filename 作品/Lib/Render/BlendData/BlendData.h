#pragma once
#include "DxLib.h"
#include "../../typedef.h"
#include "../../Define.h"
namespace MyLib
{
	enum class BLEND_MODE :u8 {
		NOBLEND,	// ノーブレンド
		ALPHA,		// αブレンド
		ADD,		// 加算ブレンド
		SUB,		// 減算ブレンド
		MUL,		// 乗算ブレンド
		INVSRC,		// 反転ブレンド
		PMA_ALPHA,	// 乗算済みα用のαブレンド
		PMA_ADD,	// 乗算済みα用の加算ブレンド
		PMA_SUB,	// 乗算済みα用の減算ブレンド
		PMA_INVSRC	//乗算済みα用の反転ブレンド
	};

#define IBlendMode (u8)BLEND_MODE
	ENUM_OPERATORS(BLEND_MODE, u8);

	//描画のブレンド関係
	class BlendData
	{
		friend class ScopedBlend;
	public:
		constexpr BlendData() :Mode(BLEND_MODE::NOBLEND), Param(0) {}
		constexpr BlendData(BLEND_MODE mode, int param) : Mode(mode), Param(param) {}

	public:
		//ブレンドを開始する
		void StartBlendMode() const { SetDrawBlendMode(static_cast<int>(Mode), Param); }
		//ブレンドを終了する
		static void EndBlendMode() { SetDrawBlendMode(IBlendMode::NOBLEND, 0); }

		//ブレンドの種類を設定
		void SetBlendMode(BLEND_MODE mode) { Mode = mode; }
		//ブレンドの種類を取得
		constexpr BLEND_MODE GetBlendMode() const { return Mode; }

		//パラメータ設定
		void SetParam(int param) { Param = param; }
		//パラメータ取得
		constexpr int GerParam() const { return Param; }

		constexpr operator bool() const { return Mode != BLEND_MODE::NOBLEND; }
	private:
		BLEND_MODE Mode;
		u8 Param;
	};
}