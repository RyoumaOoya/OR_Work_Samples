#include "String.h"
#include <string>
#include <xmmintrin.h>
#include <immintrin.h>

namespace MyLib
{
	bool StrCalc::Same(const char* strA, const char* strB)
	{
		size_t a_len = strlen(strA);

		//長さが違ったら同じではない
		if (a_len != strlen(strB))
			return false;
		a_len += 1;

		//文字列を格納して比較するための変数
		unsigned long long lnameA, lnameB;
		//const __m256i *a = reinterpret_cast<const __m256i*>(strA), *b = reinterpret_cast<const __m256i*>(strB);

		//8バイトずつ比較
		for (; a_len > 8; a_len -= 8) {
			//8バイト分コピー
			memcpy(&lnameA, strA, sizeof(unsigned long long));
			memcpy(&lnameB, strB, sizeof(unsigned long long));
			//値が違ったらfalse
			if ((lnameA & ~lnameB) != 0)
				return false;

			//8バイト進める
			strA += 8;
			strB += 8;
		}

		//残りが8バイト以下の時

		//コピーしたデータが残る可能性があるため
		//初期化する
		lnameA = 0;
		lnameB = 0;

		//残り文字数分コピー
		memcpy(&lnameA, strA, a_len);
		memcpy(&lnameB, strB, a_len);

		//比較した結果を返す
		return ((lnameA & ~lnameB) == 0);
	}
}