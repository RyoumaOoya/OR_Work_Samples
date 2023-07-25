#pragma once

#include "InputCommon.h"
#include "KeyBoard/KeyBoard.h"
#include "Mouse/Mouse.h"
#include "JoyPad/JoyPad.h"
#include <unordered_map>
#include <string>
//#include <vector>
#include "../GameSystem/Allocator/Allocator.h"

#define GET_KEY MyLib::CInputManager::GetKeyBoard()
#define GET_PAD MyLib::CInputManager::GetJoyPad()
#define GET_MOUSE MyLib::CInputManager::GetMouse()

namespace MyLib
{
	//入力関連
	class CInputManager
	{
	public:
		CInputManager() = delete;
		~CInputManager() = delete;
	public:
		static void Init();

		static void SetInput(bool key_board, bool mouse, bool joy_pad);

		/*
			名前を使用した入力処理
			引数: key キー名, inputType 入力の種類
			戻り値: 入力されたか
		*/
		static bool Input(const std::string& key, INPUT_TYPE inputType);
		/*
			名前を使用した入力処理
			引数: key キー名, inputType 入力の種類
			戻り値: 入力されたか
		*/
		static bool Input(std::string&& key, INPUT_TYPE inputType);

		/*
			関数のバインド
			引数: key キー名, func 関数, type 入力の種類
		*/
		static void BindFunc(const std::string& key, void(*func)(), INPUT_TYPE type);
		/*
			関数のバインド
			引数: key キー名, func 関数, type 入力の種類
		*/
		static void BindFunc(std::string&& key, void(*func)(), INPUT_TYPE type);

		//フレームレートに関わらず呼ばれる処理
		static void AlwaysStep();

		static void Step();
		//レンダリングをマルチスレッドで行うための関数
		static void MainStep();

		static void Fin();

		//キーボードクラスポインタを取得
		static Pointer<CKeyBoardInput> GetKeyBoard() { return KeyBoard_; };
		//マウスクラスポインタを取得
		static Pointer<CMouseInput> GetMouse() { return Mouse_; };
		//ゲームパッドクラスポインタを取得
		static Pointer<CJoyPadInput> GetJoyPad() { return JoyPad_; };

	private:
		static inline void KeyBoardInputStart();
		static inline void KeyBoardInputFin();

		static inline void MouseInputStart();
		static inline void MouseInputFin();

		static inline void JoyPadInputStart();
		static inline void JoyPadInputFin();

	private:
		//static constexpr int k = sizeof(CKeyBoardInput) + sizeof(CMouseInput) + sizeof(CJoyPadInput);

		static inline std::unordered_map<std::string, Bind> BindData_;
		//キーボード
		static inline Pointer<CKeyBoardInput> KeyBoard_;
		//マウス
		static inline Pointer<CMouseInput> Mouse_;
		//ゲームパッド
		static inline Pointer<CJoyPadInput> JoyPad_;
	};
}

using namespace MyLib;