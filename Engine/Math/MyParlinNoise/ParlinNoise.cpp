#include "ParlinNoise.h"



// コンストラクタ
ParlinNoise::ParlinNoise() 
{
	for (int i = 0; i < 256; ++i) {
		gradientTable_[i] = i;
	}

	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(gradientTable_.begin(), gradientTable_.begin() + 256, generator);

	gradientTable_.insert(gradientTable_.end(), gradientTable_.begin(), gradientTable_.begin() + 256);
}


// インスタンスの取得
ParlinNoise* ParlinNoise::GetInstance()
{
    static ParlinNoise instance;
    return &instance;
}


// 自作パーリンノイズ
float ParlinNoise::Noise(const Vector3& val) 
{
    Vector3 valI = {
        float((int)floorf(val.x) & 255),
        float((int)floorf(val.y) & 255),
        float((int)floorf(val.z) & 255),
    };

    Vector3 valN = {
        val.x - floorf(val.x),
        val.y - floorf(val.y),
        val.z - floorf(val.z),
    };

    float u = Fade(valN.x);
    float v = Fade(valN.x);
    float w = Fade(valN.x);


    int p000 = ParlinNoise::GetInstance()->gradientTable_[int(valI.x)] + int(valI.y);
    int p001 = ParlinNoise::GetInstance()->gradientTable_[p000] + int(valI.z);
    int p010 = ParlinNoise::GetInstance()->gradientTable_[static_cast<std::vector<int, std::allocator<int>>::size_type>(p000) + 1] + int(valI.z);
    int p011 = ParlinNoise::GetInstance()->gradientTable_[static_cast<std::vector<int, std::allocator<int>>::size_type>(int(valI.x)) + 1] + int(valI.y);
    int p100 = ParlinNoise::GetInstance()->gradientTable_[p011] + int(valI.z);
    int p101 = ParlinNoise::GetInstance()->gradientTable_[static_cast<std::vector<int, std::allocator<int>>::size_type>(p011) + 1] + int(valI.z);


    float g000 = grad(ParlinNoise::GetInstance()->gradientTable_[p001], valN);
    float g001 = grad(ParlinNoise::GetInstance()->gradientTable_[p100], {valN.x - 1.0f, valN.y, valN.z});
    float g010 = grad(ParlinNoise::GetInstance()->gradientTable_[p010], { valN.x, valN.y - 1.0f, valN.z });
    float g011 = grad(ParlinNoise::GetInstance()->gradientTable_[p101], { valN.x - 1.0f, valN.y - 1.0f, valN.z });
    float g100 = grad(ParlinNoise::GetInstance()->gradientTable_[static_cast<std::vector<int, std::allocator<int>>::size_type>(p001) + 1], { valN.x, valN.y, valN.z - 1.0f });
    float g101 = grad(ParlinNoise::GetInstance()->gradientTable_[static_cast<std::vector<int, std::allocator<int>>::size_type>(p100) + 1], { valN.x - 1.0f, valN.y, valN.z - 1.0f });
    float g110 = grad(ParlinNoise::GetInstance()->gradientTable_[static_cast<std::vector<int, std::allocator<int>>::size_type>(p010) + 1], { valN.x, valN.y - 1.0f, valN.z - 1.0f });
    float g111 = grad(ParlinNoise::GetInstance()->gradientTable_[static_cast<std::vector<int, std::allocator<int>>::size_type>(p101) + 1], valN - 1.0f);


    float n000 = lerp(g000, g001, u);
    float n001 = lerp(g010, g011, u);
    float n010 = lerp(g100, g101, u);
    float n011 = lerp(g110, g111, u);
    float n100 = lerp(n000, n001, v);
    float n101 = lerp(n010, n011, v);

    float result = lerp(n100, n101, w);

    return (result + 1.0f) / 4.0f;
}


// フェード
float ParlinNoise::Fade(const float& t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}


// 補間
float ParlinNoise::pLerp(const float& start, const float& end, const float& t)
{
	return start + t * (end - start);
}


// 勾配
float ParlinNoise::grad(const int& hash, const Vector3& val)
{
	int h = hash & 15;
	float u = h < 8 ? val.x : val.y;
	float v = h < 4 ? val.y : h == 12 || h == 14 ? val.x : val.z;
	return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

