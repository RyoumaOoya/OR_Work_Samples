#pragma once
#include "../InputCommon.h"
#include "../../Object/Object.h"
#include "../../Math/MyMathData.h"
#include "../../typedef.h"
#include <vector>
#include <functional>

//#define PAD_INPUT_LSTICK (0x40000000)
//#define PAD_INPUT_RSTICK (0x80000000)
namespace MyLib
{
	//ゲームパッドの入力管理クラス
	class CJoyPadInput
	{
	private:
		friend class CInputManager;
		friend class Allocator;
		typedef iVector2D StickVec;
	protected:
		CJoyPadInput();
		~CJoyPadInput();

	public:
#define IPAD_TYPE (int)PAD_TYPE
		enum class PAD_TYPE {
			OTHER,				// その他のコントローラー
			XBOX_360,			// Xbox360コントローラー
			XBOX_ONE,			// XboxOneコントローラー
			DUAL_SHOCK_3,		// PS3コントローラー
			DUAL_SHOCK_4,		// PS4コントローラー
			DUAL_SENSE,			// PS5コントローラー
			SWITCH_JOY_CON_L,	// Switch Joycon(左)
			SWITCH_JOY_CON_R,	// Switch Joycon(右)
			SWITCH_PRO_CTRL,	// Switch Proコントローラー
			NUM
		};

	public:
		/*
			入力の確認
			引数: joypadCode キーの種類, Type 入力の種類
			戻り値: 入力されているか
		*/
		bool Input(u32 joypadCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE Type) const;
		/*
			入力の確認
			引数: joypadCode キーの種類
			戻り値: 入力の種類
		*/
		INPUT_TYPE Input(u32 joypadCode) const;

		/*
			入力の確認
			引数: joypadCode キーの種類, Type 入力の種類
			戻り値: 入力されているか
		*/
		_inline bool operator()(u32 joypadCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE Type) const { return Input(joypadCode, Type); }
		/*
			入力の確認
			引数: joypadCode キーの種類
			戻り値: 入力の種類
		*/
		_inline INPUT_TYPE operator()(u32 joypadCode) const { return Input(joypadCode); }

		/*void AddBindKey(char Name[8], int key_code);
		void AddBindFunc(char Name[8], InputType Type, Object* object, void (*Func)(Object*));
		void AddBindFunc(char Name[8], Object* object, void (*Func)(Object*, Vector2D));*/

		/*
			入力があったかを確認
			引数: 入力の種類
			戻り値: キーの種類(入力されたキー 入力がなかった場合-1)
		*/
		i32 AnythingInput(INPUT_TYPE type) const;

		//左スティック情報の取得
		inline StickVec GetLiftStick() const { return LeftStick_; };
		//右スティック情報の取得
		inline StickVec GetRightStick() const { return RightStick_; };

		//左スティック情報の取得(方向のみ)
		inline Vector2D GetLStickNVec() const { return static_cast<Vector2D>(LeftStick_).Normalize(); };
		//右スティック情報の取得(方向のみ)
		inline Vector2D GetRStickNVec() const { return static_cast<Vector2D>(RightStick_).Normalize(); };

		//振動の開始
		inline void		StartVibration(int power, int time) const { StartJoypadVibration(DX_INPUT_PAD1, power, time); }
		//振動の停止
		inline void		StopVibration() const { StopJoypadVibration(DX_INPUT_PAD1); }

	private:
		void Init();

		void Step();
		void MainStep();

		void AlwaysStep();

		void Fin();

	private:
		//メインスレッドで更新されたゲームパッド情報
		u32 MainBuf_;
		//現在フレームのゲームパッド情報
		u32 CurrentBuf_;
		//前フレームのゲームパッド情報
		u32 PreBuf_;
		i32 PadType_;

		StickVec LeftStick_;
		StickVec RightStick_;
	
		std::function<StickVec(const DINPUT_JOYSTATE&)> RightStickFunc_[IPAD_TYPE::NUM];
	};
}
#undef IPAD_TYPE

#define IPAD_TYPE (int)CJoyPadInput::PAD_TYPE

/*
	CMouseInput メモリイメージ

	currentBuf	4byte	preBuf	4byte
	LeftStick	8byte
	RightStick	8byte
*/

namespace Size {
	constexpr size_t JOYPAD_INPUT = sizeof(MyLib::CJoyPadInput);
}