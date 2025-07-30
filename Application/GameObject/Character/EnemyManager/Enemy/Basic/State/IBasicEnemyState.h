#pragma once

// ステートの種類
enum class BasicEnemyStateType {
	SPAWN,
	IDLE,
	APPROACH,
	DEATH,
	Count,
};

// BasicEnemyの前方宣言
class BasicEnemy;

/* IBasicEnemyStateの基底クラス */
class IBasicEnemyState {

public:

	// 仮想デストラクタ
	virtual ~IBasicEnemyState() = default;

	// 初期化、更新、終了
	virtual void Enter(BasicEnemy* owner) = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

protected:

	BasicEnemy* owner_ = nullptr;

};