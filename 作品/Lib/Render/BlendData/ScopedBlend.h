#pragma once
#include "BlendData.h"

namespace MyLib
{
	/*
		�R���X�g���N�^�Ńu�����h���[�h��ς���
		�f�X�g���N�^�Ŋm���Ƀu�����h���[�h�����������邱�Ƃ�
		�X�R�[�v�Ńu�����h�ݒ��ς�����悤�ɂ���N���X
	*/
	class ScopedBlend
	{
	public:
		ScopedBlend(int BlendMode, int BlendParam) { SetDrawBlendMode(BlendMode, BlendParam); }
		ScopedBlend(BlendData blendData) { SetDrawBlendMode(static_cast<int>(blendData.Mode), blendData.Param); }
		~ScopedBlend() { SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); }

		void SetBlendMode(int BlendMode, int BlendParam) { SetDrawBlendMode(BlendMode, BlendParam); }
	};
}