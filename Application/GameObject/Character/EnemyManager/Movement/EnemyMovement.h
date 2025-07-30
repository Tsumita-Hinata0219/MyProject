#pragma once

#include <memory>
#include <functional>

#include "../../../GameObject.h"
#include "../Property/EnemyProperty.h"


// 前方宣言
class EnemyManager;
class IEnemy;
class Player;


/* エネミーの移動処理 */
class EnemyMovement {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyMovement(EnemyManager* manager, IEnemy* owner, Player* player);

	/// <summary>
	/// デストラク
	/// </summary>
	~EnemyMovement() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(const enemy::MovementFuncData& data);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();


private:

	/// <summary>
	/// 移動関数の設定
	/// </summary>
	void SetMovementFunc(enemy::MovementBehavior movement);

	/// <summary>
	/// 不動
	/// </summary>
	void Movement_Static();

	/// <summary>
	/// 追従
	/// </summary>
	void Movement_Follow();

	/// <summary>
	/// 直線移動
	/// </summary>
	void Movement_Horizontal();

	/// <summary>
	/// 円移動
	/// </summary>
	void Movement_Circular();

	/// <summary>
	/// プレイヤーに体を向ける処理
	/// </summary>
	void BodyTilt();


private:

	// エネミーマネージャー
	EnemyManager* pManager_ = nullptr;

	// エネミー
	IEnemy* pOwner_ = nullptr;

	// プレイヤー
	Player* pPlayer_ = nullptr;

	// 移動関数
	std::function<void()> movementFunc_;

	// 移動データ
	enemy::MovementFuncData data_{};

	// 直線移動の座標を保管しておく
	std::vector<Vector3> movePoints_{};
	int targetIndex_ = 0;

	// 
	float angle_ = 0.0f;

	// playerとの最低距離
	float minToPlayer_ = 4.0f;

	// プレイヤーに体を向けるか
	bool isTilt_ = false;
};