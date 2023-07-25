#pragma once
#include "../../typedef.h"
#include "../../Define.h"
#include "../../Math/MyMathData.h"

namespace DxLib
{
	static int DrawRotaGraphFast(MyLib::iVector2D Pos, float ExRate, float Angle, int GrHandle, int TransFlag, int ReverseXFlag DEFAULTPARAM(= FALSE), int ReverseYFlag DEFAULTPARAM(= FALSE))	// �摜�̉�]�`��( �����ŁA���W�v�Z�̃A���S���Y�����ȗ�������Ă��܂��A�`�挋�ʂɕs�s����������΂�����̕��������ł� )
	{
		return DrawRotaGraphFast(Pos.x, Pos.y, ExRate, Angle, GrHandle, TransFlag, ReverseXFlag, ReverseYFlag);
	}
}
namespace MyLib
{
	enum class RenderTags : u8
	{
		isWriting,		//�������ݒ�
		StaticShadow,	//�X�^�[�g���̂݉e���������ސݒ�
		ShadowWrite,	//�e�̏������݂��s����
		NotShadowDraw,	//���̃I�u�W�F�N�g�ɑ΂��ĉe��`�悷�邩�H
		isDraw,			//�ʏ�̕`����s����(�ꎞ�I�ȓ�������e�̂ݕ`�悷��Ƃ��ȂǂɎd�l)
		FrontDraw,
		isReverseX,
		isReverseY,

		isDeleteTarget,	//Render���Ń��X�g����폜����t���O
		isDeleted,		//�폜�ς݃t���O

		NUM
	};

	static_assert((int)RenderTags::NUM < 64, "Only 64 RenderTags can be defined");
	ENUM_OPERATORS(RenderTags, u8)
}