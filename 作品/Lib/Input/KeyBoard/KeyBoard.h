#pragma once
#include "../InputCommon.h"
#include <vector>
#include "../../typedef.h"

namespace MyLib
{
	//キーボードの入力管理クラス
	class CKeyBoardInput
	{
	public:
		static constexpr size_t KEY_BUF_LEN = 256;
	private:
		friend class CInputManager;
		friend class Allocator;
	protected:
		CKeyBoardInput();
		~CKeyBoardInput();

	public:
		/*
			入力の確認
			引数: keyCode キーの種類, Type 入力の種類
			戻り値: 入力されているか
		*/
		bool Input(_In_range_(0, 255)u32 keyCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE type) const;
		/*
			入力の確認
			引数: keyCode キーの種類
			戻り値: 入力の種類
		*/
		INPUT_TYPE Input(_In_range_(0, 255)u32 keyCode) const;

		/*
			入力の確認
			引数: keyCode キーの種類, Type 入力の種類
			戻り値: 入力されているか
		*/
		_inline bool operator()(_In_range_(0, 255)u32 keyCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE type) const { return Input(keyCode, type); }
		/*
			入力の確認
			引数: keyCode キーの種類
			戻り値: 入力の種類
		*/
		_inline INPUT_TYPE operator()(_In_range_(0, 255)u32 keyCode) const { return Input(keyCode); }

		/*
			入力があったかを確認
			引数: 入力の種類
			戻り値: キーの種類(入力されたキー 入力がなかった場合-1)
		*/
		i32 AnythingInput(INPUT_TYPE type);
	private:
		void Init();

		void Step();
		//メインスレッド用更新処理
		void MainStep();

		void AlwaysStep();

		void Fin();

	private:
		//メインスレッドで更新されたキー情報
		char mainKeyBuf_[KEY_BUF_LEN];
		//現在フレームのキー情報
		char currentBuf_[KEY_BUF_LEN];
		//前フレームのキー情報
		char preBuf_[KEY_BUF_LEN];
	};
}
/*
	CMouseInput メモリイメージ

	currentBuf	256byte
	preBuf		256byte
*/

namespace Size {
	constexpr size_t KEYBOARD_INPUT = sizeof(MyLib::CKeyBoardInput);
}