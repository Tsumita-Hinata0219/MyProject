#include "BasicEnemyIdleState.h"
#include "../../BasicEnemy.h"



// 初期化処理
void BasicEnemyIdleState::Enter(BasicEnemy* owner)
{
	// Ownerの設定
	owner_ = owner;

	// アイドル状態になった座標の設定
	enterIdlePos_ = owner_->GetWorldPos();

	// 初期ターゲットは現在位置
	targetPos_ = enterIdlePos_;

	// 経過時間は0でスタート
	timeElapsed_ = 0.0f;

	// インターバル。3秒
	targetChangeInterval_ = 4.0f * 60.0f;

	// 移動速度
	moveSpeed_ = 0.04f;
}


// 更新処理
void BasicEnemyIdleState::Update()
{
	// SRTの取得
	srt_ = owner_->GetSRT();

	// 時間を更新
	timeElapsed_++;

	// 一定時間で、ターゲット更新
	if (timeElapsed_ >= targetChangeInterval_) {

		UpdateTargetPos();
		timeElapsed_ = 0.0f; // 時間をリセット
	}

	// 現在位置からターゲット位置へのベクトルを計算
	Vector3 direction = targetPos_ - srt_.translate;

	// ターゲットに向かって移動
	if (Length(direction) > 0.1f) { // 近づいたら止まる

		direction = Normalize(direction);
		srt_.translate += direction * moveSpeed_;
	}

	// SRTの再設定
	owner_->SetSRT(srt_);
}


// 終了処理
void BasicEnemyIdleState::Exit()
{
}


// ターゲットの座標を更新
void BasicEnemyIdleState::UpdateTargetPos()
{
	targetPos_ = {
		enterIdlePos_.x + RandomGenerator::getRandom(idleRange_),
		enterIdlePos_.y,
		enterIdlePos_.z + RandomGenerator::getRandom(idleRange_),
	};
}
