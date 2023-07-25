#ifndef _MYMATH_H_
#define _MYMATH_H_

#include "DxLib.h"
#include "MyMathData.h"
#include "../Constant/Constant.h"
#include "../Literal.h"
#include <initializer_list>

#define MAX(...) MyLib::MyMath::Max({__VA_ARGS__})
#define MIN(...) MyLib::MyMath::Min({__VA_ARGS__})
namespace MyLib
{
	struct POLYGON
	{
		POLYGON() :A(), B(), C() {}
		POLYGON(Vector a, Vector b, Vector c) { A = a; B = b; C = c; }
		Vector A, B, C;
	};

	// Quaternion構造体
	struct Quaternion {
		Quaternion() :x(0.f), y(0.f), z(0.f), w(0.f) {}
		Quaternion(float X, float Y, float Z, float W) :x(X), y(Y), z(Z), w(W) {}
		float w;
		float x;
		float y;
		float z;
	};

	/*
		クォータニオン同士の掛け算のオーバーロード
	*/
	inline Quaternion operator*(const Quaternion& Q1, const Quaternion& Q2) {
		/*
			P(Px, Py, Pz, Pw) = P(Pv, Pw)と表せるとする

			QP = (Qv, Qw) x (Pv, Qw)
			   = ((Qv x Pv + Pw * Qv + Qw * Pv), (Qw * Pw - Qv・Pv))
		*/
		Vector V1(Q1.x, Q1.y, Q1.z);
		Vector V2(Q2.x, Q2.y, Q2.z);
		float w = Q1.w * Q2.w - (V1 | V2);
		/* float w = Q1.w * Q2.w - Dot(V1, V2) */
		Vector Result = (V1 ^ V2) + V1 * Q2.w + V2 * Q1.w;
		/* VECTOR Result = VecAdd(VecAdd(VecCross(V1, V2), VecScale(V1, Q2.w), VecScale(V2, Q1.w)) */
		return Quaternion(Result.x, Result.y, Result.z, w);
	}

	//計算処理をまとめたクラス
	class MyMath
	{
	public:
		MyMath() = delete;
		~MyMath() = delete;
	public:
		//符号ビット取得
		template<class T>
		constexpr static auto GetSignBit() ->typename std::enable_if<std::is_integral_v<T>, T>::type { return 1ull << ((sizeof(T) * 8) - 1); }
		//逆数
		template<class T>
		static constexpr T Inv(T a) { return T(1.0) / a; }
		//2乗
		template<class T>
		static constexpr T Squared(const T a) { return a * a; }
		//n乗
		template<class T>
		static constexpr T Exponentiation(const T a, const size_t exponent) { if (exponent == 0)return 1; else if (exponent == 1)return a; else return a * Exponentiation(a, exponent - 1); }
		/*絶対値*/
		template<class T>
		constexpr static T Abs(const T a) { return (a >= 0.) ? a : -a; }
		/*絶対値(Vectorの各要素を絶対値にする)*/
		template<>
		constexpr static Vector Abs(const Vector a) { return Vector((a.x >= 0.) ? a.x : -a.x, (a.y >= 0.) ? a.y : -a.y, (a.z >= 0.) ? a.z : -a.z); }

		//最大値
		template<class T>
		constexpr static T Max(const T& a, const T& b) { return (a > b) ? a : b; }
		//最小値
		template<class T>
		constexpr static T Min(const T& a, const T& b) { return (a < b) ? a : b; }
		//最大値
		template<class T>
		constexpr static T Max(const std::initializer_list<T> list) {
			T result = *list.begin();
			for (const T& n : list)	result = Max(result, n);
			return result;
		}
		//最小値
		template<class T>
		constexpr static T Min(const std::initializer_list<T> list) {
			T result = *list.begin();
			for (const T& n : list)	result = Min(result, n);
			return result;
		}

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

		//Sin波
		static float GetSin(float rad);
		//Sin波(範囲制限付き)
		static float GetSin(float min, float max, float rad);

		//aからbへ向けてのベクトル正規化されたベクトルを返す
		static Vector GetForwardVec(const Vector& a, const Vector& b);
		/*
			視界内判定
			viewpoint:視点, angle:見てる方向, pos:対象, fieldOfView:視野角(ラジアン)
		*/
		static bool DecisionInField(const Vector& viewpoint, const Vector& angle, const Vector& pos, float fieldOfView = 1_PI_F);
		/*
			視界内判定
			viewpoint:視点, angle:見てる方向, pos:対象, fieldOfView:視野角(ラジアン)
		*/
		static bool DecisionInField(const Vector2D& viewpoint, const Vector2D& angle, const Vector2D& pos, float fieldOfView = 1_PI_F);
		//ポリゴンの法線ベクトルを取得
		static Vector	GetPolygonNormalVec(const Vector& vertexA, const Vector& vertexB, const Vector& vertexC);
		//ポリゴンの法線ベクトルを取得
		static Vector	GetPolygonNormalVec(const POLYGON& polygon);
		//ポリゴンの法線ベクトルを取得
		static Vector	GetPolygonCenterPos(const Vector& vertexA, const Vector& vertexB, const Vector& vertexC);
		//ポリゴンの法線ベクトルを取得
		static Vector	GetPolygonCenterPos(const POLYGON& polygon);

		static float	GetTriangleHeightXY(const Vector& point, const POLYGON& polygon);
		static float	GetTriangleHeightXZ(const Vector& point, const POLYGON& polygon);
		static float	GetTriangleHeightYZ(const Vector& point, const POLYGON& polygon);

		static void GetAxisRotationQuaternion(const Vector& axis, float angle, Quaternion& out_quaA, Quaternion& out_quaB);// 軸回転用クオータニオン取得
		static Vector PointRotationQuaternion(const Vector& pos, const Vector& axis, float angle);// 点の軸回転
		static Quaternion QuaternionMult(const Quaternion& quaA, const Quaternion& quaB); // クオータニオン×クオータニオン
		static inline Matrix TransformQuaternionToRotMatrix(const Quaternion& q);

		// アークタンジェント関数(UEからの移植)
		static constexpr float Atan2(float y, float x);
		// アークタンジェント関数(UEからの移植)
		static constexpr double Atan2(double y, double x);
	};
}
#endif