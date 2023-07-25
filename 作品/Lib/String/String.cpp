#include "String.h"
#include <string>
#include <xmmintrin.h>
#include <immintrin.h>

namespace MyLib
{
	bool StrCalc::Same(const char* strA, const char* strB)
	{
		size_t a_len = strlen(strA);

		//������������瓯���ł͂Ȃ�
		if (a_len != strlen(strB))
			return false;
		a_len += 1;

		//��������i�[���Ĕ�r���邽�߂̕ϐ�
		unsigned long long lnameA, lnameB;
		//const __m256i *a = reinterpret_cast<const __m256i*>(strA), *b = reinterpret_cast<const __m256i*>(strB);

		//8�o�C�g����r
		for (; a_len > 8; a_len -= 8) {
			//8�o�C�g���R�s�[
			memcpy(&lnameA, strA, sizeof(unsigned long long));
			memcpy(&lnameB, strB, sizeof(unsigned long long));
			//�l���������false
			if ((lnameA & ~lnameB) != 0)
				return false;

			//8�o�C�g�i�߂�
			strA += 8;
			strB += 8;
		}

		//�c�肪8�o�C�g�ȉ��̎�

		//�R�s�[�����f�[�^���c��\�������邽��
		//����������
		lnameA = 0;
		lnameB = 0;

		//�c�蕶�������R�s�[
		memcpy(&lnameA, strA, a_len);
		memcpy(&lnameB, strB, a_len);

		//��r�������ʂ�Ԃ�
		return ((lnameA & ~lnameB) == 0);
	}
}