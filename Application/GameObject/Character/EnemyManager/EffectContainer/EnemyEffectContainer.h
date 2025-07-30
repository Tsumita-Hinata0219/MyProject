#pragma once

#include "GameObject/GameObject.h"
#include "../Property/EnemyProperty.h"
#include "../Bullet/EnemyBullet.h"

#include "../Effect/Interface/IEnemyEffect.h"
#include "../Effect/HitEffect/Circle/EnemyHitEffectCircle.h"


// 前方宣言
class IEnemy;


/* Enemyそれぞれが持つ、Effectlletの管理クラス */
class EnemyEffectContainer {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyEffectContainer() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyEffectContainer() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// バレット追加
	/// </summary>
	void AddEffectInstance();


#pragma region Accessor アクセッサ

	// 親エネミー
	void SetOwner(IEnemy* pEnemy) { this->owner_ = pEnemy; }

#pragma endregion


private:

	// 親エネミー
	IEnemy* owner_ = nullptr;

	// EffectのPool
	ObjectPool<EnemyHitEffectCircle> circleEffectPool_;
	// EffectのList配列
	std::list<EnemyHitEffectCircle*> circleEffectList_;

};