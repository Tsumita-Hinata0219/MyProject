#pragma once

#include "../../../../../../GameObject.h"
#include "../IBasicEnemyState.h"

#include <memory>
#include <utility>

/* IBasicEnemyStateの派生クラス : 接近処理 */
class BasicEnemyApproachState : public IBasicEnemyState {

public:

	// コンストラクタ
	BasicEnemyApproachState() {};

	// 初期化、更新、終了
	void Enter(BasicEnemy* owner) override;
	void Update() override;
	void Exit() override;


private:

};

