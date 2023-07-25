#pragma once
#include <xmmintrin.h>

class COLOR;
class COLOR_F;

__declspec(align(4)) class COLOR {
private:
	typedef unsigned __int8 ColorElm;
public:
	constexpr COLOR() :R(0), G(0), B(0), A(0) {}
	constexpr COLOR(ColorElm r, ColorElm g, ColorElm b, ColorElm a) : R(r), G(g), B(b), A(a) {}
	constexpr COLOR(unsigned __int32 code) : ColorCode(code) {}

	operator ColorElm*() noexcept { return Arr; }
	operator const ColorElm*() const noexcept { return Arr; }
	constexpr explicit operator unsigned __int32() const noexcept { return ColorCode; }
	constexpr ColorElm operator[](size_t idx) const { return Arr[idx]; }
	constexpr ColorElm& operator[](size_t idx) { return Arr[idx]; }
	constexpr explicit operator COLOR_F() const noexcept;
public:
	union {
		struct {
			ColorElm B, G, R, A;
		};
		unsigned __int32 ColorCode;	//0xffffffff等カラーコード指定用
		ColorElm  Arr[4];	//配列型
	};
};

//フロート型のカラー構造体
__declspec(align(16)) class COLOR_F {
public:
	constexpr COLOR_F() :R(0.f), G(0.f), B(0.f), A(0.f) {}
	constexpr COLOR_F(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
	constexpr COLOR_F(float arr[4]) : R(arr[0]), G(arr[1]), B(arr[2]), A(arr[3]) {}

	operator float*() noexcept { return Arr; }
	operator const float*() const noexcept { return Arr; }
	constexpr float operator[](size_t idx) const { return Arr[idx]; }
	constexpr float& operator[](size_t idx) { return Arr[idx]; }
	constexpr explicit operator __m128() const noexcept { return m128; }
	constexpr explicit operator COLOR() const noexcept;
public:
	union {
		struct {
			float R, G, B, A;
		};
		float Arr[4];
		__m128 m128;
	};
};

constexpr COLOR::operator COLOR_F() const noexcept
{
	return COLOR_F(static_cast<float>(R) / 255.f, static_cast<float>(G) / 255.f, static_cast<float>(B) / 255.f, static_cast<float>(A) / 255.f);
}

constexpr COLOR_F::operator COLOR() const noexcept
{
	return COLOR(static_cast<unsigned __int8>(R * 255), static_cast<unsigned __int8>(G * 255), static_cast<unsigned __int8>(B * 255), static_cast<unsigned __int8>(A * 255));
}