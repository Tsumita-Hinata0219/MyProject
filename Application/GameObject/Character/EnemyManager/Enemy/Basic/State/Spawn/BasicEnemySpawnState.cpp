#include "BasicEnemySpawnState.h"
#include "../../BasicEnemy.h"



// 初期化処理
void BasicEnemySpawnState::Enter(BasicEnemy* owner)
{
	// Ownerの設定
	owner_ = owner;

	// スケールのイージングにかかる時間の設定。1秒
	scaleEaseTime_.Init(0.0f, 60.0f);
	scaleEaseTime_.Start();
}


// 更新処理
void BasicEnemySpawnState::Update()
{
	// SRTの取得
	srt_ = owner_->GetSRT();

	// スケールをイージングにかける
	if (OutElasticScale()) {

		// trueでチェンジステート
		owner_->ChangeState(BasicEnemyStateType::IDLE);
	}

	// SRTの再設定
	owner_->SetSRT(srt_);
}


// 終了処理
void BasicEnemySpawnState::Exit()
{


}


// スケールをイージングにかける
bool BasicEnemySpawnState::OutElasticScale()
{
	// タイマーの更新
	scaleEaseTime_.Update();

	// 設定するスケール
	float setScale =
		scalePair_.first + (scalePair_.second - scalePair_.first) * Ease::OutElastic(scaleEaseTime_.GetRatio());

	srt_.scale = { setScale, setScale, setScale };

	// イージングが終了したら、クリアしてtrueを返す
	if (scaleEaseTime_.IsFinish()) {

		scaleEaseTime_.Clear();
		return true;
	}

	return false;
}

