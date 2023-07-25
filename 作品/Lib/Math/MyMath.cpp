#include "MyMath.h"
#include "../Constant/Constant.h"

namespace MyLib
{
	// ラジアン角からSin波を取得
	float MyMath::GetSin(float rad)
	{
		return sinf(rad);
	}

	// ラジアン角からmin～maxの範囲でSin波を取得
	float MyMath::GetSin(float min, float max, float rad)
	{
		float mult = (max - min) * 0.5f;
		return (sinf(rad) + 1.0f) * mult + min;
	}

	Vector MyMath::GetForwardVec(const Vector& a, const Vector& b)
	{
		return a.Create(b).Normalize();
	}

	bool MyMath::DecisionInField(const Vector& viewpoint, const Vector& angle, const Vector& pos, float fieldOfView)
	{
		return (((viewpoint.Create(pos) | angle) - 1.f) * -1_PI_F <= fieldOfView);
	}

	bool MyMath::DecisionInField(const Vector2D& viewpoint, const Vector2D& angle, const Vector2D& pos, float fieldOfView)
	{
		return (((viewpoint.Create(pos) | angle) - 1.f) * -1_PI_F <= fieldOfView);
	}

	Vector MyMath::GetPolygonNormalVec(const Vector& vertexA, const Vector& vertexB, const Vector& vertexC)
	{
		return (vertexA.Create(vertexB) ^ vertexA.Create(vertexC)).Normalize();
	}

	Vector MyMath::GetPolygonNormalVec(const POLYGON& polygon)
	{
		return (polygon.A.Create(polygon.B) ^ polygon.A.Create(polygon.C)).Normalize();
	}

	Vector MyMath::GetPolygonCenterPos(const Vector& vertexA, const Vector& vertexB, const Vector& vertexC)
	{
		return (vertexA + vertexB + vertexC) / 3;
	}

	Vector MyMath::GetPolygonCenterPos(const POLYGON& polygon)
	{
		return (polygon.A + polygon.B + polygon.C) / 3;
	}

	float MyMath::GetTriangleHeightXY(const Vector& point, const POLYGON& polygon)
	{
		// 面法線を計算
		Vector vecNorm = GetPolygonNormalVec(polygon);
		// 平面の方程式から高さ（Y座標を計算）
		return (-vecNorm.x * (point.x - polygon.A.x) - vecNorm.y * (point.y - polygon.A.y) + vecNorm.z * polygon.A.z) / vecNorm.z;
	}

	float MyMath::GetTriangleHeightXZ(const Vector& point, const POLYGON& polygon)
	{
		// 面法線を計算
		Vector vecNorm = GetPolygonNormalVec(polygon);
		// 平面の方程式から高さ（Y座標を計算）
		return (-vecNorm.x * (point.x - polygon.A.x) - vecNorm.z * (point.z - polygon.A.z) + vecNorm.y * polygon.A.y) / vecNorm.y;
	}

	float MyMath::GetTriangleHeightYZ(const Vector& point, const POLYGON& polygon)
	{
		// 面法線を計算
		Vector vecNorm = GetPolygonNormalVec(polygon);
		// 平面の方程式から高さ（Y座標を計算）
		return (-vecNorm.y * (point.y - polygon.A.y) - vecNorm.z * (point.z - polygon.A.z) + vecNorm.x * polygon.A.x) / vecNorm.x;
	}

	// 軸回転用クオータニオン取得
	// 入力引数
	// axis:回転軸
	// angle：角度
	// 引数出力
	// out_quaA:軸回転用クオータニオンA
	// out_quaB:軸回転用クオータニオンB
	void MyMath::GetAxisRotationQuaternion(const Vector& axis, float angle, Quaternion& out_quaA, Quaternion& out_quaB)
	{
		// クオータニオンA計算
		out_quaA.w = cosf(angle * 0.5f);
		out_quaA.x = axis.x * sinf(angle * 0.5f);
		out_quaA.y = axis.y * sinf(angle * 0.5f);
		out_quaA.z = axis.z * sinf(angle * 0.5f);

		// クオータニオンB計算
		out_quaB.w = cosf(angle * 0.5f);
		out_quaB.x = -axis.x * sinf(angle * 0.5f);
		out_quaB.y = -axis.y * sinf(angle * 0.5f);
		out_quaB.z = -axis.z * sinf(angle * 0.5f);
	}

	// 点の軸回転
	// pos:軸回転させる点の座標
	// axis：軸を表すベクトル
	// angle:回転角度
	Vector MyMath::PointRotationQuaternion(const Vector& pos, const Vector& axis, float angle)
	{
		Quaternion Q, R, result;
		GetAxisRotationQuaternion(axis, angle, Q, R);
		Quaternion P = { 0.0f, pos.x, pos.y, pos.z };
		result = QuaternionMult(QuaternionMult(R, P), Q);

		return Vector(result.x, result.y, result.z);
	}


	// クオータニオン×クオータニオン
	Quaternion MyMath::QuaternionMult(const Quaternion& quaA, const Quaternion& quaB)
	{
		Vector vecA(quaA.x, quaA.y, quaA.z);
		Vector vecB(quaB.x, quaB.y, quaB.z);
		Vector ima;

		// 虚数部計算
		ima = vecB * quaA.w;
		ima = ima + (vecA * quaB.w);
		ima = ima + (vecA ^ vecB);

		// 実数部計算
		float w = quaA.w * quaB.w - (vecA | vecB);

		// 結果返却
		return Quaternion(w, ima.x, ima.y, ima.z);
	}

	inline Matrix MyMath::TransformQuaternionToRotMatrix(const Quaternion& q) {
		Matrix result = Matrix::GetIdentityMat();

		result.m[0][0] = 1.f - 2.f * Squared(q.y) - 2.f * Squared(q.z);
		result.m[0][1] = 2.f * q.x * q.y - 2.f * q.w * q.z;
		result.m[0][2] = 2.f * q.x * q.z + 2.f * q.w * q.y;

		result.m[1][0] = 2.f * q.x * q.y + 2.f * q.w * q.z;
		result.m[1][1] = 1.f - 2.f * Squared(q.x) - 2.f * Squared(q.z);
		result.m[1][2] = 2.f * q.y * q.z - 2.f * q.w * q.x;

		result.m[2][0] = 2.f * q.x * q.z - 2.f * q.w * q.y;
		result.m[2][1] = 2.f * q.y * q.z + 2.f * q.w * q.x;
		result.m[2][2] = 1.f - 2.f * Squared(q.x) - 2.f * Squared(q.y);

		return result;
	}
	constexpr float MyMath::Atan2(float y, float x)
	{
		const float absX = Abs(x);
		const float absY = Abs(y);
		const bool yAbsBigger = (absY > absX);
		float t0 = yAbsBigger ? absY : absX;	// Max(absY, absX)
		float t1 = yAbsBigger ? absX : absY;	// Min(absX, absY)

		if (t0 == 0.f) return 0.f;

		float t3 = t1 / t0;
		float t4 = t3 * t3;

		CONSTANT_ARRAY(float, 7) c = {
			+7.2128853633444123e-03f,
			-3.5059680836411644e-02f,
			+8.1675882859940430e-02f,
			-1.3374657325451267e-01f,
			+1.9856563505717162e-01f,
			-3.3324998579202170e-01f,
			+1.0f
		};

		t0 = c[0];
		t0 = t0 * t4 + c[1];
		t0 = t0 * t4 + c[2];
		t0 = t0 * t4 + c[3];
		t0 = t0 * t4 + c[4];
		t0 = t0 * t4 + c[5];
		t0 = t0 * t4 + c[6];
		t3 = t0 * t3;

		t3 = yAbsBigger ? (0.5f * DX_PI_F) - t3 : t3;
		t3 = (x < 0.0f) ? DX_PI_F - t3 : t3;
		t3 = (y < 0.0f) ? -t3 : t3;

		return t3;
	}
	constexpr double MyMath::Atan2(double y, double x)
	{
		const double absX = Abs(x);
		const double absY = Abs(y);
		const bool yAbsBigger = (absY > absX);
		double t0 = yAbsBigger ? absY : absX;	// Max(absY, absX)
		double t1 = yAbsBigger ? absX : absY;	// Min(absX, absY)

		if (t0 == 0.0) return 0.0;

		double t3 = t1 / t0;
		double t4 = t3 * t3;

		CONSTANT_ARRAY(double, 7) c = {
			+7.2128853633444123e-03,
			-3.5059680836411644e-02,
			+8.1675882859940430e-02,
			-1.3374657325451267e-01,
			+1.9856563505717162e-01,
			-3.3324998579202170e-01,
			+1.0
		};

		t0 = c[0];
		t0 = t0 * t4 + c[1];
		t0 = t0 * t4 + c[2];
		t0 = t0 * t4 + c[3];
		t0 = t0 * t4 + c[4];
		t0 = t0 * t4 + c[5];
		t0 = t0 * t4 + c[6];
		t3 = t0 * t3;

		t3 = yAbsBigger ? (0.5 * DX_PI) - t3 : t3;
		t3 = (x < 0.0) ? DX_PI - t3 : t3;
		t3 = (y < 0.0) ? -t3 : t3;

		return t3;
	}
}