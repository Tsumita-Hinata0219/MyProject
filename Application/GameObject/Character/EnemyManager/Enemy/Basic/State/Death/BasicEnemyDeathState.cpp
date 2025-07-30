#include "BasicEnemyDeathState.h"
#include "../../BasicEnemy.h"



// 初期化処理
void BasicEnemyDeathState::Enter(BasicEnemy* owner)
{
	// Ownerの設定
	owner_ = owner;

	// イージングのタイマー時間設定。1.5秒
	posEaseTime_.Init(0.0f, 90.0f);
	posEaseTime_.Start();

	// イージングの開始座標と終了座標
	posPair_ = {
		owner_->GetWorldPos().y,
		owner_->GetWorldPos().y - 20,
	};
}


// 更新処理
void BasicEnemyDeathState::Update()
{
	// SRTの取得
	srt_ = owner_->GetSRT();

	// スケールをイージングにかける
	if (InOutBackPos()) {

		// trueでデスフラグを立てる
		owner_->SetDeadFlag(true);
	}

	// SRTの再設定
	owner_->SetSRT(srt_);
}


// 終了処理
void BasicEnemyDeathState::Exit()
{


}


// ポジションにイージングにかける
bool BasicEnemyDeathState::InOutBackPos()
{
	// タイマーの更新
	posEaseTime_.Update();

	srt_.translate.y =
		posPair_.first + (posPair_.second - posPair_.first) * Ease::InOutBack(posEaseTime_.GetRatio());


	// イージングが終了したら、クリアしてtrueを返す
	if (posEaseTime_.IsFinish()) {

		posEaseTime_.Clear();
		return true;
	}

	return false;
}
