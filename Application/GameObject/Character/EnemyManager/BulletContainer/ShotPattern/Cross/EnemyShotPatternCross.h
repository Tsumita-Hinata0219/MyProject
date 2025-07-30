#pragma once

#include "../IEnemyShotPattern.h"

/* 射撃タイプ : クロス */
class EnemyShotPatternCross : public IEnemyShotPattern {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyShotPatternCross() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyShotPatternCross() = default;

	/// <summary>
	/// 射撃処理
	/// </summary>
	void Execute() override;


private:


};
