#pragma once



/// <summary>
/// 3x3行列
/// </summary>
struct Matrix3x3 final {

	// 静的定数メンバ変数
	static const Matrix3x3 zero;
	static const Matrix3x3 one;
	static const Matrix3x3 identity;


	// メンバ変数
	float m[3][3];


	// コンストラクタ
	Matrix3x3(float a11, float a12, float a13,
		float a21, float a22, float a23,
		float a31, float a32, float a33) {
		m[0][0] = a11; m[0][1] = a12; m[0][2] = a13;
		m[1][0] = a21; m[1][1] = a22; m[1][2] = a23;
		m[2][0] = a31; m[2][1] = a32; m[2][2] = a33;
	}
	// デフォルトコンストラクタ
	Matrix3x3() : m{
		{(0.0f, 0.0f, 0.0f)},
		{(0.0f, 0.0f, 0.0f)},
		{(0.0f, 0.0f, 0.0f)},
	} {};

	// == 演算子のオーバーロード　
	inline constexpr bool operator==(const Matrix3x3& rhs) noexcept {
		return
			m[0][0] == rhs.m[0][0] && m[0][1] == rhs.m[0][1] && m[0][2] == rhs.m[0][2] &&
			m[1][0] == rhs.m[1][0] && m[1][1] == rhs.m[1][1] && m[1][2] == rhs.m[1][2] &&
			m[2][0] == rhs.m[2][0] && m[2][1] == rhs.m[2][1] && m[2][2] == rhs.m[2][2];
	}

	// != 演算子のオーバーロード 
	inline constexpr bool operator!=(const Matrix3x3& rhs) noexcept {
		return !(*this == rhs);
	}
};



/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {

	// 静的定数メンバ変数
	static const Matrix4x4 zero;
	static const Matrix4x4 one;
	static const Matrix4x4 identity;


	// メンバ変数
	float m[4][4];


	// コンストラクタ
	Matrix4x4(
		float a11, float a12, float a13, float a14,
		float a21, float a22, float a23, float a24,
		float a31, float a32, float a33, float a34,
		float a41, float a42, float a43, float a44) {
		m[0][0] = a11; m[0][1] = a12; m[0][2] = a13; m[0][3] = a14;
		m[1][0] = a21; m[1][1] = a22; m[1][2] = a23; m[1][3] = a24;
		m[2][0] = a31; m[2][1] = a32; m[2][2] = a33; m[2][3] = a34;
		m[3][0] = a41; m[3][1] = a42; m[3][2] = a43; m[3][3] = a44;
	}
	// デフォルトコンストラクタ
	Matrix4x4() : m{
		{(0.0f, 0.0f, 0.0f, 0.0f)},
		{(0.0f, 0.0f, 0.0f, 0.0f)},
		{(0.0f, 0.0f, 0.0f, 0.0f)},
		{(0.0f, 0.0f, 0.0f, 0.0f)},
	} {};

	// == 演算子のオーバーロード　
	inline constexpr bool operator==(const Matrix4x4& rhs) noexcept {
		return
			m[0][0] == rhs.m[0][0] && m[0][1] == rhs.m[0][1] && m[0][2] == rhs.m[0][2] && m[0][3] == rhs.m[0][3] &&
			m[1][0] == rhs.m[1][0] && m[1][1] == rhs.m[1][1] && m[1][2] == rhs.m[1][2] && m[1][3] == rhs.m[1][3] &&
			m[2][0] == rhs.m[2][0] && m[2][1] == rhs.m[2][1] && m[2][2] == rhs.m[2][2] && m[2][3] == rhs.m[2][3] &&
			m[3][0] == rhs.m[3][0] && m[3][1] == rhs.m[3][1] && m[3][2] == rhs.m[2][2] && m[3][3] == rhs.m[3][3];
	}

	// != 演算子のオーバーロード 
	inline constexpr bool operator!=(const Matrix4x4& rhs) noexcept {
		return !(*this == rhs);
	}

};