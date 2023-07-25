#pragma once
#include "DxLib.h"
#include <initializer_list>
#include <array>
#include <string_view>
#include <xmmintrin.h>
#include "../typedef.h"

//�萔�֘A

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

		//uint�^�ւ̕ϊ�
		constexpr operator u32() const { return code; }
		//DxLib�ł�%�\�L�̃J���[�ւ̕ϊ�
		operator COLOR_F()const {
			__m128 color = _mm_setr_ps(static_cast<float>(r), static_cast<float>(g), static_cast<float>(b), static_cast<float>(a));
			__m128 denom = _mm_set1_ps(255.f);
			auto result = _mm_div_ps(color, denom);
			return COLOR_F(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2], result.m128_f32[3]);
		}
	public:
		union {
			struct {
				ColorElm b;	//��
				ColorElm g;	//��
				ColorElm r;	//��
				ColorElm a; //COLOR_F�Ȃǂ̃L���X�g�p��Buffer���A���t�@�l�Ƃ��ėp����
			};
			u32 code;
		};
	};

	namespace Size
	{
		constexpr size_t COLOR = sizeof(Color);
	}

	//�萔���N���X
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
		//�萔���pGetColor(Color�N���X�𐄏�)
		static constexpr unsigned Color(unsigned char red, unsigned char green, unsigned char blue) { return (0xff << 6 * 4) | (red << 4 * 4) | (green << 2 * 4) | blue; }
	};
}

#include "Color.h"

namespace DxLib {
	//�t�H�O�J���[��ݒ肷��
	void SetFogColor(const MyLib::Color& c);
	//�t�H�O�J���[���擾����
	MyLib::Color GetFogColor();
}