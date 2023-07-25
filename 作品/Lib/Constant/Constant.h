#pragma once
#include "DxLib.h"
#include <initializer_list>
#include <array>
#include <string_view>
#include <xmmintrin.h>
#include "../typedef.h"

//定数関連

#if !_HAS_CXX17
#define CONSTEXPR_STRING(size) constexpr MyLib::Constant::String<(size)>
#define CONSTANT_STRING(size) MyLib::Constant::String<(size)>
#else
#define CONSTEXPR_STRING(size) constexpr std::string_view
#define CONSTANT_STRING(size) std::string_view
#endif //!_HAS_CXX17

#define CONSTANT_ARRAY(type, len) constexpr std::array<type, len>

namespace MyLib
{
	struct Color
	{
	private:
		typedef u8 ColorElm;
	public:
		constexpr Color() :r(0), g(0), b(0), a(255) {}
		constexpr Color(ColorElm red, ColorElm green, ColorElm blue, ColorElm alpha = 255) : r(red), g(green), b(blue), a(alpha) {}
		constexpr Color(unsigned __int32 colorCode) : code(colorCode) {}

		//uint型への変換
		constexpr operator u32() const { return code; }
		//DxLibでの%表記のカラーへの変換
		operator COLOR_F()const {
			__m128 color = _mm_setr_ps(static_cast<float>(r), static_cast<float>(g), static_cast<float>(b), static_cast<float>(a));
			__m128 denom = _mm_set1_ps(255.f);
			auto result = _mm_div_ps(color, denom);
			return COLOR_F(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2], result.m128_f32[3]);
		}
	public:
		union {
			struct {
				ColorElm b;	//青
				ColorElm g;	//緑
				ColorElm r;	//赤
				ColorElm a; //COLOR_Fなどのキャスト用にBufferをアルファ値として用いる
			};
			u32 code;
		};
	};

	namespace Size
	{
		constexpr size_t COLOR = sizeof(Color);
	}

	//定数式クラス
	class Constant
	{
	public:
		Constant() = delete;
		~Constant() = delete;
	public:
		template<size_t len>
		class String {
		public:
			constexpr String() {}
			constexpr String(const char* str) {
				for (auto i = 0; *str != '\0' && i < (len - 1); ++i, ++str)
					text[i] = *str;
			}
			constexpr operator const char* () const { return text; }
			constexpr const char* data() const { return text; }
		private:
			char text[len] = { 0 };
		};

	public:
		//定数式用GetColor(Colorクラスを推奨)
		static constexpr unsigned Color(unsigned char red, unsigned char green, unsigned char blue) { return (0xff << 6 * 4) | (red << 4 * 4) | (green << 2 * 4) | blue; }
	};
}

#include "Color.h"

namespace DxLib {
	//フォグカラーを設定する
	void SetFogColor(const MyLib::Color& c);
	//フォグカラーを取得する
	MyLib::Color GetFogColor();
}