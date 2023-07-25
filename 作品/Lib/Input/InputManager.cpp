#include "DxLib.h"
#include "InputManager.h"
#include "../Constant/Constant.h"
namespace MyLib
{
	namespace {
		struct DEFAULT_BIND_DATA {
			constexpr DEFAULT_BIND_DATA() {}
			constexpr DEFAULT_BIND_DATA(const char* s, const ConstBind& bind_data) :data(bind_data) {
				for (int i = 0; s[i] != '\0' && i < 16; i++)
					str[i] = s[i];
			}
			char str[16] = { 0 };
			ConstBind data;
		};
		CONSTANT_ARRAY(DEFAULT_BIND_DATA, 16) DefaultBindData = {
			DEFAULT_BIND_DATA("Enter",	ConstBind(KEY_INPUT_RETURN, MOUSE_INPUT_LEFT,	PAD_INPUT_B)),
			DEFAULT_BIND_DATA("Forward",ConstBind(KEY_INPUT_W,		MOUSE_INPUT_NULL,	PAD_INPUT_NULL)),
			DEFAULT_BIND_DATA("Lift",	ConstBind(KEY_INPUT_A,		MOUSE_INPUT_NULL,	PAD_INPUT_NULL)),
			DEFAULT_BIND_DATA("Right",	ConstBind(KEY_INPUT_D,		MOUSE_INPUT_NULL,	PAD_INPUT_NULL)),
			DEFAULT_BIND_DATA("Back",	ConstBind(KEY_INPUT_S,		MOUSE_INPUT_NULL,	PAD_INPUT_NULL)),
			DEFAULT_BIND_DATA("Shot",	ConstBind(KEY_INPUT_NULL,	MOUSE_INPUT_1,		PAD_INPUT_A)),
		};
	}

	void CInputManager::Init()
	{
		for (const auto& data : DefaultBindData) {
			if (data.str[0] == '\0')
				break;
			BindData_.emplace(data.str, data.data);
		}
		if (KeyBoard_) {
			MyFree(KeyBoard_);
		}
		if (Mouse_) {
			MyFree(Mouse_);
		}
		if (JoyPad_) {
			MyFree(JoyPad_);
		}
	}

	void CInputManager::SetInput(bool key_board, bool mouse, bool joy_pad)
	{
		if (key_board) {
			KeyBoardInputStart();
		}

		if (mouse) {
			MouseInputStart();
		}

		if (joy_pad) {
			JoyPadInputStart();
		}
	}
	//変更したら右辺参照版関数も書き換えること
	bool CInputManager::Input(const std::string& key, INPUT_TYPE inputType)
	{
		auto itr = BindData_.find(key);
		if (itr == end(BindData_))
			return false;
		Bind data = itr->second;

		return	(data.keyCode != KEY_INPUT_NULL && KeyBoard_	? KeyBoard_->Input(data.keyCode, inputType) : false) ||
			(/*NULLチェック不要*/					Mouse_		? Mouse_->Input(data.mouseCode, inputType) : false) ||
			(/*同上*/								JoyPad_		? JoyPad_->Input(data.joypadCode, inputType) : false);
	}
	//変更したら左辺参照版関数も書き換えること
	bool CInputManager::Input(std::string&& key, INPUT_TYPE inputType)
	{
		auto itr = BindData_.find(key);
		if (itr == end(BindData_))
			return false;
		Bind data = itr->second;

		return	(data.keyCode != KEY_INPUT_NULL &&		KeyBoard_	? KeyBoard_->Input(data.keyCode, inputType) : false) ||
				(/*NULLチェック不要*/					Mouse_		? Mouse_->Input(data.mouseCode, inputType) : false) ||
				(/*同上*/								JoyPad_		? JoyPad_->Input(data.joypadCode, inputType) : false);
	}
	//変更したら右辺参照版関数も書き換えること
	void CInputManager::BindFunc(const std::string& key, void(*func)(), INPUT_TYPE type)
	{
		if (INPUT_TYPE::Null == type || type == INPUT_TYPE::NUM)
			return;
		auto itr = BindData_.find(key);
		if (itr == end(BindData_))
			return;
		Bind& data = itr->second;

		auto f = &data.pushFunc;

		*f = func;
	}
	//変更したら左辺参照版関数も書き換えること
	void CInputManager::BindFunc(std::string&& key, void(*func)(), INPUT_TYPE type)
	{
		if (INPUT_TYPE::Null == type || type == INPUT_TYPE::NUM)
			return;
		auto itr = BindData_.find(key);
		if (itr == end(BindData_))
			return;
		Bind& data = itr->second;

		auto f = &data.pushFunc;

		*f = func;
	}

	void CInputManager::AlwaysStep()
	{
		if (KeyBoard_) {
			KeyBoard_->AlwaysStep();
		}
		if (Mouse_) {
			Mouse_->AlwaysStep();
		}
		if (JoyPad_) {
			JoyPad_->AlwaysStep();
		}
	}

	void CInputManager::Step()
	{
		//キーボード変数更新
		if (KeyBoard_) {
			KeyBoard_->Step();
		}

		//マウス情報更新
		if (Mouse_) {
			Mouse_->Step();
		}

		//ゲームパッド情報更新
		if (JoyPad_) {
			JoyPad_->Step();
		}

		static auto bind_func = [](INPUT_TYPE type, const Bind& data) {
			switch (type)
			{
			case INPUT_TYPE::Push:	if (data.pushFunc) (data.pushFunc)(); if (data.downFunc) (data.downFunc)(); break;
			case INPUT_TYPE::Keep:	if (data.keepFunc) (data.keepFunc)(); if (data.downFunc) (data.downFunc)(); break;
			case INPUT_TYPE::Release:if (data.releaseFunc) (data.releaseFunc)(); break;
			}
		};

		for (auto pair : BindData_) {
			const Bind& data = pair.second;
			if (KeyBoard_ && data.keyCode != KEY_INPUT_NULL) {
				bind_func(KeyBoard_->Input(data.keyCode), data);
			}
			if (Mouse_) {
				bind_func(Mouse_->Input(data.keyCode), data);
			}
			if (JoyPad_) {
				bind_func(JoyPad_->Input(data.keyCode), data);
			}
		}
	}

	void CInputManager::MainStep()
	{
		//キーボード変数更新
		if (KeyBoard_) {
			KeyBoard_->MainStep();
		}

		//マウス情報更新
		if (Mouse_) {
			Mouse_->MainStep();
		}

		//ゲームパッド情報更新
		if (JoyPad_) {
			JoyPad_->MainStep();
		}
	}

	void CInputManager::Fin()
	{
		KeyBoardInputFin();

		MouseInputFin();

		JoyPadInputFin();
	}

	inline void CInputManager::KeyBoardInputStart()
	{
		if (!KeyBoard_)
			KeyBoard_ = MyNew(CKeyBoardInput, AllocType::Library, Tags(MemTag::Library));

		KeyBoard_->Init();
	}
	inline void CInputManager::KeyBoardInputFin()
	{
		if (KeyBoard_) {
			KeyBoard_->Fin();
			MyFree(KeyBoard_);
		}
	}

	inline void CInputManager::MouseInputStart()
	{
		if (!Mouse_)
			Mouse_ = MyNew(CMouseInput, AllocType::Library, Tags(MemTag::Library));

		Mouse_->Init();
	}
	inline void CInputManager::MouseInputFin()
	{
		if (Mouse_) {
			Mouse_->Fin();
			MyFree(Mouse_);
		}
	}

	inline void CInputManager::JoyPadInputStart()
	{
		if (!JoyPad_)
			JoyPad_ = MyNew(CJoyPadInput, AllocType::Library, Tags(MemTag::Library));

		JoyPad_->Init();
	}
	inline void CInputManager::JoyPadInputFin()
	{
		if (JoyPad_) {
			JoyPad_->Fin();
			MyFree(JoyPad_);
		}
	}
}