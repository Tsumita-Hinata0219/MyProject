#pragma once

#include "../IEnemyShotPattern.h"

/* 射撃タイプ : 全方位8Way */
class EnemyShotPatternOmniEight : public IEnemyShotPattern {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyShotPatternOmniEight() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyShotPatternOmniEight() = default;

	/// <summary>
	/// 射撃処理
	/// </summary>
	void Execute() override;


private:


};
