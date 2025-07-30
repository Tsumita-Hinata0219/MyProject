#include "IEnemy.h"
#include "../../EnemyManager/EnemyManager.h"


/// <summary>
/// ヒットリアクション
/// </summary>
void IEnemy::HitReaction()
{
	// フラグが折れていたら早期return
	if (!isHitReactioning_) { return; }

	// タイマー更新
	hitReactionTimer_.Update();
	// タイマー終了
	if (hitReactionTimer_.IsFinish()) {
		// リセット
		hitReactionTimer_.Reset();
		// リアクション中のフラグを折る
		isHitReactioning_ = false;
	}

	// スケール
	HitReaction_Scale();
	// カラー
	HitReaction_Color();
}


/// <summary>
/// スケールのリアクション
/// </summary>
void IEnemy::HitReaction_Scale()
{
	transform_.srt.scale.x =
		InterpolateWithPeak(hitReactionScale_.x, hitReactionScale_.y, hitReactionScale_.z, hitReactionTimer_.GetRatio(), Ease::WithPeak);
	transform_.srt.scale.y =
		InterpolateWithPeak(hitReactionScale_.x, hitReactionScale_.y, hitReactionScale_.z, hitReactionTimer_.GetRatio(), Ease::WithPeak);
	transform_.srt.scale.z =
		InterpolateWithPeak(hitReactionScale_.x, hitReactionScale_.y, hitReactionScale_.z, hitReactionTimer_.GetRatio(), Ease::WithPeak);
}


/// <summary>
/// 色のアクション
/// </summary>
void IEnemy::HitReaction_Color()
{
	colorAdd_.intensity =
		Interpolate(hitReactionColor_.first, hitReactionColor_.second, hitReactionTimer_.GetRatio(), Ease::OutQuint);
}
