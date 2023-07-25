#pragma once
#include "../InputCommon.h"
#include "../../Math/MyMathData.h"
#include "../../typedef.h"

namespace MyLib
{
	//マウスの入力管理クラス
	class CMouseInput
	{
	private:
		friend class CInputManager;
		friend class Allocator;
	protected:
		CMouseInput();
		~CMouseInput();

	public:
		/*
			入力の確認
			引数: joypadCode キーの種類, Type 入力の種類
			戻り値: 入力されているか
		*/
		bool Input(u32 mouseCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE Type) const;
		/*
			入力の確認
			引数: joypadCode キーの種類
			戻り値: 入力の種類
		*/
		INPUT_TYPE Input(u32 mouseCode) const;

		/*
			入力の確認
			引数: joypadCode キーの種類, Type 入力の種類
			戻り値: 入力されているか
		*/
		_inline bool operator()(u32 mouseCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE Type) const { return Input(mouseCode, Type); }
		/*
			入力の確認
			引数: joypadCode キーの種類
			戻り値: 入力の種類
		*/
		_inline INPUT_TYPE operator()(u32 mouseCode) const { return Input(mouseCode); }

		/*
			入力があったかを確認
			引数: 入力の種類
			戻り値: キーの種類(入力されたキー 入力がなかった場合-1)
		*/
		i32 AnythingInput(_In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE type) const;

		//マウスのスクリーン座標を取得
		inline iVector2D GetMousePos() const { return CurrentMousePos_; }
		//マウスの移動距離を取得
		inline iVector2D GetOffSetMousePos() const { return OffSetMousePos_; }
		//マウス座標を設定
		void SetMousePos(iVector2D Pos, double fixationTime = 0.0);
		//マウスカーソルの描画設定
		inline void DrawMousePointer(bool Draw) { SetMouseDispFlag(static_cast<int>(Draw)); }

		//マウスの移動範囲を設定
		void SetMouseMoveRange(iVector2D min, iVector2D max);
		//マウスの移動範囲を初期化
		void InitMouseMoveRange();
		//マウスホイールを動かした数を設定
		int GetMouseWheelRotVolume()const { return MouseWheelVol_; }

	private:
		void Init();

		void Step();
		//メインスレッド用更新処理
		void MainStep();

		void AlwaysStep();

		void Fin();

	private:
		iVector2D OidMousePos_;
		iVector2D CurrentMousePos_;
		iVector2D MainMousePos_;
		iVector2D OffSetMousePos_;
		iVector2D FixationMousePos_;

		iVector2D Range_[2];

		int MouseWheelVol_;

		//メインスレッドで更新されたマウス情報
		u32 MainKeyBuf_;
		//現在フレームのマウス情報
		u32 CurrentBuf_;
		//前フレームのマウス情報
		u32 PreBuf_;

		//マウスの座標固定時間(秒)
		double FixationTime_;
	};
}
/*
	CMouseInput メモリイメージ

	oidMousePos		8byte
	currentMousePos	8byte
	Range			16byte
	FixationTime	8byte
	currentBuf		4byte	preBuf	4byte
*/

namespace Size {
	constexpr size_t MOUSE_INPUT = sizeof(MyLib::CMouseInput);
}