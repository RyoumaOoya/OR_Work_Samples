#pragma once
#include <random>

namespace MyLib
{
	class Common
	{
	private:
		Common() = delete;
		~Common() = delete;

		//Tの最大値取得
		template<class T>
		constexpr static auto GetMaxVal() ->typename std::enable_if<std::is_integral_v<T>, T>::type {
			T result = ~T(0ull);
			if constexpr (std::is_signed_v<T>) {
				result &= ~GetSignBit<T>();
			}
			return result;
		}
		//Tの最大値取得
		template<class T>
		constexpr static auto GetMaxVal(T) ->typename std::enable_if<std::is_integral_v<T>, T>::type { return GetMaxVal<T>; }
	public:
		//乱数初期化
		static void InitRandom();
		/*
			乱数生成
			引数: min_value 最低値, max_value 最大値
		*/
		static int  GetRandom(int min_value, int max_value);
		/*
			乱数生成
			引数: max_value 最大値
		*/
		static int  GetRandom(int max_value);
		//乱数生成(0or1)
		static bool  GetRandomBoolean();
		//指定の整数型の乱数取得
		template<class T>
		static auto GetRandom()->typename std::enable_if<std::is_integral_v<T>, T>::type
		{
			return MT() & GetMaxVal<T>();
		}
		/*
			乱数生成
			引数: min_value 最低値, max_value 最大値, digit 少数の桁数(精度)
		*/
		static float GetRandomf(float min_value, float max_value, int digit = 5);
		/*
			乱数生成
			引数: max_value 最大値, digit 少数の桁数(精度)
		*/
		static float GetRandomf(float max_value, int digit = 5);
	private:

		static inline std::mt19937_64 MT;
	};
}