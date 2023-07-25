#include "Common.h"

using namespace std;

namespace MyLib
{
	void Common::InitRandom()
	{
		// �����V�[�h�ݒ�
		random_device rand;
		MT.seed(rand());
	}

	// �����擾
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

	// �����_��n�ʂ܂ł̗���
	float Common::GetRandomf(float min_value, float max_value, int digit)
	{
		int coe = 1;
		for (int i = 0; i < digit - 1; i++) {
			coe *= 10;
		}

		float min = (min_value)*coe;
		float max = (max_value)*coe;
		int range = static_cast<int>(max - min);

		// �����_���͈͂������ꍇ�͍ŏ��l��Ԃ�
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

		// �����_���͈͂������ꍇ�͍ŏ��l��Ԃ�
		if (range == 0) return 0.f;

		return static_cast<float>(MT() % range) / coe;
	}

}