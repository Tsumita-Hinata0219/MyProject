#pragma once



/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {

	// 静的定数メンバ変数
	static const Vector2 zero;
	static const Vector2 one;
	static const Vector2 oneX;
	static const Vector2 oneY;

	// メンバ変数
	float x, y;

	// デフォルトコンストラクタ
	Vector2() : x(0.0f), y(0.0f) {};
	// パラメータ付きコンストラクタ
	Vector2(float x, float y) : x(x), y(y) {};

	// == 演算子のオーバーロード　
	inline constexpr bool operator==(const Vector2& rhs) noexcept {
		return x == rhs.x && y == rhs.y;
	}

	// != 演算子のオーバーロード 
	inline constexpr bool operator!=(const Vector2& rhs) noexcept { 
		return !(*this == rhs); 
	}

	// スカラー代入
	Vector2& operator=(float f) {
		x = f;
		y = f;
		return *this;
	}

};



/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {

	// 静的定数メンバ変数
	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 oneX;
	static const Vector3 oneY;
	static const Vector3 oneZ;

	// メンバ変数
	float x, y, z;

	// デフォルトコンストラクタ
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {};
	// パラメータ付きコンストラクタ
	Vector3(float x, float y, float z) : x(x), y(y), z(z){};

	// == 演算子のオーバーロード
	inline constexpr bool operator==(const Vector3& rhs) noexcept {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	// != 演算子のオーバーロード 
	inline constexpr bool operator!=(const Vector3& rhs) noexcept {
		return !(*this == rhs);
	}

	// スカラー代入
	Vector3& operator=(float f) {
		x = f;
		y = f;
		z = f;
		return *this;
	}
};



/// <summary>
/// 4次元ベクトル
/// </summary>
struct Vector4 final {

	// 静的定数メンバ変数
	static const Vector4 zero;
	static const Vector4 one;
	static const Vector4 oneX;
	static const Vector4 oneY;
	static const Vector4 oneZ;
	static const Vector4 oneW;

	// メンバ変数
	float x, y, z, w;

	// デフォルトコンストラクタ
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {};
	// パラメータ付きコンストラクタ
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

	// == 演算子のオーバーロード　
	inline constexpr bool operator==(const Vector4& rhs) noexcept {
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}

	// != 演算子のオーバーロード 
	inline constexpr bool operator!=(const Vector4& rhs) noexcept {
		return !(*this == rhs);
	}

	// スカラー代入
	Vector4& operator=(float f) {
		x = f;
		y = f;
		z = f;
		w = f;
		return *this;
	}
};
