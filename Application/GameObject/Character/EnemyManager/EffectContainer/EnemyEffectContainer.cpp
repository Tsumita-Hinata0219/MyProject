#include "EnemyEffectContainer.h"



/// <summary>
/// 初期化処理
/// </summary>
void EnemyEffectContainer::Init()
{
	// EffectのPoolのインスタンスを先に作っておく
	circleEffectPool_.Create(20);

}


/// <summary>
/// 更新処理
/// </summary>
void EnemyEffectContainer::Update()
{
	// Effectの更新処理
	for (EnemyHitEffectCircle* effect : circleEffectList_) {
		effect->Update();
	}
	// 死亡フラグが立っていたら削除
	circleEffectList_.remove_if([this](EnemyHitEffectCircle* effect) {
		if (effect->IsDead()) {
			// 死亡したエフェクトはプールに返却
			circleEffectPool_.Return(effect);
			return true;
		}
		return false;
		}
	);

}


/// <summary>
/// 描画処理
/// </summary>
void EnemyEffectContainer::Draw()
{
	// Effectの描画
	for (EnemyHitEffectCircle* effect : circleEffectList_) {
		effect->Draw3D();
	}
}


/// <summary>
/// バレット追加
/// </summary>
void EnemyEffectContainer::AddEffectInstance()
{
	// プールから新しいエフェクトを取得
	EnemyHitEffectCircle* newEffect = circleEffectPool_.Get();

	// newEffectの初期化
	newEffect->SetEnemyPtr(owner_);
	newEffect->Init();

	// リストに追加
	circleEffectList_.push_back(newEffect);
}
