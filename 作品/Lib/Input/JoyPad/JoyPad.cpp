#include "JoyPad.h"
#include "../../Define.h"
#include "DxLib.h"

namespace MyLib
{
	CJoyPadInput::CJoyPadInput()
	{
		MainBuf_ = 0;
		CurrentBuf_ = 0;
		PadType_ = 0;
		PreBuf_ = 0;
		SetUseJoypadVibrationFlag(TRUE);
	}

	CJoyPadInput::~CJoyPadInput()
	{
	}

	bool CJoyPadInput::Input(u32 joypadCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE type) const
	{
		switch (type)
		{
		case INPUT_TYPE::Push:	 return ((PreBuf_ & joypadCode) == 0 && (CurrentBuf_ & joypadCode) != 0);
		case INPUT_TYPE::Keep:	 return ((PreBuf_ & joypadCode) != 0 && (CurrentBuf_ & joypadCode) != 0);
		case INPUT_TYPE::Release:return ((PreBuf_ & joypadCode) != 0 && (CurrentBuf_ & joypadCode) == 0);
		case INPUT_TYPE::Down:	 return ((CurrentBuf_ & joypadCode) != 0);
		default:				 return false;
		}
	}

	INPUT_TYPE CJoyPadInput::Input(u32 joypadCode) const
	{
		return  ((PreBuf_ & joypadCode) != 0 && (CurrentBuf_ & joypadCode) != 0) ? INPUT_TYPE::Keep :
			((PreBuf_ & joypadCode) == 0) ? INPUT_TYPE::Push :
			((CurrentBuf_ & joypadCode) == 0) ? INPUT_TYPE::Release :
			INPUT_TYPE::Null;
	}

	i32 CJoyPadInput::AnythingInput(INPUT_TYPE type) const
	{
		FOR(Index, 32) {
			int joypad_code = 1 << Index;
			if (Input(joypad_code, type))
				return joypad_code;
		}

		return -1;
	}

	void CJoyPadInput::Init()
	{
		CurrentBuf_ = 0;
		PreBuf_ = 0;
		LeftStick_(0, 0);
		RightStick_(0, 0);
		PadType_ = GetJoypadType(DX_INPUT_PAD1);
		if (PadType_ == -1)
			PadType_ = 0;
		PadType_ = 2;
		std::function<StickVec(const DINPUT_JOYSTATE&)> func[IPAD_TYPE::NUM] = {
			[](const DINPUT_JOYSTATE& pad) { return StickVec(); },						//PAD_TYPE::OTHER
			[](const DINPUT_JOYSTATE& pad) { return StickVec(); },						//PAD_TYPE::XBOX_360
			[](const DINPUT_JOYSTATE& pad) { return StickVec(pad.Rx, pad.Rz); },		//PAD_TYPE::XBOX_ONE
			[](const DINPUT_JOYSTATE& pad) { return StickVec(); },						//PAD_TYPE::DUAL_SHOCK_4
			[](const DINPUT_JOYSTATE& pad) { return StickVec(); },						//PAD_TYPE::DUAL_SENSE
			[](const DINPUT_JOYSTATE& pad) { return StickVec(); },						//PAD_TYPE::SWITCH_JOY_CON_L
			[](const DINPUT_JOYSTATE& pad) { return StickVec(); },						//PAD_TYPE::SWITCH_JOY_CON_R
			[](const DINPUT_JOYSTATE& pad) { return StickVec(pad.Z, pad.Rz); },			//PAD_TYPE::SWITCH_PRO_CTRL
		};
		for (int i = 0; i < IPAD_TYPE::NUM; i++) {
			RightStickFunc_[i] = func[i];
		}
	}

	void CJoyPadInput::Step()
	{
		PreBuf_ = CurrentBuf_;
		CurrentBuf_ = MainBuf_;
	}

	void CJoyPadInput::MainStep()
	{
		MainBuf_ = GetJoypadInputState(DX_INPUT_PAD1);

		DINPUT_JOYSTATE JoypadState = { 0 };
		GetJoypadDirectInputState(DX_INPUT_PAD1, &JoypadState);
		LeftStick_.x = JoypadState.X;
		LeftStick_.y = JoypadState.Y;
		RightStick_ = RightStickFunc_[PadType_](JoypadState);
	}

	void CJoyPadInput::AlwaysStep()
	{
	}

	void CJoyPadInput::Fin()
	{

	}
}