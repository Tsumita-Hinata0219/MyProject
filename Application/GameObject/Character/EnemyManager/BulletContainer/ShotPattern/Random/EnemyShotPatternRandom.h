#pragma once

#include "../IEnemyShotPattern.h"

/* 射撃タイプ : ランダム */
class EnemyShotPatternRandom : public IEnemyShotPattern {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyShotPatternRandom();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyShotPatternRandom() = default;

	/// <summary>
	/// 射撃処理
	/// </summary>
	void Execute() override;


private:

	std::mt19937 randEngine_; // ランダムエンジン
	std::uniform_int_distribution<> behabirDist_; // 0 or 1 の分布
	std::uniform_real_distribution<float> directionDist_; // -1 ~ +1 の分布

};
