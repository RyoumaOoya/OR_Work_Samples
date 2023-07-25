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
	//�Q�[���p�b�h�̓��͊Ǘ��N���X
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
			OTHER,				// ���̑��̃R���g���[���[
			XBOX_360,			// Xbox360�R���g���[���[
			XBOX_ONE,			// XboxOne�R���g���[���[
			DUAL_SHOCK_3,		// PS3�R���g���[���[
			DUAL_SHOCK_4,		// PS4�R���g���[���[
			DUAL_SENSE,			// PS5�R���g���[���[
			SWITCH_JOY_CON_L,	// Switch Joycon(��)
			SWITCH_JOY_CON_R,	// Switch Joycon(�E)
			SWITCH_PRO_CTRL,	// Switch Pro�R���g���[���[
			NUM
		};

	public:
		/*
			���͂̊m�F
			����: joypadCode �L�[�̎��, Type ���͂̎��
			�߂�l: ���͂���Ă��邩
		*/
		bool Input(u32 joypadCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE Type) const;
		/*
			���͂̊m�F
			����: joypadCode �L�[�̎��
			�߂�l: ���͂̎��
		*/
		INPUT_TYPE Input(u32 joypadCode) const;

		/*
			���͂̊m�F
			����: joypadCode �L�[�̎��, Type ���͂̎��
			�߂�l: ���͂���Ă��邩
		*/
		_inline bool operator()(u32 joypadCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE Type) const { return Input(joypadCode, Type); }
		/*
			���͂̊m�F
			����: joypadCode �L�[�̎��
			�߂�l: ���͂̎��
		*/
		_inline INPUT_TYPE operator()(u32 joypadCode) const { return Input(joypadCode); }

		/*void AddBindKey(char Name[8], int key_code);
		void AddBindFunc(char Name[8], InputType Type, Object* object, void (*Func)(Object*));
		void AddBindFunc(char Name[8], Object* object, void (*Func)(Object*, Vector2D));*/

		/*
			���͂������������m�F
			����: ���͂̎��
			�߂�l: �L�[�̎��(���͂��ꂽ�L�[ ���͂��Ȃ������ꍇ-1)
		*/
		i32 AnythingInput(INPUT_TYPE type) const;

		//���X�e�B�b�N���̎擾
		inline StickVec GetLiftStick() const { return LeftStick_; };
		//�E�X�e�B�b�N���̎擾
		inline StickVec GetRightStick() const { return RightStick_; };

		//���X�e�B�b�N���̎擾(�����̂�)
		inline Vector2D GetLStickNVec() const { return static_cast<Vector2D>(LeftStick_).Normalize(); };
		//�E�X�e�B�b�N���̎擾(�����̂�)
		inline Vector2D GetRStickNVec() const { return static_cast<Vector2D>(RightStick_).Normalize(); };

		//�U���̊J�n
		inline void		StartVibration(int power, int time) const { StartJoypadVibration(DX_INPUT_PAD1, power, time); }
		//�U���̒�~
		inline void		StopVibration() const { StopJoypadVibration(DX_INPUT_PAD1); }

	private:
		void Init();

		void Step();
		void MainStep();

		void AlwaysStep();

		void Fin();

	private:
		//���C���X���b�h�ōX�V���ꂽ�Q�[���p�b�h���
		u32 MainBuf_;
		//���݃t���[���̃Q�[���p�b�h���
		u32 CurrentBuf_;
		//�O�t���[���̃Q�[���p�b�h���
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
	CMouseInput �������C���[�W

	currentBuf	4byte	preBuf	4byte
	LeftStick	8byte
	RightStick	8byte
*/

namespace Size {
	constexpr size_t JOYPAD_INPUT = sizeof(MyLib::CJoyPadInput);
}