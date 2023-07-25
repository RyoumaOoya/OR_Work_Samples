#pragma once
#include "DxLib.h"
#include <math.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <compare>
#include "../Literal.h"

namespace MyLib
{
	class Matrix;
	namespace MData {
		
		namespace {
			template<class T>
			inline constexpr T Abs(T a) { return (a >= 0.) ? a : -a; }
			template<class T>
			inline constexpr T Squared(const T a) { return a * a; }
			inline constexpr float Atan2(float y, float x) {
				const float absX = Abs(x);
				const float absY = Abs(y);
				const bool yAbsBigger = (absY > absX);
				float t0 = yAbsBigger ? absY : absX;	// Max(absY, absX)
				float t1 = yAbsBigger ? absX : absY;	// Min(absX, absY)

				if (t0 == 0.f) return 0.f;

				float t3 = t1 / t0;
				float t4 = t3 * t3;

				constexpr float c[7] = {
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
		}

		template<typename InElementType>
		class Vector2D;
		template<typename InElementType>
		class Vector;

		template<typename InElementType>
		class Vector2D
		{
		private:
			typedef InElementType ElementType;

		public:	//スタティック関数
			inline static constexpr Vector2D<ElementType> ZEROVEC() { return Vector2D<ElementType>(static_cast<ElementType>(0), static_cast<ElementType>(0)); }
		public:
			constexpr Vector2D() :x(0), y(0) {}
			constexpr Vector2D(ElementType initX, ElementType initY) : x(initX), y(initY) {}
			constexpr Vector2D(const Vector2D& init) : x(static_cast<ElementType>(init.x)), y(static_cast<ElementType>(init.y)) {}
			constexpr Vector2D(const VECTOR& init) : x(static_cast<ElementType>(init.x)), y(static_cast<ElementType>(init.y)) {}
			constexpr Vector2D(VECTOR&& init) : x(static_cast<ElementType>(init.x)), y(static_cast<ElementType>(init.y)) {}

			//ベクトル生成
			inline constexpr Vector2D<ElementType> Create(const Vector2D& b) const { return b - *this; }

			/*
				ベクトルの正規化
				戻り値: 正規化されたベクトル
			*/
			constexpr Vector2D<ElementType> Normalize() const {
				//インライン化していないのはsize変数の命名被りによるエラー回避の為
				float size = this->Long();
				if (size == 0.f || size == 1.f)
					return *this;

				return Vector2D<ElementType>(static_cast<ElementType>(x / size), static_cast<ElementType>(y / size));
			}

			/*
				入力ベクトルを減算したベクトルの長さを返す
				戻り値: ベクトルの長さ
			*/
			inline constexpr float Long() const { return sqrtf(static_cast<float>(Squared(x) + Squared(y))); }
			/*
				入力ベクトルを減算したベクトルの長さを返す
				引数: b 減算するベクトル
				戻り値: ベクトルの長さ
			*/
			inline constexpr float Long(const Vector2D& b) const { return sqrtf(static_cast<float>(Squared(x - b.x) + Squared(y - b.y))); }
			/*
				入力ベクトルを減算したベクトルの長さの2乗を返す
				戻り値: ベクトルの長さの2乗
			*/
			inline constexpr float LongSq() const { return  static_cast<float>(Squared(x) + Squared(y)); }
			/*
				入力ベクトルを減算したベクトルの長さの2乗を返す
				引数: b 減算するベクトル
				戻り値: ベクトルの長さの2乗
			*/
			inline constexpr float LongSq(const Vector2D& b) const { return  static_cast<float>(Squared(x - b.x) + Squared(y - b.y)); }

			/*
				ベクトルAからベクトルBへの移動
				引数: b 移動先, moveLen 移動量
				戻り値: 移動後のベクトル
			*/
			inline constexpr Vector2D<ElementType> Move(const Vector2D& b, const float moveLen) const { return *this + (this->Create(b).Normalize() * moveLen); }

			/*
				ベクトルの回転
				引数: angle:回転させる角度
				戻り値: 回転したベクトル
			*/
			inline Vector2D<ElementType> Rotation(float angle) const {
				Vector2D<ElementType> result;
				result.x = x * cosf(angle) - y * sinf(angle);
				result.y = x * sinf(angle) + y * cosf(angle);
				return result;
			}

			inline constexpr bool isZeroVector()const { return *this == ZEROVEC(); }
		public:		//オペレーターのオーバロード宣言
			//数値設定
			inline Vector2D<ElementType> operator()(const ElementType setX, const ElementType setY) { x = setX; y = setY; return *this; }
			inline Vector2D<ElementType> operator()(const Vector2D& b) { x = b.x; y = b.y; return *this; }
			/*
				ベクトルの加算
				引数: b 加算するベクトル
				戻り値: 加算したベクトル
			*/
			inline constexpr Vector2D<ElementType> operator+(const Vector2D& b) const { return Vector2D<ElementType>(x + b.x, y + b.y); }
			/*
				ベクトルの加算
				引数: b 加算するベクトル
				戻り値: 加算したベクトルのコピー
			*/
			inline Vector2D<ElementType> operator+=(const Vector2D& b) { return *this = *this + b; }
			/*
				入力された数値を各要素に加算する
				引数: b 加算する数値
				戻り値: 加算したベクトル
			*/
			inline constexpr Vector2D<ElementType> operator+(const ElementType& b) const { return Vector2D<ElementType>((x + b), (y + b)); }
			/*
				入力された数値を各要素に加算する
				引数: b 加算する数値
				戻り値: 加算したベクトルのコピー
			*/
			inline Vector2D<ElementType> operator+=(const ElementType& b) { return *this = *this + b; }

			/*
				ベクトルの減算
				引数: b 減算するベクトル
				戻り値: 減算したベクトル
			*/
			inline constexpr Vector2D<ElementType> operator-(const Vector2D& b) const { return Vector2D<ElementType>(x - b.x, y - b.y); }
			/*
				ベクトルの減算
				引数: b 減算するベクトル
				戻り値: 減算したベクトルのコピー
			*/
			inline Vector2D<ElementType> operator-=(const Vector2D& b) { return *this = *this - b; }
			/*
				入力された数値を各要素に減算する
				引数: b 減算する数値
				戻り値: 減算したベクトル
			*/
			inline constexpr Vector2D<ElementType> operator-(const ElementType& b) const { return Vector2D<ElementType>((x - b), (y - b)); }
			/*
				入力された数値を各要素に減算する
				引数: b 減算する数値
				戻り値: 減算したベクトルのコピー
			*/
			inline Vector2D<ElementType> operator-=(const ElementType& b) { return *this = *this - b; }

			/*
				ベクトルのスケーリング(x * x, y * y, z * z)
				引数: b スケーリングする量
				戻り値: スケーリングしたベクトル
			*/
			inline constexpr Vector2D<ElementType> operator*(const Vector2D& b) const { return Vector2D<ElementType>((ElementType)(x * b.x), (ElementType)(y * b.y)); }
			/*
				ベクトルのスケーリング
				引数: b スケーリングする量
				戻り値: スケーリングしたベクトル
			*/
			inline constexpr Vector2D<ElementType> operator*(const double& b) const { return Vector2D<ElementType>((ElementType)(x * b), (ElementType)(y * b)); }
			/*
				ベクトルのスケーリング(x * x, y * y, z * z)
				引数: b スケーリングする量
				戻り値: スケーリングしたベクトル
			*/
			inline double operator*=(const Vector2D& b) { return *this = *this * b; }
			/*
				ベクトルのスケーリング
				引数: b スケーリングする量
				戻り値: スケーリングしたベクトルのコピー
			*/
			inline Vector2D<ElementType> operator*=(const double& b) { return *this = *this * b; }
			/*
				ベクトルのスケーリング(除算)(x / x, y / y, z / z)
				引数: b 除算する数値(ベクトル)
				戻り値: スケーリングしたベクトルのコピー
			*/
			inline constexpr Vector2D<ElementType> operator/(const Vector2D& b) { return Vector2D<ElementType>(x / b.x, y / b.y); }
			/*
				ベクトルのスケーリング(除算)
				引数: b 除算する数値
				戻り値: スケーリングしたベクトルのコピー
			*/
			inline constexpr Vector2D<ElementType> operator/(const double& b) { return Vector2D<ElementType>(x / b, y / b); }
			/*
				ベクトルのスケーリング(除算)(x / x, y / y, z / z)
				引数: b 除算する数値(ベクトル)
				戻り値: スケーリングしたベクトルのコピー
			*/
			inline Vector2D<ElementType> operator/=(const Vector2D& b) { return *this = *this / b; }
			/*
				ベクトルのスケーリング(除算)
				引数: b 除算する数値
				戻り値: スケーリングしたベクトルのコピー
			*/
			inline Vector2D<ElementType> operator/=(const double& b) { return *this = *this / b; }
			/*
				自身と入力ベクトルとの外積
				引数: b 入力ベクトル
				戻り値: 外積
			*/
			inline constexpr double operator^(const Vector2D& b) const { return (x * b.y - y * b.x); }
			/*
				自身と入力ベクトルとの内積
				引数: b 入力ベクトル
				戻り値: 内積
			*/
			inline constexpr float operator|(const Vector2D& b) const { return (x * b.x + y * b.y); }
			/*
				ベクトルの反転
				戻り値:反転したベクトル
			*/
			inline constexpr Vector2D<ElementType> operator-() const { return Vector2D<ElementType>(-x, -y); }

			inline Vector2D<ElementType> operator=(const VECTOR& b) { return *this(static_cast<ElementType>(b.x), static_cast<ElementType>(b.y)); }
			inline Vector2D<ElementType> operator=(VECTOR&& b) { return *this(static_cast<ElementType>(b.x), static_cast<ElementType>(b.y)); }
			//ベクトルの要素が同一か
			inline constexpr bool operator==(const Vector2D<ElementType>& b) const { return (x == b.x && y == b.y); }
			inline constexpr bool operator!=(const Vector2D<ElementType>& b) const { return !(*this == b); }

			//ベクトルの長さを比較
			inline constexpr auto operator<=>(const float b) {
				float a_long = LongSq(), b_long = b * b;
				if (a_long != b_long) {
					return a_long <=> b_long;
				}
				return std::partial_ordering::equivalent;
			}

			template<class T> constexpr explicit
			operator Vector2D<T>() const { return Vector2D<T>((T)x, (T)y); }

			//DXライブラリベクトル構造体へのキャスト演算子
			inline constexpr explicit operator VECTOR() const noexcept { return VECTOR{ static_cast<float>(x), static_cast<float>(y), 0.f }; }
			//3Dベクトルへのキャスト演算子
			template<class T> constexpr explicit
			operator Vector<T>() const noexcept { return Vector<T>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(0)); }

		public:
			ElementType x, y;
		};

		template<typename InElementType>
		class Vector
		{
		private:
			typedef InElementType ElementType;

		public:	//スタティック関数
			inline static constexpr Vector<ElementType> ZEROVEC() { return Vector<ElementType>(static_cast<ElementType>(0), static_cast<ElementType>(0), static_cast<ElementType>(0)); }

		public:
			constexpr Vector() :x(0), y(0), z(0) {}
			constexpr Vector(const ElementType& initX, const ElementType& initY, const ElementType& initZ) : x(initX), y(initY), z(initZ) {}
			constexpr Vector(ElementType&& initX, ElementType&& initY, ElementType&& initZ) : x(initX), y(initY), z(initZ) {}
			constexpr Vector(const VECTOR& init) : x(init.x), y(init.y), z(init.z) {}
			constexpr Vector(VECTOR&& init) : x(init.x), y(init.y), z(init.z) {}

			inline Vector<ElementType> SetX(const ElementType& value) { x = value; return *this; }
			inline Vector<ElementType> SetX(ElementType&& value)	  { x = value; return *this; }

			inline Vector<ElementType> SetY(const ElementType& value) { y = value; return *this; }
			inline Vector<ElementType> SetY(ElementType&& value)	  { y = value; return *this; }

			inline Vector<ElementType> SetZ(const ElementType& value) { z = value; return *this; }
			inline Vector<ElementType> SetZ(ElementType&& value)	  { z = value; return *this; }

			//ベクトル生成
			inline constexpr Vector<ElementType> Create(const Vector& b) const { return b - *this; }
			inline constexpr Vector<ElementType> Create(Vector&& b) const { return b - *this; }

			/*
				ベクトルの正規化
				戻り値: 正規化されたベクトル
			*/
			constexpr Vector<ElementType> Normalize() const {
				//インライン化していないのはsize変数の命名被りによるエラー回避の為
				float size = this->Long();
				if (size == 0.f || size == 1.f)
					return *this;
				else
					return Vector<ElementType>(static_cast<ElementType>(x / size), static_cast<ElementType>(y / size), static_cast<ElementType>(z / size));
			}
			/*
				入力ベクトルを減算したベクトルの長さを返す
				戻り値: ベクトルの長さ
			*/
			inline constexpr float Long() const {
				return sqrtf(Squared(static_cast<float>(x)) + Squared(static_cast<float>(y)) + Squared(static_cast<float>(z)));
			}
			/*
				入力ベクトルを減算したベクトルの長さを返す
				引数: v 減算するベクトル
				戻り値: ベクトルの長さ
			*/
			inline constexpr float Long(const Vector& v) const {
				return sqrtf(Squared(static_cast<float>(x) - static_cast<float>(v.x)) + Squared(static_cast<float>(y) - static_cast<float>(v.y)) + Squared(static_cast<float>(z) - static_cast<float>(v.z)));
			}
			/*
				入力ベクトルを減算したベクトルの長さの2乗を返す
				戻り値: ベクトルの長さの2乗
			*/
			inline constexpr float LongSq() const { 
				return Squared(static_cast<float>(x)) + Squared(static_cast<float>(y)) + Squared(static_cast<float>(z)); 
			}
			/*
				入力ベクトルを減算したベクトルの長さの2乗を返す
				引数: v 減算するベクトル
				戻り値: ベクトルの長さの2乗
			*/
			inline constexpr float LongSq(const Vector& v) const { 
				return Squared(static_cast<float>(x) - static_cast<float>(v.x)) + Squared(static_cast<float>(y) - static_cast<float>(v.y)) + Squared(static_cast<float>(z) - static_cast<float>(v.z));
			}
			/*
				XY平面でのベクトルの長さを返す
				戻り値: XY平面でのベクトルの長さ
			*/
			inline constexpr float LongXY()		const { return sqrtf(Squared(static_cast<float>(x)) + Squared(static_cast<float>(y))); }
			/*
				XY平面でのベクトルの長さの2乗を返す
				戻り値: XY平面でのベクトルの長さの2乗
			*/
			inline constexpr float LongXYSq()	const { return		 Squared(static_cast<float>(x)) + Squared(static_cast<float>(y)); }
			/*
				YZ平面でのベクトルの長さを返す
				戻り値: YZ平面でのベクトルの長さ
			*/
			inline constexpr float LongYZ()		const { return sqrtf(Squared(static_cast<float>(y)) + Squared(static_cast<float>(z))); }
			/*
				YZ平面でのベクトルの長さの2乗を返す
				戻り値: YZ平面でのベクトルの長さの2乗
			*/
			inline constexpr float LongYZSq()	const { return		 Squared(static_cast<float>(y)) + Squared(static_cast<float>(z)); }
			/*
				XZ平面でのベクトルの長さを返す
				戻り値: XZ平面でのベクトルの長さ
			*/
			inline constexpr float LongXZ()		const { return sqrtf(Squared(static_cast<float>(x)) + Squared(static_cast<float>(z))); }
			/*
				XZ平面でのベクトルの長さの2乗を返す
				戻り値: XZ平面でのベクトルの長さの2乗
			*/
			inline constexpr float LongXZSq()	const { return		 Squared(static_cast<float>(x)) + Squared(static_cast<float>(z)); }
			/*
				座標を中心に回転、引き伸ばしした座標を返す
				引数: rotation 回転値, distance 引き伸ばす距離
				戻り値: 回転、引き伸ばしした座標
			*/
			template<class T>
			constexpr Vector<ElementType> RotationMove(const Vector2D<T>& rotation, const float& distance) const;
			/*
				座標を中心に回転、引き伸ばしした座標を返す
				引数: rotation 回転値, distance 引き伸ばす距離
				戻り値: 回転、引き伸ばしした座標
			*/
			template<class T>
			constexpr Vector<ElementType> RotationMove(const Vector2D<T>& rotation, const Vector& distance) const;

			/*
				ベクトルの角度を取得
				向きベクトル用
			*/
			inline constexpr Vector2D<float> GetRotationXY() const { return Vector2D<float>(Atan2(y, Vector2D<float>(Abs<float>(x), Abs<float>(z)).Long()), Atan2(x, z)); }
			/*
				ベクトルBへの角度を取得
				向きベクトル用
			*/
			inline constexpr Vector2D<float> GetFollowRotationXY(const Vector& b) const { return this->Create(b).GetRotationXY(); }

			/*
				ベクトルの角度を取得
				上から見た角度
			*/
			inline constexpr float GetRotationXZ() const { return Atan2(x, z); }
			/*
				ベクトルBへの角度を取得
				上から見た角度
			*/
			inline constexpr float GetFollowRotationXZ(const Vector& b) const { return this->Create(b).GetRotationXZ(); }

			inline constexpr Vector<double> GetRotation() const { return Vector<double>(atan2(-y, z), atan2(x, z) + DX_PI, atan2(x, -y)); }
			/*
				ベクトルを回転させる
				引数: rotation 回転ベクトル
				戻り値: 回転後のベクトル
			*/
			inline constexpr Vector<ElementType> Rotation(const Vector& rotation) const { return (Matrix::GetRotationMat(rotation)) * *this; }
			/*
				ベクトルBへの移動
				引数: b 移動先, moveLen 移動量
				戻り値: 移動後のベクトル
			*/
			inline constexpr Vector<ElementType> Move(const Vector& b, const float moveLen) const { return *this + (this->Create(b).Normalize() * moveLen); }
			/*
				縦に垂直なベクトルを取得
			*/
			inline constexpr Vector<ElementType> GetNormVecX() const {
				return Rotation(Vector<ElementType>(static_cast<ElementType>(0.5 * DX_PI_F), static_cast<ElementType>(0), static_cast<ElementType>(0))) ^ *this;
			}
			/*
				横に垂直なベクトルを取得
			*/
			inline constexpr Vector<ElementType> GetNormVecY() const {
				return Rotation(Vector<ElementType>(static_cast<ElementType>(0), static_cast<ElementType>(0.5 * DX_PI_F), static_cast<ElementType>(0))) ^ *this;
			}
			/*
				奥に垂直なベクトルを取得
			*/
			inline constexpr Vector<ElementType> GetNormVecZ() const {
				return Rotation(Vector<ElementType>(static_cast<ElementType>(0), static_cast<ElementType>(0), static_cast<ElementType>(0.5 * DX_PI_F))) ^ *this;
			}
			/*
				回転値から右向きを表すベクトルを取得
			*/
			inline constexpr Vector<ElementType> GetAxisX() const {
				return Rotation(Vector<ElementType>(static_cast<ElementType>(1), static_cast<ElementType>(0), static_cast<ElementType>(0)));
			}
			/*
				回転値から上向きを表すベクトルを取得
			*/
			inline constexpr Vector<ElementType> GetAxisY() const {
				return Rotation(Vector<ElementType>(static_cast<ElementType>(0), static_cast<ElementType>(1), static_cast<ElementType>(0)));
			}
			/*
				回転値から奥向きを表すベクトルを取得
			*/
			inline constexpr Vector<ElementType> GetAxisZ() const {
				return Rotation(Vector<ElementType>(static_cast<ElementType>(0), static_cast<ElementType>(0), static_cast<ElementType>(1)));
			}
			inline constexpr bool isZeroVector()const {
				return *this == ZEROVEC();
			}
		public:		//オペレーターのオーバロード宣言

			//数値設定
			_inline Vector<ElementType> operator()(const ElementType setX, const ElementType setY, const ElementType setZ) { x = setX; y = setY; z = setZ; return *this; }
			inline Vector<ElementType> operator()(const Vector& v) { x = v.x; y = v.y; z = v.z; return *this; }
			inline Vector<ElementType> operator()(const VECTOR& v) { x = v.x; y = v.y; z = v.z; return *this; }
			/*
				ベクトルの加算
				引数: 加算するベクトル
				戻り値: 加算したベクトル
			*/
			inline constexpr Vector<ElementType> operator+(const Vector& b) const { 
				return Vector<ElementType>(static_cast<ElementType>(x + b.x), static_cast<ElementType>(y + b.y), static_cast<ElementType>(z + b.z));
			}
			/*
				ベクトルの加算
				引数: 加算するベクトル
				戻り値: 加算したベクトルのコピー
			*/
			inline Vector<ElementType> operator+=(const Vector& b) { return *this = *this + b; }
			/*
				入力された数値を各要素に加算する
				引数: 加算する数値
				戻り値: 加算したベクトル
			*/
			inline constexpr Vector<ElementType> operator+(const ElementType& b) const { return Vector<ElementType>(x + b, y + b, z + b); }
			/*
				入力された数値を各要素に加算する
				引数: 加算する数値
				戻り値: 加算したベクトルのコピー
			*/
			inline Vector<ElementType> operator+=(const ElementType& b) { return *this = *this + b; }

			/*
				ベクトルの減算
				引数: 減算するベクトル
				戻り値: 減算したベクトル
			*/
			inline constexpr Vector<ElementType> operator-(const Vector& b) const { return Vector<ElementType>(static_cast<ElementType>(x - b.x), static_cast<ElementType>(y - b.y), static_cast<ElementType>(z - b.z)); }
			/*
				ベクトルの減算
				引数: 減算するベクトル
				戻り値: 減算したベクトルのコピー
			*/
			inline Vector<ElementType> operator-=(const Vector& b) { return *this = *this - b; }
			/*
				入力された数値を各要素に減算する
				引数: 減算する数値
				戻り値: 減算したベクトル
			*/
			inline constexpr Vector<ElementType> operator-(const ElementType& b) const { return Vector<ElementType>(x - b, y - b, z - b); }
			/*
				入力された数値を各要素に減算する
				引数: 減算する数値
				戻り値: 減算したベクトルのコピー
			*/
			inline Vector<ElementType> operator-=(const ElementType& b) { return *this = *this - b; }

			/*
				ベクトルのスケーリング
				引数: v スケーリングするベクトル
				戻り値: スケーリングしたベクトル
			*/
			inline constexpr Vector<ElementType> operator*(const Vector& b) const { return Vector(x * b.x, y * b.y, z * b.z); }
			/*
				ベクトルのスケーリング
				引数: scale スケーリングする量
				戻り値: スケーリングしたベクトル
			*/
			inline constexpr Vector<ElementType> operator*(const double& b) const { return Vector<ElementType>(static_cast<ElementType>(x * b), static_cast<ElementType>(y * b), static_cast<ElementType>(z * b)); }
			/*
				ベクトルのスケーリング
				引数: v スケーリングするベクトル
				戻り値: スケーリングしたベクトルのコピー
			*/
			inline Vector<ElementType> operator*=(const Vector& b) { return *this = *this * b; }
			/*
				ベクトルのスケーリング
				引数: scale スケーリングする量
				戻り値: スケーリングしたベクトルのコピー
			*/
			inline Vector<ElementType> operator*=(const ElementType b) { return *this = *this * b; }
			/*
				ベクトルのスケーリング(除算)
				引数: scale 除算する数値(ベクトル)
				戻り値: スケーリングしたベクトルのコピー
			*/
			inline constexpr Vector<ElementType> operator/(const Vector& b) { return Vector<ElementType>(x / b.x, y / b.y, z / b.z); }
			/*
				ベクトルのスケーリング(除算)
				引数: scale 除算する数値
				戻り値: スケーリングしたベクトルのコピー
			*/
			inline constexpr Vector<ElementType> operator/(const ElementType b) { return Vector<ElementType>(x / b, y / b, z / b); }
			/*
			ベクトルのスケーリング(除算)
			引数: scale 除算する数値(ベクトル)
			戻り値: スケーリングしたベクトルのコピー
		*/
			inline Vector<ElementType> operator/=(const Vector& b) { return *this = *this / b; }
			/*
				ベクトルのスケーリング(除算)
				引数: scale 除算する数値
				戻り値: スケーリングしたベクトルのコピー
			*/
			inline Vector<ElementType> operator/=(const double& b) { return *this = *this / b; }
			/*
				自身と入力ベクトルとの外積
				引数: v 入力ベクトル
				戻り値: 外積
			*/
			inline constexpr Vector<ElementType> operator^(const Vector& b) const { return Vector<ElementType>(static_cast<ElementType>(y * b.z - z * b.y), static_cast<ElementType>(z * b.x - x * b.z), static_cast<ElementType>(x * b.y - y * b.x)); }
			/*
				自身と入力ベクトルとの外積
				引数: v 入力ベクトル
				戻り値: 外積のコピー
			*/
			inline Vector<ElementType> operator^=(const Vector& b) { return *this = *this ^ b; }
			/*
				自身と入力ベクトルとの内積
				引数: v 入力ベクトル
				戻り値: 内積
			*/
			inline constexpr float operator|(const Vector& b) const { return static_cast<float>(x * b.x + y * b.y + z * b.z); }
			/*
				ベクトルの反転
				戻り値:反転したベクトル
			*/
			inline constexpr Vector<ElementType> operator-() const { return Vector<ElementType>(-x, -y, -z); }

			inline Vector<ElementType> operator=(const VECTOR& b) { return this->operator()(static_cast<ElementType>(b.x), static_cast<ElementType>(b.y), static_cast<ElementType>(b.z)); }
			inline Vector<ElementType> operator=(VECTOR&& b) { return this->operator()(static_cast<ElementType>(b.x), static_cast<ElementType>(b.y), static_cast<ElementType>(b.z)); }
			//ベクトルの要素が同一か
			inline constexpr bool operator==(const Vector<ElementType>& b) const { return (x == b.x && y == b.y && z == b.z); }
			inline constexpr bool operator!=(const Vector<ElementType>& b) const { return !(*this == b); }

			//ベクトルの長さを比較
			inline constexpr auto operator<=>(const float b) {
				float a_long = LongSq(), b_long = b * b;
				if (a_long != b_long) { return a_long <=> b_long; }
				return std::partial_ordering::equivalent;
			}

			template<class T>constexpr explicit
				operator Vector<T>() const { return Vector<T>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z)); }

			//DXライブラリベクトル構造体へのキャスト演算子
			inline constexpr operator VECTOR() const { return VECTOR{ static_cast<float>(x), static_cast<float>(y) ,static_cast<float>(z) }; }
			inline constexpr explicit operator VECTOR_D() const { return VECTOR_D{ x, y ,z }; }

			//2Dベクトルへのキャスト演算子
			template<class T>constexpr explicit
				operator Vector2D<T>() const { return Vector2D<T>(static_cast<T>(x), static_cast<T>(y)); }

		public:
			ElementType x, y, z;
		};
		template<typename InElementType>
		template<class T>
		inline constexpr Vector<InElementType> Vector<InElementType>::RotationMove(const Vector2D<T>& rotation, const float& distance) const
		{
			return (*this) * (Matrix::GetTranslateMat(*this) * (Matrix::GetYawMat(static_cast<float>(rotation.y)) * (Matrix::GetPitchMat(static_cast<float>(rotation.x)) *
				(Matrix::GetTranslateMat(Vector(0.f, 0.f, -distance)) * Matrix::GetTranslateMat(-*this)))));
		}

		template<typename InElementType>
		template<class T>
		inline constexpr Vector<InElementType> Vector<InElementType>::RotationMove(const Vector2D<T>& rotation, const Vector& distance) const
		{
			return (*this) * (Matrix::GetTranslateMat(*this) * (Matrix::GetYawMat(static_cast<float>(rotation.y)) * (Matrix::GetPitchMat(static_cast<float>(rotation.x)) *
				(Matrix::GetTranslateMat(distance) * Matrix::GetTranslateMat(-*this)))));
		}
}
	class Matrix
	{
	public:
		union Mat4
		{
			inline float& operator[](_In_range_(0, 3)size_t idx) { return c[idx]; }
			inline const float& operator[](_In_range_(0, 3)size_t idx)const { return c[idx]; }

			inline Mat4 operator=(const __m128& m128) { _mm_store_ps(c, m128); return *this; }

			inline operator __m128& () { return row; }
			inline operator const __m128& () const { return row; }

			alignas(16) float c[4];
			__m128 row;
		};
	public:
		constexpr Matrix() {
			for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					m[y][x] = 0;
		}

	public:	//スタティック関数
		/*
			単位行列を取得
		*/
		constexpr static Matrix GetIdentityMat() {
			Matrix IdentityMat;
			for (int i = 0; i < 4; i++) { IdentityMat[i][i] = 1; }
			return IdentityMat;
		}
		/*
			平行移動行列を取得
			引数: Translate 平行移動するベクトル
			戻り値: 平行移動行列
		*/
		static Matrix GetTranslateMat(const MData::Vector<float>& Translate) {
			Matrix  TranslateMat = GetIdentityMat();
			TranslateMat[0][3] = Translate.x; TranslateMat[1][3] = Translate.y; TranslateMat[2][3] = Translate.z;
			return TranslateMat;
		}
		/*
			拡縮行列を取得
			引数: scale 拡縮値のベクトル
			戻り値: 拡縮行列
		*/
		static Matrix GetScaleMat(const MData::Vector<float>& scale) {
			Matrix ScaleMat = GetIdentityMat();
			ScaleMat[0][0] = scale.x; ScaleMat[1][1] = scale.y; ScaleMat[2][2] = scale.z;
			return ScaleMat;
		}
		/*
			X軸回転行列を取得
			引数: rota 回転値
			戻り値: X軸回転行列
		*/
		static Matrix GetPitchMat(const float& rota) {
			Matrix PitchMat = GetIdentityMat();
			PitchMat[1][1] = cosf(rota); PitchMat[1][2] = -sinf(rota); PitchMat[2][1] = sinf(rota); PitchMat[2][2] = cosf(rota);
			return PitchMat;
		}
		/*
			Y軸回転行列を取得
			引数: rota 回転値
			戻り値: Y軸回転行列
		*/
		static Matrix GetYawMat(const float& rota) {
			Matrix YawMat = GetIdentityMat();
			YawMat[0][0] = cosf(rota); YawMat[0][2] = sinf(rota); YawMat[2][0] = -sinf(rota); YawMat[2][2] = cosf(rota);
			return YawMat;
		}
		/*
			Z軸回転行列を取得
			引数: rota 回転値
			戻り値: Z軸回転行列
		*/
		static Matrix GetRollMat(const float& rota) {
			Matrix RollMat = GetIdentityMat();
			RollMat[0][0] = cosf(rota); RollMat[0][1] = -sinf(rota); RollMat[1][0] = sinf(rota); RollMat[1][1] = cosf(rota);
			return RollMat;
		}
		/*
			回転行列を取得
			引数: rotation 回転ベクトル
			戻り値: 回転行列
		*/
		static Matrix GetRotationMat(const MData::Vector<float>& rotation) {
			return GetRollMat(rotation.z) * GetYawMat(rotation.y) * GetPitchMat(rotation.x) * GetIdentityMat();
		}
	public:
		//転置
		Matrix MatTranspose() const{
			Matrix result;
			for (int i = 0; i < 4; i++)
				result[i] = _mm_set_ps(m[3][i], m[2][i], m[1][i], m[0][i]);
			return result;
		}

	public:	//オペレータのオーバーロード
		Mat4& operator[](_In_range_(0, 3)size_t idx) { return m[idx]; }
		const Mat4& operator[](_In_range_(0, 3)size_t idx) const { return m[idx]; }
		/*
			行列の加算
			引数: B 加算する行列
			戻り値: 加算した行列
		*/
		Matrix operator+(const Matrix& B) const {
			Matrix add;
			/*for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					add[y][x] = m[y][x] + B.m[y][x];*/
			for (int y = 0; y < 4; y++)
				add[y] = _mm_add_ps(m[y], B[y]);
			return add;
		}
		/*
			行列の加算
			引数: B 加算する行列
			戻り値: 加算した行列のコピー
		*/
		Matrix operator+=(const Matrix& B) { return *this = *this + B; }
		/*
			行列の減算
			引数: B 減算する行列
			戻り値: 減算した行列
		*/
		constexpr Matrix operator-(const Matrix& B) const {
			Matrix sub;
			/*for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					sub[y][x] = m[y][x] - B.m[y][x];*/
			for (int y = 0; y < 4; y++)
				sub[y] = _mm_sub_ps(m[y], B[y]);
			return sub;
		}
		/*
			行列の減算
			引数: B 減算する行列
			戻り値: 減算した行列のコピー
		*/
		Matrix operator-=(const Matrix& B) { return *this = *this - B; }
		/*
			行列のスカラー倍
			引数: scale スカラー値
			戻り値: スカラー倍した行列
		*/
		Matrix operator*(float scale) const {
			Matrix ScaleMat;
			__m128 Scale = _mm_set_ps1(scale);
			for (int y = 0; y < 4; y++)
				ScaleMat[y] = _mm_mul_ps(m[y], Scale);
			return ScaleMat;
		}
		/*
			行列のスカラー倍
			引数: scale スカラー値
			戻り値: スカラー倍した行列のコピー
		*/
		Matrix operator*=(const float& scale) { return *this = *this * scale; }
		/*
			行列の内積
			引数: mat かける行列
			戻り値: 内積した行列
		*/
		Matrix operator*(const Matrix& mat) const {
			Matrix Mult;
			__m128 ymm[4];
			for (int i = 0; i < 4; i++) {
				/*m[i][k]をymm[k]の4要素にコピーする*/
				for (int k = 0; k < 4; k++)
					ymm[k] = _mm_set_ps1(m[i][k]);
				/*乗算して加算を繰り返す*/
				ymm[0] = _mm_mul_ps(ymm[0], mat[0]);
				ymm[0] = _mm_fmadd_ps(ymm[1], mat[1], ymm[0]);
				ymm[0] = _mm_fmadd_ps(ymm[2], mat[2], ymm[0]);
				ymm[0] = _mm_fmadd_ps(ymm[3], mat[3], ymm[0]);
				//結果のコピー
				_mm_store_ps(Mult[i].c, ymm[0]);
			}

			return Mult;
		}
		/*
			行列の内積
			引数: mat かける行列
			戻り値: 内積した行列のコピー
		*/
		Matrix operator*=(const Matrix& mat) { return *this = *this * mat; }

		/*
			行列に-をかける
		*/
		Matrix operator-() const {
			Matrix  minus;
			__m128 mult = _mm_set_ps1(-1.f);
			for (int y = 0; y < 4; y++)
				minus[y] = _mm_mul_ps(m[y], mult);
			return minus;
		}

		operator MATRIX() const {
			return *reinterpret_cast<const MATRIX*>(this);
		}

		constexpr operator MData::Vector<float>() const { return MData::Vector<float>(m[3][0], m[3][1], m[3][2]); }
	public:
		alignas(64) Mat4 m[4];
	};

	//要改良
	/*class Rotator
	{
	public:
		class RotatorMember
		{
		public:	friend class RotatorMember;
		public:
			constexpr RotatorMember() :m(0.f) {}
			constexpr RotatorMember(float num) : m(num) {}
		private:
			RotatorMember Norm() {}
		public:
			//加算
			RotatorMember operator+(float f) const { return RotatorMember(m + f).Norm(); }
			RotatorMember operator+(RotatorMember R) const { return RotatorMember(m + R.m).Norm(); }
			RotatorMember operator+=(float f) { return *this = *this + f; }
			RotatorMember operator+=(RotatorMember R) { return *this = *this + R; }

			//減算
			RotatorMember operator-(float f) const { return RotatorMember(m - f).Norm(); }
			RotatorMember operator-(RotatorMember R) const { return RotatorMember(m - R.m).Norm(); }
			RotatorMember operator-=(float f) { return *this = *this - f; }
			RotatorMember operator-=(RotatorMember R) { return *this = *this - R; }

			//乗算
			RotatorMember operator*(float f) const { return RotatorMember(m * f).Norm(); }
			RotatorMember operator*(RotatorMember R) const { return RotatorMember(m * R.m).Norm(); }
			RotatorMember operator*=(float f) { return *this = *this * f; }
			RotatorMember operator*=(RotatorMember R) { return *this = *this * R; }

			//除算
			RotatorMember operator/(float f) const { return RotatorMember(m / f).Norm(); }
			RotatorMember operator/(RotatorMember R) const { return RotatorMember(m / R.m).Norm(); }
			RotatorMember operator/=(float f) { return *this = *this / f; }
			RotatorMember operator/=(RotatorMember R) { return *this = *this / R; }

			RotatorMember operator-() const { return RotatorMember(-m).Norm(); }
			RotatorMember operator=(float f) { m = f; return *this = Norm(); }
			RotatorMember operator=(RotatorMember f) { m = f.m; return *this = Norm(); }

			explicit operator float()const { return m; }
		private:
			float m;
		};
	public:
		constexpr Rotator() :Pitch(0.f), Yaw(0.f), Roll(0.f) {}
		constexpr Rotator(float in_pitch, float in_yaw, float in_roll) : Pitch(in_pitch), Yaw(in_yaw), Roll(in_roll) {}

	public:


	public:
		RotatorMember Pitch, Yaw, Roll;
	};*/

	template<class T>
	MData::Vector<T> operator*(const Matrix& mat, const MData::Vector<T>& v) {
		__m128 work = _mm_set_ps(1.f, v.z, v.y, v.x);
		__m128 result_buf = { 0 };
		auto m = mat.MatTranspose();
		for (int i = 0; i < 4; i++) {
			result_buf = _mm_fmadd_ps(m[i], _mm_set1_ps(work.m128_f32[i]), result_buf);
		}
		return MData::Vector<T>(static_cast<T>(result_buf.m128_f32[0]), static_cast<T>(result_buf.m128_f32[1]), static_cast<T>(result_buf.m128_f32[2]));
	}
	template<class T>
	MData::Vector<T> operator*(const MData::Vector<T>& v, const Matrix& mat) {
		__m128 work = _mm_set_ps(1.f, v.z, v.y, v.x);
		__m128 result_buf = { 0 };
		auto m = mat.MatTranspose();
		for (int i = 0; i < 4; i++) {
			result_buf = _mm_fmadd_ps(m[i], _mm_set1_ps(work.m128_f32[i]), result_buf);
		}
		return MData::Vector<T>(static_cast<T>(result_buf.m128_f32[0]), static_cast<T>(result_buf.m128_f32[1]), static_cast<T>(result_buf.m128_f32[2]));
	}

	typedef MData::Vector<float> Vector;
	typedef MData::Vector<double> dVector;
	typedef MData::Vector2D<float> Vector2D;
	typedef MData::Vector2D<double> dVector2D;
	typedef MData::Vector2D<int> iVector2D;
}

using namespace MyLib;