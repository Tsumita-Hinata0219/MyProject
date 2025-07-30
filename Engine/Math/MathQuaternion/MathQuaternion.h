#pragma once


/// <summary>
/// クォータニオン
/// </summary>
struct Quaternion final {

	// 静的定数メンバ変数
	static const Quaternion zero;
	static const Quaternion identity;

	// メンバ変数
	float w, x, y, z;

	// デフォルトコンストラクタ
	Quaternion() : w(0.0f), x(0.0f), y(0.0f), z(0.0f) {};
	// パラメータ付きコンストラクタ
	Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {};

	// == 演算子のオーバーロード
	inline constexpr bool operator==(const Quaternion& rhs) noexcept {
		return w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z;
	}

	// != 演算子のオーバーロード
	inline constexpr bool operator!=(const Quaternion& rhs) noexcept {
		return !(*this == rhs);
	}
};

