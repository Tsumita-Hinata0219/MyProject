#pragma once

#include "../../../../../../GameObject.h"
#include "../IBasicEnemyState.h"

#include <memory>
#include <utility>

/* IBasicEnemyStateの派生クラス : アイドル処理 */
class BasicEnemyIdleState : public IBasicEnemyState {

public:

	// コンストラクタ
	BasicEnemyIdleState() {};

	// 初期化、更新、終了
	void Enter(BasicEnemy* owner) override;
	void Update() override;
	void Exit() override;

private:

	// ターゲットの座標を更新
	void UpdateTargetPos();


private:

	// SRT
	SRTData srt_{};

	// アイドル状態になった座標
	Vector3 enterIdlePos_{};

	// ターゲット座標
	Vector3 targetPos_{};

	// 移動速度
	float moveSpeed_ = 0.0f;

	// うろつき範囲
	Scope idleRange_ = { -5.0f, 5.0f };

	// 移動先を変える間隔。3秒
	float targetChangeInterval_ = 0.0f;

	// 経過時間
	float timeElapsed_ = 0.0f;
};

