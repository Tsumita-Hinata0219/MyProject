#pragma once

#include "../IEnemyShotPattern.h"

/* 射撃タイプ : 全方位5Way */
class EnemyShotPatternOmniFive : public IEnemyShotPattern {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyShotPatternOmniFive() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyShotPatternOmniFive() = default;

	/// <summary>
	/// 射撃処理
	/// </summary>
	void Execute() override;


private:


};
