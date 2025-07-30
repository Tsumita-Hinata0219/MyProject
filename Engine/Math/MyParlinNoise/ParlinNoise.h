#pragma once

#include "Math/MyMath.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <cmath>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <array>
#include <numbers>
#include <vector>

class ParlinNoise {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParlinNoise();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParlinNoise() {};

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static ParlinNoise* GetInstance();

	/// <summary>
	/// 自作のパーリンノイズ
	/// </summary>
	static float Noise(const Vector3& val);

private:

	/// <summary>
	/// フェード
	/// </summary>
	static float Fade(const float& t);

	/// <summary>
	/// 補間
	/// </summary>
	static float pLerp(const float& start, const float& end, const float& t);

	/// <summary>
	/// 勾配
	/// </summary>
	static float grad(const int& hash, const Vector3& val);


private:

	std::vector<int> gradientTable_ = std::vector<int>(512);

};

