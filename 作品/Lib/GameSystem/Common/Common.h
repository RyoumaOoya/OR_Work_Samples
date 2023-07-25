#pragma once
#include <random>
#include "../../Math/MyMath.h"
namespace MyLib
{
	class Common
	{
	private:
		Common() = delete;
		~Common() = delete;
	public:
		static constexpr float Y_LOWER_LIMIT = -20.f;
		static constexpr Vector Gravity = Vector(0.f, -0.01f, 0.f) * 90;
	public:
		//����������
		static void InitRandom();
		/*
			��������
			����: min_value �Œ�l, max_value �ő�l
		*/
		static int  GetRandom(int min_value, int max_value);
		/*
			��������
			����: max_value �ő�l
		*/
		static int  GetRandom(int max_value);
		//��������(0or1)
		static bool  GetRandomBoolean();
		//�w��̐����^�̗����擾
		template<class T>
		static auto GetRandom()->typename std::enable_if<std::is_integral_v<T>, T>::type
		{
			return MT() & MyMath::GetMaxVal<T>();
		}
		/*
			��������
			����: min_value �Œ�l, max_value �ő�l, digit �����̌���(���x)
		*/
		static float GetRandomf(float min_value, float max_value, int digit = 5);
		/*
			��������
			����: max_value �ő�l, digit �����̌���(���x)
		*/
		static float GetRandomf(float max_value, int digit = 5);
	private:

		static inline std::mt19937_64 MT;
	};
}