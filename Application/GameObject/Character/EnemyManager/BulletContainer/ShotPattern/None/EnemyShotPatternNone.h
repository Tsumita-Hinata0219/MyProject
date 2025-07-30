#pragma once

#include "../IEnemyShotPattern.h"

/* 射撃タイプ : なし */
class EnemyShotPatternNone : public IEnemyShotPattern {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyShotPatternNone() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyShotPatternNone() = default;

	/// <summary>
	/// 射撃処理
	/// </summary>
	void Execute() override;


private:


};

