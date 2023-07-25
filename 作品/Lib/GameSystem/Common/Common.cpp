#include "Common.h"

using namespace std;

namespace MyLib
{
	void Common::InitRandom()
	{
		// 乱数シード設定
		random_device rand;
		MT.seed(rand());
	}

	// 乱数取得
	int Common::GetRandom(int min_value, int max_value)
	{
		int range = max_value - min_value + 1;
		return (MT() % range) + min_value;
	}

	int Common::GetRandom(int max_value)
	{
		return MT() % (max_value + 1);
	}

	bool Common::GetRandomBoolean()
	{
		return MT() & 1;
	}

	// 小数点第n位までの乱数
	float Common::GetRandomf(float min_value, float max_value, int digit)
	{
		int coe = 1;
		for (int i = 0; i < digit - 1; i++) {
			coe *= 10;
		}

		float min = (min_value)*coe;
		float max = (max_value)*coe;
		int range = static_cast<int>(max - min);

		// ランダム範囲が無い場合は最小値を返す
		if (range == 0) return min_value;

		return static_cast<float>((MT() % range) + min) / coe;
	}

	float Common::GetRandomf(float max_value, int digit)
	{
		int coe = 1;
		for (int i = 0; i < digit - 1; i++) {
			coe *= 10;
		}
		float max = (max_value)*coe;
		int range = static_cast<int>(max);

		// ランダム範囲が無い場合は最小値を返す
		if (range == 0) return 0.f;

		return static_cast<float>(MT() % range) / coe;
	}

}