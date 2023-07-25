#pragma once
#include "typedef.h"
#include <compare>

namespace LiteralParameter
{
	//1行の高さ
	constexpr int ROW_HEIGHT = 16;
	constexpr double PI = 3.1415926535897932384626433832795;
	constexpr float PI_F = 3.1415926535897932384626433832795f;
	constexpr size_t KB = 1'024;
	constexpr size_t MB = 1'048'576;
	constexpr size_t GB = 1'073'741'824;
}

constexpr u64 ConvSecondsToNanoSecond(u64 sec) { return sec * static_cast<u64>(1e9); }
constexpr u64 ConvSecondsToMicroSecond(u64 sec) { return sec * static_cast <u64>(1e7); }
constexpr ldouble ConvNanosecondToSeconds(u64 sec) { return static_cast<ldouble>(sec) * 1e-9; }
constexpr ldouble ConvNanosecondToSeconds(ldouble sec) { return  sec * 1e-9; }

//明示の為の修飾子
constexpr size_t operator""_B(u64 num) { return (size_t)num; }
//nKBを返す
constexpr size_t operator""_KB(u64 num) { return (size_t)(num * LiteralParameter::KB); }
//nMBを返す
constexpr size_t operator""_MB(u64 num) { return (size_t)(num * LiteralParameter::MB); }
//nGBを返す
constexpr size_t operator""_GB(u64 num) { return (size_t)(num * LiteralParameter::GB); }

//nKBを返す
constexpr size_t operator""_KB(ldouble num) { return (size_t)(num * LiteralParameter::KB); }
//nMBを返す
constexpr size_t operator""_MB(ldouble num) { return (size_t)(num * LiteralParameter::MB); }
//nGBを返す
constexpr size_t operator""_GB(ldouble num) { return (size_t)(num * LiteralParameter::GB); }

constexpr i8 operator""_i8(u64 num) { return static_cast<i8>(num); }
constexpr u8 operator""_u8(u64 num) { return static_cast<u8>(num); }
constexpr i16 operator""_i16(u64 num) { return static_cast<i16>(num); }
constexpr u16 operator""_u16(u64 num) { return static_cast<u16>(num); }
constexpr i32 operator""_i32(u64 num) { return static_cast<i32>(num); }
constexpr u32 operator""_u32(u64 num) { return static_cast<u32>(num); }
constexpr i64 operator""_i64(u64 num) { return static_cast<i64>(num); }
constexpr u64 operator""_u64(u64 num) { return static_cast<u64>(num); }

//nπを返す
constexpr double operator""_π(ldouble num) { return num * LiteralParameter::PI; }
//nπを返す
constexpr double operator""_PI(ldouble num) { return num * LiteralParameter::PI; }
//nπを返す
constexpr double operator""_PI(u64 num) { return (ldouble)(num)*LiteralParameter::PI; }
//nπを返す
constexpr float operator""_PI_F(ldouble num) { return (float)(num)*LiteralParameter::PI_F; }
//nπを返す
constexpr float operator""_PI_F(u64 num) { return (float)(num)*LiteralParameter::PI_F; }

//度数法をラジアン角に変換
constexpr double operator""_DegreeToRadian(u64 num) { return (ldouble)(num) * (1_PI / 180); }
//度数法をラジアン角に変換
constexpr double operator""_DegreeToRadian(ldouble num) { return num * (1_PI / 180); }
//度数法をラジアン角に変換
constexpr float operator""_DegreeToRadian_F(u64 num) { return (float)(num) * (1_PI_F / 180); }
//度数法をラジアン角に変換
constexpr float operator""_DegreeToRadian_F(ldouble num) { return (float)(num) * (1_PI_F / 180); }

//ラジアンを度数法に変換
constexpr double operator""_RadianToDegree(ldouble Angle) { return Angle * 180 / 1_PI; }
//ラジアンを度数法に変換
constexpr float operator""_RadianToDegree_F(ldouble Angle) { return (float)Angle * (float)(180 / 1_PI_F); }


//DxLibのDrawString基準のn行目のy座標を返す
constexpr i32 operator""_ROW(u64 num) {
	return static_cast<i32>(num) * LiteralParameter::ROW_HEIGHT;
}

union u64_char {
private:
	constexpr u64 StrTo64(const char* str, const _In_range_(0, 9)size_t size) {
		u64 result = 0;
		for (size_t i = 0; i < 8 && i < size && str[i] != '\0'; i++) {
			result |= (static_cast<u64>(str[i])) << i * 8;
		}
		return result;
	}
public:
	constexpr u64_char() :num(0) {}
	constexpr u64_char(u64 n) : num(n) {}
	template<size_t len>
	constexpr u64_char(char n[len]) : num(StrTo64(n, len)) {}

	constexpr auto operator<=>(const u64_char& b) {
		if (num != b.num) {
			return num <=> b.num;
		}
		return std::strong_ordering::equivalent;
	}

	operator const u64& ()const { return num; }
	operator u64& () { return num; }
private:
	char str[8];
	u64 num;
};

//文字列を符号なし64ビット整数に変換する
constexpr u64 operator""_int64(const char* str, const _In_range_(0, 9)size_t size) {
	u64 result = 0;
	for (size_t i = 0; i < 8 && i < size && str[i] != '\0'; i++) {
		result |= (static_cast<u64>(str[i])) << i * 8;
	}
	return result;
}