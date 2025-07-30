#pragma once

#include "../IEnemyShotPattern.h"

/* 射撃タイプ : 全方位4Way */
class EnemyShotPatternOmniFour : public IEnemyShotPattern {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyShotPatternOmniFour() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyShotPatternOmniFour() = default;

	/// <summary>
	/// 射撃処理
	/// </summary>
	void Execute() override;


private:


};
