#pragma once
#include "../../Math/Interpolation.h"
namespace MyLib
{
	enum class FADE_STATUS :int {
		In,
		Out,
		End,
	};
	//�t�F�[�h�C���E�A�E�g�`��N���X
	class Fade {
	public:
		Fade() = delete;
		~Fade() = delete;
	public:
		static void Init();
		static void Step();
		static void Draw();
		static void Fin();

		/*
			�t�F�[�h�C�������̊J�n
			����: time ����(�b), kind �⊮�̎��
		*/
		static void FadeIn(float time = 1.2, Interpolation::KIND kind = Interpolation::KIND::EASE_IN);
		/*
			�t�F�[�h�A�E�g�����̊J�n
			����: time ����(�b), kind �⊮�̎��
		*/
		static void FadeOut(float time = 1.2, Interpolation::KIND kind = Interpolation::KIND::EASE_OUT);
		//�t�F�[�h���I����Ă��邩
		static bool IsFadeEnd() { return Status == FADE_STATUS::End; }

	private:
		static inline float Param = 0.f;
		static inline FADE_STATUS Status = FADE_STATUS::End;
		static inline float Time = 0.f;
		static inline Interpolation Interpolation;
	};
}