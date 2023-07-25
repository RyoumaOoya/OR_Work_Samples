#pragma once
#include "../Math/MyMath.h"
namespace MyLib
{
	class Interpolation
	{
	public:
		enum class KIND
		{
			NONE = -1,

			LERP,			// ���`���
			EASE_IN,		// �C�[�Y�C��(�񎟊֐����)
			EASE_OUT,		// �C�[�Y�A�E�g(�񎟊֐����)
			EASE_IN_OUT,	// �C�[�Y�C���A�E�g(�O���֐����)

			NUM,
		};
	public:
		Interpolation() :Kind_(KIND::NONE), TotalTime_(1.f) {}
		Interpolation(KIND kind, float totaltime) :Kind_(kind), TotalTime_(totaltime) {}
		~Interpolation() {}
		/*
			��ԏ���
			����: inT ���ݎ���, start inT / TotalTime_ == 0�̎��̐��l, end inT / TotalTime_ == 1�̎��̏���
			�߂�l: start�`end�̊�
		*/
		template<class T>
		T Interpolate(float inT, const T& start, const T& end) const {
			float t = inT / TotalTime_;

			//t�͈̔͂͒ʏ�u0�`1�v�̊ԂɂȂ邽�߁A�͈͊O�ɂȂ����ꍇ�͐�������
			if (t < 0.0f) t = 0.0f;
			else if (t > 1.0f) t = 1.0f;

			float v = Interpolate(t);
			T calc1 = start * (1.f - v);
			T calc2 = end * v;

			return calc1 + calc2;
		}

		void SetTotalTime(float t) { TotalTime_ = t; }
		void SetKind(KIND k) { Kind_ = k; }
	private:
		float Interpolate(float t) const { return ((Kind_ <= KIND::NONE || Kind_ >= KIND::NUM)) ? 0.f : (*InterpolateFunc[static_cast<int>(Kind_)])(t); }
	private:
		static constexpr float (*InterpolateFunc[(int)KIND::NUM])(float) = {
			[](float t) { return t; },										//���`���
			[](float t) { return MyMath::Squared(t); },						//�C�[�Y�C��(���X�ɉ���)
			[](float t) { return t * (2.f - t); },							//�C�[�Y�A�E�g(���X�Ɍ���)
			[](float t) { return MyMath::Squared(t) * (3.f - 2.f * t); }	//�C�[�Y�C���E�C�[�Y�A�E�g(���X�ɉ������Ă��猸��)
		};
	private:
		KIND Kind_;
		float TotalTime_;
	};
}
