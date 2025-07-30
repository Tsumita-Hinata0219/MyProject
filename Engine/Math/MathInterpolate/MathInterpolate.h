#pragma once

#include "../MathStructure/Vector/Vector.h"
#include "../MathEasing/MathEasing.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <array>
#include <numbers>
#include <span>
#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <map>

/// <summary>
/// 汎用的な補間処理を行う関数。
/// イージング関数を指定して、開始値から終了値へのスムーズな補間を実現します。
/// </summary>
/// <typeparam name="T">補間する値の型（例: float, Vector3 など）</typeparam>
/// <param name="startValue">開始値</param>
/// <param name="endValue">終了値</param>
/// <param name="ratio">進行度（0.0f～1.0fの範囲）</param>
/// <param name="easingFunc">イージング関数（例: Ease::OutExpo）</param>
/// <returns>補間された値</returns>
template <typename T, typename Func>
T Interpolate(const T& startValue, const T& endValue, float ratio, Func easingFunc) {
    float easedRatio = easingFunc(ratio); // イージング関数で進行度を補正
    return startValue + (endValue - startValue) * easedRatio;
}

/// <summary>
/// 汎用的な補間処理を行う関数。
/// イージング関数を指定して、開始値から終了値へのスムーズな補間を実現します。
/// </summary>
/// <typeparam name="T">補間する値の型（例: float, Vector3 など）</typeparam>
/// <param name="start">開始</param>
/// <param name="peak">ピーク値</param>
/// <param name="end">開始&終了値</param>
/// <param name="ratio">進行度（0.0f～1.0fの範囲）</param>
/// <returns>補間された値</returns>
template <typename T, typename Func>
T InterpolateWithPeak(const T& start, const T& peak, const T& end, float ratio, Func easingFunc) {
    T result = easingFunc(start, peak, end, ratio);
    return result;
}