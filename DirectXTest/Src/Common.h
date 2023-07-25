#pragma once
#include <random>

namespace MyLib
{
	class Common
	{
	private:
		Common() = delete;
		~Common() = delete;

		//T�̍ő�l�擾
		template<class T>
		constexpr static auto GetMaxVal() ->typename std::enable_if<std::is_integral_v<T>, T>::type {
			T result = ~T(0ull);
			if constexpr (std::is_signed_v<T>) {
				result &= ~GetSignBit<T>();
			}
			return result;
		}
		//T�̍ő�l�擾
		template<class T>
		constexpr static auto GetMaxVal(T) ->typename std::enable_if<std::is_integral_v<T>, T>::type { return GetMaxVal<T>; }
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
			return MT() & GetMaxVal<T>();
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