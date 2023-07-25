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

	// Quaternion�\����
	struct Quaternion {
		Quaternion() :x(0.f), y(0.f), z(0.f), w(0.f) {}
		Quaternion(float X, float Y, float Z, float W) :x(X), y(Y), z(Z), w(W) {}
		float w;
		float x;
		float y;
		float z;
	};

	/*
		�N�H�[�^�j�I�����m�̊|���Z�̃I�[�o�[���[�h
	*/
	inline Quaternion operator*(const Quaternion& Q1, const Quaternion& Q2) {
		/*
			P(Px, Py, Pz, Pw) = P(Pv, Pw)�ƕ\����Ƃ���

			QP = (Qv, Qw) x (Pv, Qw)
			   = ((Qv x Pv + Pw * Qv + Qw * Pv), (Qw * Pw - Qv�EPv))
		*/
		Vector V1(Q1.x, Q1.y, Q1.z);
		Vector V2(Q2.x, Q2.y, Q2.z);
		float w = Q1.w * Q2.w - (V1 | V2);
		/* float w = Q1.w * Q2.w - Dot(V1, V2) */
		Vector Result = (V1 ^ V2) + V1 * Q2.w + V2 * Q1.w;
		/* VECTOR Result = VecAdd(VecAdd(VecCross(V1, V2), VecScale(V1, Q2.w), VecScale(V2, Q1.w)) */
		return Quaternion(Result.x, Result.y, Result.z, w);
	}

	//�v�Z�������܂Ƃ߂��N���X
	class MyMath
	{
	public:
		MyMath() = delete;
		~MyMath() = delete;
	public:
		//�����r�b�g�擾
		template<class T>
		constexpr static auto GetSignBit() ->typename std::enable_if<std::is_integral_v<T>, T>::type { return 1ull << ((sizeof(T) * 8) - 1); }
		//�t��
		template<class T>
		static constexpr T Inv(T a) { return T(1.0) / a; }
		//2��
		template<class T>
		static constexpr T Squared(const T a) { return a * a; }
		//n��
		template<class T>
		static constexpr T Exponentiation(const T a, const size_t exponent) { if (exponent == 0)return 1; else if (exponent == 1)return a; else return a * Exponentiation(a, exponent - 1); }
		/*��Βl*/
		template<class T>
		constexpr static T Abs(const T a) { return (a >= 0.) ? a : -a; }
		/*��Βl(Vector�̊e�v�f���Βl�ɂ���)*/
		template<>
		constexpr static Vector Abs(const Vector a) { return Vector((a.x >= 0.) ? a.x : -a.x, (a.y >= 0.) ? a.y : -a.y, (a.z >= 0.) ? a.z : -a.z); }

		//�ő�l
		template<class T>
		constexpr static T Max(const T& a, const T& b) { return (a > b) ? a : b; }
		//�ŏ��l
		template<class T>
		constexpr static T Min(const T& a, const T& b) { return (a < b) ? a : b; }
		//�ő�l
		template<class T>
		constexpr static T Max(const std::initializer_list<T> list) {
			T result = *list.begin();
			for (const T& n : list)	result = Max(result, n);
			return result;
		}
		//�ŏ��l
		template<class T>
		constexpr static T Min(const std::initializer_list<T> list) {
			T result = *list.begin();
			for (const T& n : list)	result = Min(result, n);
			return result;
		}

		//T�̍ő�l�擾
		template<class T>
		constexpr static auto GetMaxVal() ->typename std::enable_if<std::is_integral_v<T>, T>::type {
			T result = ~T(0ull);
			if constexpr (std::is_signed_v<T>) {
				result &= ~GetSignBit<T>();
			}
			return result;
		}
		//T�̍ő�l�擾
		template<class T>
		constexpr static auto GetMaxVal(T) ->typename std::enable_if<std::is_integral_v<T>, T>::type { return GetMaxVal<T>; }

		//Sin�g
		static float GetSin(float rad);
		//Sin�g(�͈͐����t��)
		static float GetSin(float min, float max, float rad);

		//a����b�֌����Ẵx�N�g�����K�����ꂽ�x�N�g����Ԃ�
		static Vector GetForwardVec(const Vector& a, const Vector& b);
		/*
			���E������
			viewpoint:���_, angle:���Ă����, pos:�Ώ�, fieldOfView:����p(���W�A��)
		*/
		static bool DecisionInField(const Vector& viewpoint, const Vector& angle, const Vector& pos, float fieldOfView = 1_PI_F);
		/*
			���E������
			viewpoint:���_, angle:���Ă����, pos:�Ώ�, fieldOfView:����p(���W�A��)
		*/
		static bool DecisionInField(const Vector2D& viewpoint, const Vector2D& angle, const Vector2D& pos, float fieldOfView = 1_PI_F);
		//�|���S���̖@���x�N�g�����擾
		static Vector	GetPolygonNormalVec(const Vector& vertexA, const Vector& vertexB, const Vector& vertexC);
		//�|���S���̖@���x�N�g�����擾
		static Vector	GetPolygonNormalVec(const POLYGON& polygon);
		//�|���S���̖@���x�N�g�����擾
		static Vector	GetPolygonCenterPos(const Vector& vertexA, const Vector& vertexB, const Vector& vertexC);
		//�|���S���̖@���x�N�g�����擾
		static Vector	GetPolygonCenterPos(const POLYGON& polygon);

		static float	GetTriangleHeightXY(const Vector& point, const POLYGON& polygon);
		static float	GetTriangleHeightXZ(const Vector& point, const POLYGON& polygon);
		static float	GetTriangleHeightYZ(const Vector& point, const POLYGON& polygon);

		static void GetAxisRotationQuaternion(const Vector& axis, float angle, Quaternion& out_quaA, Quaternion& out_quaB);// ����]�p�N�I�[�^�j�I���擾
		static Vector PointRotationQuaternion(const Vector& pos, const Vector& axis, float angle);// �_�̎���]
		static Quaternion QuaternionMult(const Quaternion& quaA, const Quaternion& quaB); // �N�I�[�^�j�I���~�N�I�[�^�j�I��
		static inline Matrix TransformQuaternionToRotMatrix(const Quaternion& q);

		// �A�[�N�^���W�F���g�֐�(UE����̈ڐA)
		static constexpr float Atan2(float y, float x);
		// �A�[�N�^���W�F���g�֐�(UE����̈ڐA)
		static constexpr double Atan2(double y, double x);
	};
}
#endif