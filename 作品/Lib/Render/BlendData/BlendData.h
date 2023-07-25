#pragma once
#include "DxLib.h"
#include "../../typedef.h"
#include "../../Define.h"
namespace MyLib
{
	enum class BLEND_MODE :u8 {
		NOBLEND,	// �m�[�u�����h
		ALPHA,		// ���u�����h
		ADD,		// ���Z�u�����h
		SUB,		// ���Z�u�����h
		MUL,		// ��Z�u�����h
		INVSRC,		// ���]�u�����h
		PMA_ALPHA,	// ��Z�ς݃��p�̃��u�����h
		PMA_ADD,	// ��Z�ς݃��p�̉��Z�u�����h
		PMA_SUB,	// ��Z�ς݃��p�̌��Z�u�����h
		PMA_INVSRC	//��Z�ς݃��p�̔��]�u�����h
	};

#define IBlendMode (u8)BLEND_MODE
	ENUM_OPERATORS(BLEND_MODE, u8);

	//�`��̃u�����h�֌W
	class BlendData
	{
		friend class ScopedBlend;
	public:
		constexpr BlendData() :Mode(BLEND_MODE::NOBLEND), Param(0) {}
		constexpr BlendData(BLEND_MODE mode, int param) : Mode(mode), Param(param) {}

	public:
		//�u�����h���J�n����
		void StartBlendMode() const { SetDrawBlendMode(static_cast<int>(Mode), Param); }
		//�u�����h���I������
		static void EndBlendMode() { SetDrawBlendMode(IBlendMode::NOBLEND, 0); }

		//�u�����h�̎�ނ�ݒ�
		void SetBlendMode(BLEND_MODE mode) { Mode = mode; }
		//�u�����h�̎�ނ��擾
		constexpr BLEND_MODE GetBlendMode() const { return Mode; }

		//�p�����[�^�ݒ�
		void SetParam(int param) { Param = param; }
		//�p�����[�^�擾
		constexpr int GerParam() const { return Param; }

		constexpr operator bool() const { return Mode != BLEND_MODE::NOBLEND; }
	private:
		BLEND_MODE Mode;
		u8 Param;
	};
}