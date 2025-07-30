#pragma once

#include "../IEnemyShotPattern.h"

/* 射撃タイプ : 前方 */
class EnemyShotPatternForward : public IEnemyShotPattern {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyShotPatternForward() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyShotPatternForward() = default;

	/// <summary>
	/// 射撃処理
	/// </summary>
	void Execute() override;


private:


};

