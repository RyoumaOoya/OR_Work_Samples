#pragma once
#include "../InputCommon.h"
#include "../../Math/MyMathData.h"
#include "../../typedef.h"

namespace MyLib
{
	//�}�E�X�̓��͊Ǘ��N���X
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
			���͂̊m�F
			����: joypadCode �L�[�̎��, Type ���͂̎��
			�߂�l: ���͂���Ă��邩
		*/
		bool Input(u32 mouseCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE Type) const;
		/*
			���͂̊m�F
			����: joypadCode �L�[�̎��
			�߂�l: ���͂̎��
		*/
		INPUT_TYPE Input(u32 mouseCode) const;

		/*
			���͂̊m�F
			����: joypadCode �L�[�̎��, Type ���͂̎��
			�߂�l: ���͂���Ă��邩
		*/
		_inline bool operator()(u32 mouseCode, _In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE Type) const { return Input(mouseCode, Type); }
		/*
			���͂̊m�F
			����: joypadCode �L�[�̎��
			�߂�l: ���͂̎��
		*/
		_inline INPUT_TYPE operator()(u32 mouseCode) const { return Input(mouseCode); }

		/*
			���͂������������m�F
			����: ���͂̎��
			�߂�l: �L�[�̎��(���͂��ꂽ�L�[ ���͂��Ȃ������ꍇ-1)
		*/
		i32 AnythingInput(_In_range_(INPUT_TYPE::Push, INPUT_TYPE::Down)INPUT_TYPE type) const;

		//�}�E�X�̃X�N���[�����W���擾
		inline iVector2D GetMousePos() const { return CurrentMousePos_; }
		//�}�E�X�̈ړ��������擾
		inline iVector2D GetOffSetMousePos() const { return OffSetMousePos_; }
		//�}�E�X���W��ݒ�
		void SetMousePos(iVector2D Pos, double fixationTime = 0.0);
		//�}�E�X�J�[�\���̕`��ݒ�
		inline void DrawMousePointer(bool Draw) { SetMouseDispFlag(static_cast<int>(Draw)); }

		//�}�E�X�̈ړ��͈͂�ݒ�
		void SetMouseMoveRange(iVector2D min, iVector2D max);
		//�}�E�X�̈ړ��͈͂�������
		void InitMouseMoveRange();
		//�}�E�X�z�C�[���𓮂���������ݒ�
		int GetMouseWheelRotVolume()const { return MouseWheelVol_; }

	private:
		void Init();

		void Step();
		//���C���X���b�h�p�X�V����
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

		//���C���X���b�h�ōX�V���ꂽ�}�E�X���
		u32 MainKeyBuf_;
		//���݃t���[���̃}�E�X���
		u32 CurrentBuf_;
		//�O�t���[���̃}�E�X���
		u32 PreBuf_;

		//�}�E�X�̍��W�Œ莞��(�b)
		double FixationTime_;
	};
}
/*
	CMouseInput �������C���[�W

	oidMousePos		8byte
	currentMousePos	8byte
	Range			16byte
	FixationTime	8byte
	currentBuf		4byte	preBuf	4byte
*/

namespace Size {
	constexpr size_t MOUSE_INPUT = sizeof(MyLib::CMouseInput);
}