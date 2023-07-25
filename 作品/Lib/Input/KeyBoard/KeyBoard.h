#pragma once
#include "../InputCommon.h"
#include <vector>
#include "../../typedef.h"

namespace MyLib
{
	//�L�[�{�[�h�̓��͊Ǘ��N���X
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
			���͂̊m�F
			����: keyCode �L�[�̎��, Type ���͂̎��
			�߂�l: ���͂���Ă��邩
		*/
		bool Input(_In_range_(0, 255)u32 keyCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE type) const;
		/*
			���͂̊m�F
			����: keyCode �L�[�̎��
			�߂�l: ���͂̎��
		*/
		INPUT_TYPE Input(_In_range_(0, 255)u32 keyCode) const;

		/*
			���͂̊m�F
			����: keyCode �L�[�̎��, Type ���͂̎��
			�߂�l: ���͂���Ă��邩
		*/
		_inline bool operator()(_In_range_(0, 255)u32 keyCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE type) const { return Input(keyCode, type); }
		/*
			���͂̊m�F
			����: keyCode �L�[�̎��
			�߂�l: ���͂̎��
		*/
		_inline INPUT_TYPE operator()(_In_range_(0, 255)u32 keyCode) const { return Input(keyCode); }

		/*
			���͂������������m�F
			����: ���͂̎��
			�߂�l: �L�[�̎��(���͂��ꂽ�L�[ ���͂��Ȃ������ꍇ-1)
		*/
		i32 AnythingInput(INPUT_TYPE type);
	private:
		void Init();

		void Step();
		//���C���X���b�h�p�X�V����
		void MainStep();

		void AlwaysStep();

		void Fin();

	private:
		//���C���X���b�h�ōX�V���ꂽ�L�[���
		char mainKeyBuf_[KEY_BUF_LEN];
		//���݃t���[���̃L�[���
		char currentBuf_[KEY_BUF_LEN];
		//�O�t���[���̃L�[���
		char preBuf_[KEY_BUF_LEN];
	};
}
/*
	CMouseInput �������C���[�W

	currentBuf	256byte
	preBuf		256byte
*/

namespace Size {
	constexpr size_t KEYBOARD_INPUT = sizeof(MyLib::CKeyBoardInput);
}