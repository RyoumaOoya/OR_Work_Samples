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
			return MT() & MyMath::GetMaxVal<T>();
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