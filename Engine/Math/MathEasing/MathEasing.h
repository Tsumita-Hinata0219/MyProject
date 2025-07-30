#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <cmath>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <array>
#include <numbers>

#include "../MathGeometry/MathGeometry.h"


/* イージング関数 */
struct Ease {

	static float InSine(float num);
	static float OutSine(float num);
	static float InOutSine(float num);

	static float InQuad(float num);
	static float OutQuad(float num);
	static float InOutQuad(float num);

	static float InCubic(float num);
	static float OutCubic(float num);
	static float InOutCubic(float num);

	static float InQuart(float num);
	static float OutQuart(float num);
	static float InOutQuart(float num);

	static float InQuint(float num);
	static float OutQuint(float num);
	static float InOutQuint(float num);

	static float InExpo(float num);
	static float OutExpo(float num);
	static float InOutExpo(float num);

	static float InCirc(float num);
	static float OutCirc(float num);
	static float InOutCirc(float num);

	static float InBack(float num, float intensity = 1.70158f);
	static float OutBack(float num);
	static float InOutBack(float num);

	static float InElastic(float num);
	static float OutElastic(float num);
	static float InOutElastic(float num);

	static float InBounce(float num);
	static float OutBounce(float num);
	static float InOutBounce(float num);

	static float WithPeak(float start, float peak, float end, float ratio);
};

