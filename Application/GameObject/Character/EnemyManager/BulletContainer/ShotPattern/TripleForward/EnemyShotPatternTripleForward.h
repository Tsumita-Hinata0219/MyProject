#pragma once

#include "../IEnemyShotPattern.h"

/* 射撃タイプ : 前方3Way */
class EnemyShotPatternTripleForward : public IEnemyShotPattern {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyShotPatternTripleForward() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyShotPatternTripleForward() = default;

	/// <summary>
	/// 射撃処理
	/// </summary>
	void Execute() override;


private:


};
