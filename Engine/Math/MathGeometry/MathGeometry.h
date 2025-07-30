#pragma once

#include "../MyMath.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <cmath>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <array>
#include <numbers>


namespace Math {
	constexpr float PI = 3.1415926535897932384626433832795028f;
	constexpr float Double_PI = PI * 2;
	constexpr float Half_PI = PI * 0.5f;
}

// 前後左右の構造体 V:Vertical H;Horizontal
struct VHInfo {

	float up, down, left, right;

	VHInfo() : up(0.0f), down(0.0f), left(0.0f), right(0.0f) {};
	VHInfo(float u, float d, float l, float r) : up(u), down(d), left(l), right(r) {};
};

// 左上 左下 右上 右下 の四頂点の構造体
struct QuadVertex {

	float LeftUp, LeftDown, RightUp, RightDown;

	QuadVertex() : LeftUp(0.0f), LeftDown(0.0f), RightUp(0.0f), RightDown(0.0f) {};
	QuadVertex(float lu, float ld, float ru, float rd) : LeftUp(lu), LeftDown(ld), RightUp(ru), RightDown(rd) {};
};
// 左上 左下 右上 右下 の四頂点の構造体
struct QuadVertex2 {

	Vector2 LeftUp, LeftDown, RightUp, RightDown;

	QuadVertex2() : LeftUp(0.0f, 0.0f), LeftDown(0.0f, 0.0f), RightUp(0.0f, 0.0f), RightDown(0.0f, 0.0f) {};
	QuadVertex2(Vector2 lu, Vector2 ld, Vector2 ru, Vector2 rd) : LeftUp(lu), LeftDown(ld), RightUp(ru), RightDown(rd) {};
};