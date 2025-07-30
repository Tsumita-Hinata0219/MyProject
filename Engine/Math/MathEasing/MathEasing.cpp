#include "MathEasing.h"



float Ease::InSine(float num)
{
	return 1.0f - cosf((num * Math::PI) / 2.0f);
}

float Ease::OutSine(float num)
{
	return sinf((num * Math::PI) / 2.0f);
}

float Ease::InOutSine(float num)
{
	return -(cosf(Math::PI * num) - 1.0f) / 2.0f;
}



float Ease::InQuad(float num)
{
	return num * num;
}

float Ease::OutQuad(float num)
{
	return 1.0f - (1.0f - num) * (1.0f - num);
}

float Ease::InOutQuad(float num)
{
	return num < 0.5f ? 2.0f * num * num : 1.0f - pow(-2.0f * num + 2.0f, 2.0f) / 2.0f;
}



float Ease::InCubic(float num)
{
	return num * num * num;
}

float Ease::OutCubic(float num)
{
	return 1.0f - pow(1.0f - num, 3.0f);
}

float Ease::InOutCubic(float num)
{
	return num < 0.5f ? 4.0f * num * num * num : 1.0f - pow(-2.0f * num + 2.0f, 3.0f) / 2.0f;
}



float Ease::InQuart(float num)
{
	return num * num * num * num;
}

float Ease::OutQuart(float num)
{
	return 1.0f - pow(1.0f - num, 4.0f);
}

float Ease::InOutQuart(float num)
{
	return num < 0.5f ? 8.0f * num * num * num * num : 1.0f - pow(-2.0f * num + 20.0f, 4.0f) / 2.0f;
}



float Ease::InQuint(float num)
{
	return num * num * num * num * num;
}

float Ease::OutQuint(float num)
{
	return 1.0f - pow(1.0f - num, 5.0f);
}

float Ease::InOutQuint(float num)
{
	return num < 0.5f ? 16.0f * num * num * num * num * num : .0f - pow(-2.0f * num + 2.0f, 5.0f);
}




float Ease::InExpo(float num)
{
	return num == 0.0f ? 0.0f : pow(2.0f, 10.0f * num - 10.0f);
}

float Ease::OutExpo(float num)
{
	return num == 1.0f ? 1.0f : 1.0f - pow(2.0f, -10.0f * num);
}

float Ease::InOutExpo(float num)
{
	return num == 0.0f
		? 0.0f
		: num == 1.0f
		? 1.0f
		: num < 0.5f ? pow(2.0f, 20.0f * num - 10.0f) / 2.0f
		: (2.0f - pow(2.0f, -20.0f * num + 10.0f)) / 2.0f;
}



float Ease::InCirc(float num)
{
	return 1.0f - sqrt(1.0f - pow(num, 2.0f));
}

float Ease::OutCirc(float num)
{
	return sqrt(1.0f - pow(num - 1.0f, 2.0f));
}

float Ease::InOutCirc(float num)
{
	return num < 0.5f
		? (1.0f - sqrt(1.0f - pow(2.0f * num, 2.0f))) / 2.0f
		: (sqrt(1.0f - pow(-2.0f * num + 2.0f, 2.0f)) + 1.0f) / 2.0f;
}



float Ease::InBack(float num, float intensity)
{
	const float c1 = intensity;
	const float c3 = c1 + 1.0f;

	return c3 * num * num * num - c1 * num * num;
}

float Ease::OutBack(float num)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return 1.0f + c3 * pow(num - 1.0f, 3.0f) + c1 * pow(num - 1.0f, 2.0f);
}

float Ease::InOutBack(float num)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return num < 0.5f
		? (pow(2.0f * num, 2.0f) * ((c2 + 1.0f) * 2.0f * num - c2)) / 2.0f
		: (pow(2.0f * num - 2.0f, 2.0f) * ((c2 + 1.0f) * (num * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
}



float Ease::InElastic(float num)
{
	const float c4 = (2.0f * Math::PI) / 3.0f;

	return num == 0.0f
		? 0.0f
		: num == 1.0f
		? 1.0f
		: -pow(2.0f, 10.0f * num - 10.0f) * sinf((num * 10.0f - 10.75f) * c4);
}

float Ease::OutElastic(float num)
{
	const float c4 = (2.0f * Math::PI) / 3.0f;

	return num == 0.0f
		? 0.0f
		: num == 1.0f
		? 1.0f
		: pow(2.0f, -10.0f * num) * sinf((num * 10.0f - 0.75f) * c4) + 1.0f;
}

float Ease::InOutElastic(float num)
{
	const float c5 = (2.0f * Math::PI) / 4.5f;

	return num == 0.0f
		? 0.0f
		: num == 1.0f
		? 1.0f
		: num < 0.5f
		? -(pow(2.0f, 20.0f * num - 10.f) * sinf((20.0f * num - 11.125f) * c5)) / 2.0f
		: (pow(2.0f, -20.0f * num + 10.0f) * sinf((20.0f * num - 11.125f) * c5)) / 2.0f + 1.0f;
}



float Ease::InBounce(float num)
{
	return 1.0f - Ease::OutBounce(1.0f - num);
}

float Ease::OutBounce(float num)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (num < 1.0f / d1) {
		return n1 * num * num;
	}
	else if (num < 2.0f / d1) {
		return n1 * (num -= 1.5f / d1) * num + 0.75f;
	}
	else if (num < 2.5f / d1) {
		return n1 * (num -= 2.25f / d1) * num + 0.9375f;
	}
	else {
		return n1 * (num -= 2.625f / d1) * num + 0.984375f;
	}
}

float Ease::InOutBounce(float num)
{
	return num < 0.5f
		? (1.0f - Ease::OutBounce(1.0f - 2.0f * num)) / 2.0f
		: (1.0f + Ease::OutBounce(2.0f * num - 1.0f)) / 2.0f;
}

float Ease::WithPeak(float start, float peak, float end, float ratio)
{
	// 正規化された時間を計算
	float midPoint = 0.5f;

	// 前半部分（start -> peak）で補間
	if (ratio < midPoint) {
		return start + (peak - start) * (ratio / midPoint);
	}
	// 後半部分（peak -> end）で補間
	else {
		return peak + (end - peak) * ((ratio - midPoint) / (1.0f - midPoint));
	}
}


