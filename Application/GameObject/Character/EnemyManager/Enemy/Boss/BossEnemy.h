#pragma once

#include "../IEnemy.h"
#include "System/IObject/IObject.h"
#include "Math/MyMath.h"

#include "../../Shield/EnemyShield.h"
#include "../../Bullet/EnemyBullet.h"


/* ボスエネミー */
class BossEnemy : public IEnemy {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossEnemy() { attribute_ = { Attributes::Category::ENEMY, Attributes::Type::BODY }; };
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossEnemy() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Draw3D() override;
	void Draw2DFront() override;
	void Draw2DBack() override;

	/// <summary>
	/// 衝突判定コールバック関数
	/// </summary>
	void onCollision([[maybe_unused]] IObject* object) override;

	/// <summary>
	/// バリアを壊す処理
	/// </summary>
	void CollapseShield();


#pragma region Accessor アクセッサ

	// プレイヤー　
	void SetPlayer(Player* setPlayer) override { this->player_ = setPlayer; }

	// 死亡フラグ
	bool IsDead() const override { return this->isDead_; }
	void SetDeadFlag(bool setFlag) override { this->isDead_ = setFlag; }

	// HP
	uint32_t GetHP() override { return this->hp_; }

#pragma endregion 


private:

	/// <summary>
	/// マークを死亡状態に設定
	/// </summary>
	void MarkAsDead();

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui();


private:

	// プレイヤー
	Player* player_ = nullptr;

	// サイズ
	Vector3 size_ = { 2.0f, 2.0f, 2.0f };

	// 死亡フラグ
	bool isDead_ = false;

	// ライト
	DirectionalLightData light_{};
	// カラー
	Vector4 modelColor_ = Vector4::one;

	// デカール
	DecalSetting decalData_; // デカール設定データ
	bool isDecalEnabled_;     // デカールが現在表示されているかどうかのフラグ

	// コライダー
	std::unique_ptr<SphereCollider> sphere_;

	// HP
	uint32_t hp_ = 0;

	// 移動速度
	Vector3 velocity_{};
	float moveVector_ = 0.05f;
	// playerとの最低距離
	float minToPlayer_ = 4.0f;

	// シールド　
	std::unique_ptr<EnemyShield> shield_;

};