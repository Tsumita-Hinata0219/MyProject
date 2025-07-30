#pragma once

#include "../../../../../../GameObject.h"
#include "../IBasicEnemyState.h"

#include <memory>
#include <utility>

/* IBasicEnemyStateの派生クラス : スポーン処理 */
class BasicEnemySpawnState : public IBasicEnemyState {

public:

	// コンストラクタ
	BasicEnemySpawnState() {};

	// 初期化、更新、終了
	void Enter(BasicEnemy* owner) override;
	void Update() override;
	void Exit() override;


private:

	// スケールをイージングにかける
	bool OutElasticScale();


private:

	// SRT
	SRTData srt_{};

	// スケールの開始値と終了値。(0.0f, 1.0f)
	std::pair<float, float> scalePair_ = { 0.0f, 1.1f };

	// スケールのイージングにかかるタイマー
	Timer scaleEaseTime_{};

};

