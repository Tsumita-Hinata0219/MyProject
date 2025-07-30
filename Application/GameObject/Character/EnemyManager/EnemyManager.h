#pragma once

#include <list>
#include <memory>

#include "../../GameObject.h"

#include "Property/EnemyProperty.h"
#include "Enemy/IEnemy.h"
#include "Enemy/Basic/BasicEnemy.h"
#include "Enemy/Static/StaticEnemy.h"
#include "Enemy/Boss/BossEnemy.h"

#include "Effect/Interface/IEnemyEffect.h"
#include "Effect/HitEffect/Circle/EnemyHitEffectCircle.h"
#include "Effect/OmniBack/EnemyOmniBackParticle.h"
#include "Effect/OmniBarst/EnemyOmniBarstParticle.h"
#include "Effect/OmniExplosion/EnemyOmniExplosionParticle.h"

#include "JsonManager/JsonEntityData.h"


// Player前方宣言
class Player;


/* EnemyManagerクラス */
class EnemyManager {

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyManager() = default;

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
	void Draw3D();

	/// <summary>
	/// Jsonで読み込んだ情報を受け取る
	/// </summary>
	void LoadEntityData(const std::vector<std::unique_ptr<EntityData>>& datas);

	/// <summary>
	/// 全滅したかのチェック
	/// </summary>
	void EliminatedChecker();


#pragma region Accessor アクセッサ

	// Playerの設定
	void SetPlayer(Player* setPlayer) { this->player_ = setPlayer; }

	// 全滅したかのフラグ
	bool IsEliminated() const { return this->isBossEliminated_; }

	// EnemyListの取得
	std::list<std::unique_ptr<IEnemy>>& GetEnemys() { return this->commonEnemies_; }

#pragma endregion 


private:

	/// <summary>
	/// 通常エネミーが全滅したかのチェック
	/// </summary>
	void Common_EliminatedChecker();

	/// <summary>
	/// ボスエネミーが全滅したかのチェック
	/// </summary>
	void Boss_EliminatedChecker();

	/// <summary>
	/// 新しいEnemyを生成する
	/// </summary>
	void CreateBasicEnemy(const EntityData& setEntityData);
	void CreateStaticEnemy(const EntityData& setEntityData);
	void CreateBossEnemy(const EntityData& setEntityData);

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawimGui();

private: // メンバ変数

	// ModelManager
	ModelManager* modelManager_ = nullptr;

	// Player
	Player* player_ = nullptr;

	// EnemyのList配列
	std::list<std::unique_ptr<IEnemy>> commonEnemies_;

	// BossEnemyのList配列
	std::list<std::unique_ptr<BossEnemy>> bossEnemies_;

	// OmniParticleのList配列
	std::list<std::shared_ptr<EnemyOmniBackParticle>> backParticles_;
	std::list<std::shared_ptr<EnemyOmniBarstParticle>> barstParticles_;
	std::list<std::shared_ptr<EnemyOmniExplosionParticle>> explosionParticles_;

	// 通常エネミーが全滅したかのフラグ
	bool isCommonEliminated_ = false;
	// ボスエネミーが全滅したかのフラグ
	bool isBossEliminated_ = false;

};

