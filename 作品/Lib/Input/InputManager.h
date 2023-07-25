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
	//���͊֘A
	class CInputManager
	{
	public:
		CInputManager() = delete;
		~CInputManager() = delete;
	public:
		static void Init();

		static void SetInput(bool key_board, bool mouse, bool joy_pad);

		/*
			���O���g�p�������͏���
			����: key �L�[��, inputType ���͂̎��
			�߂�l: ���͂��ꂽ��
		*/
		static bool Input(const std::string& key, INPUT_TYPE inputType);
		/*
			���O���g�p�������͏���
			����: key �L�[��, inputType ���͂̎��
			�߂�l: ���͂��ꂽ��
		*/
		static bool Input(std::string&& key, INPUT_TYPE inputType);

		/*
			�֐��̃o�C���h
			����: key �L�[��, func �֐�, type ���͂̎��
		*/
		static void BindFunc(const std::string& key, void(*func)(), INPUT_TYPE type);
		/*
			�֐��̃o�C���h
			����: key �L�[��, func �֐�, type ���͂̎��
		*/
		static void BindFunc(std::string&& key, void(*func)(), INPUT_TYPE type);

		//�t���[�����[�g�Ɋւ�炸�Ă΂�鏈��
		static void AlwaysStep();

		static void Step();
		//�����_�����O���}���`�X���b�h�ōs�����߂̊֐�
		static void MainStep();

		static void Fin();

		//�L�[�{�[�h�N���X�|�C���^���擾
		static Pointer<CKeyBoardInput> GetKeyBoard() { return KeyBoard_; };
		//�}�E�X�N���X�|�C���^���擾
		static Pointer<CMouseInput> GetMouse() { return Mouse_; };
		//�Q�[���p�b�h�N���X�|�C���^���擾
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
		//�L�[�{�[�h
		static inline Pointer<CKeyBoardInput> KeyBoard_;
		//�}�E�X
		static inline Pointer<CMouseInput> Mouse_;
		//�Q�[���p�b�h
		static inline Pointer<CJoyPadInput> JoyPad_;
	};
}

using namespace MyLib;