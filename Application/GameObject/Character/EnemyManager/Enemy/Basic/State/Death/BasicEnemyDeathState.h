#pragma once

#include "../../../../../../GameObject.h"
#include "../IBasicEnemyState.h"

#include <memory>
#include <utility>

/* IBasicEnemyStateの派生クラス : 死亡処理 */
class BasicEnemyDeathState : public IBasicEnemyState {

public:

	// コンストラクタ
	BasicEnemyDeathState() {};

	// 初期化、更新、終了
	void Enter(BasicEnemy* owner) override;
	void Update() override;
	void Exit() override;


private:

	// ポジションにイージングにかける
	bool InOutBackPos();


private:

	// SRT
	SRTData srt_{};

	// 開始値と終了値
	std::pair<float, float> posPair_;

	// スケールのイージングにかかるタイマー
	Timer posEaseTime_{};

};