#include "EnemyMovement.h"
#include "../EnemyManager.h"
#include "../Enemy/IEnemy.h"
#include "../../Player/Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
EnemyMovement::EnemyMovement(EnemyManager* manager, IEnemy* owner, Player* player)
{
	this->pManager_ = manager;
	this->pOwner_ = owner;
	this->pPlayer_ = player;
}


/// <summary>
/// 初期化処理
/// </summary>
void EnemyMovement::Init(const enemy::MovementFuncData& data)
{
	data_ = data;

	data_.velocity /= 60.0f;

	// 関数設定
	SetMovementFunc(data_.behavior);

	// プレイヤーに体を向けるかのフラグ
	isTilt_ = data_.isTilt;

	// behaviorがHorizontalなら中間点をOwnerの初期座標で初期化
	if (data_.behavior == enemy::MovementBehavior::Horizontal) {
		data_.horizontal_middle = pOwner_->GetWorldPos();
		movePoints_.resize(4);
		movePoints_ = {
			data_.horizontal_middle,
			data_.horizontal_start,
			data_.horizontal_middle,
			data_.horizontal_end,
		};
	}

	// behaviorがCircularなら、3つの座標をvector配列に入れる
	if (data_.behavior == enemy::MovementBehavior::Circular) {
		// 初期位置と中心点から角度を計算
		Vector3 position = pOwner_->GetWorldPos();
		angle_ = atan2(position.z - data_.circular_center.z, position.x - data_.circular_center.x);  // 角度を計算
	}
}


/// <summary>
/// 更新処理
/// </summary>
void EnemyMovement::Update()
{
	// 移動処理
	movementFunc_();

	// フラグが立っていたら
	if (isTilt_) {
		BodyTilt();
	}
}


/// <summary>
/// 移動関数の設定
/// </summary>
void EnemyMovement::SetMovementFunc(enemy::MovementBehavior movement)
{
	switch (movement)
	{
	case enemy::MovementBehavior::Static:
		movementFunc_ = std::bind(&EnemyMovement::Movement_Static, this);
		break;
	case enemy::MovementBehavior::Follow:
		movementFunc_ = std::bind(&EnemyMovement::Movement_Follow, this);
		break;
	case enemy::MovementBehavior::Horizontal:
		movementFunc_ = std::bind(&EnemyMovement::Movement_Horizontal, this);
		break;
	case enemy::MovementBehavior::Circular:
		movementFunc_ = std::bind(&EnemyMovement::Movement_Circular, this);
		break;
	default:
		break;
	}
}


/// <summary>
/// 不動
/// </summary>
void EnemyMovement::Movement_Static() {}


/// <summary>
/// 追従
/// </summary>
void EnemyMovement::Movement_Follow()
{
	// ある程度近ければ早期return
	if (std::abs(Length(pPlayer_->GetWorldPos() - pOwner_->GetWorldPos())) <= minToPlayer_) {
		return;
	}

	// velocityを計算
	// 差分をNormalize
	Vector3 player2Enemy =
		Normalize(pPlayer_->GetWorldPos() - pOwner_->GetWorldPos());

	// 差分Normalizeに速度をかけてvelocityに設定
	Vector3 moveVelocity = {
		player2Enemy.x * data_.velocity,
		player2Enemy.y, // y軸は移動してほしくないのでそのまま
		player2Enemy.z * data_.velocity,
	};

	// 座標にvelocityを加算
	Vector3 setTranslate = pOwner_->GetWorldPos() + moveVelocity;
	pOwner_->SetTranslate(setTranslate);
}


/// <summary>
/// 直線移動
/// </summary>
void EnemyMovement::Movement_Horizontal()
{
	// 現在のターゲット地点と移動先を取得
	Vector3 target = movePoints_[targetIndex_];

	// 目標地点までのベクトル（移動方向）
	Vector3 direction = target - pOwner_->GetWorldPos();

	// 移動速度を設定（方向ベクトルの単位ベクトルを求めて、velocityでスケーリング）
	if (Length(direction) > data_.velocity) {
		direction = Normalize(direction); // 単位ベクトルにする
		Vector3 setTranslate = pOwner_->GetWorldPos() + direction * data_.velocity; // 移動
		pOwner_->SetTranslate(setTranslate);
	}
	else {
		// 目標地点に到達した場合、次のターゲットに切り替え
		pOwner_->SetTranslate(target);  // 到達した位置を設定
		targetIndex_ = (targetIndex_ + 1) % movePoints_.size(); // 次のポイントへ
	}
}


/// <summary>
/// 円移動
/// </summary> 
void EnemyMovement::Movement_Circular()
{
	// 円移動用にvelocityを調整（角速度に変換）
	float angularVelocity = data_.velocity / data_.circular_radius; // 半径で割ることで適切な角速度を算出

	// 角速度を直接使用
	angle_ += angularVelocity;

	// 角度が2πを超えたらリセット
	if (angle_ > 2.0f * 3.14159265359f) {
		angle_ -= 2.0f * 3.14159265359f; // 角度を0から2πに保つ
	}

	// 新しい位置を計算
	Vector3 setTranslate = {
		data_.circular_center.x + data_.circular_radius * cos(angle_),
		pOwner_->GetWorldPos().y, // y軸はそのまま
		data_.circular_center.z + data_.circular_radius * sin(angle_),
	};

	// 新しい位置を設定
	pOwner_->SetTranslate(setTranslate);
}


/// <summary>
/// プレイヤーに体を向ける処理
/// </summary>
void EnemyMovement::BodyTilt()
{
	// 差分をNormalize
	Vector3 player2Enemy =
		Normalize(pPlayer_->GetWorldPos() - pOwner_->GetWorldPos());

	// 差分Normalizeに速度をかけてvelocityに設定
	Vector2 direction = {
		player2Enemy.x,
		player2Enemy.z,
	};

	// Y軸周り角度(θy)
	pOwner_->SetRotate_Y(std::atan2(direction.x, direction.y));
}

