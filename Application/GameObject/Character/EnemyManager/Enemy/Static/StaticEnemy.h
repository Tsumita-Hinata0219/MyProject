#pragma once

#include "../IEnemy.h"

#include "System/IObject/IObject.h"
#include "../../../../GameObject.h"

#include "../../Bullet/EnemyBullet.h"


/* 不動型のエネミー */
class StaticEnemy : public IEnemy {

public:

	// コンストラクタ、デストラクタ
	StaticEnemy() { attribute_ = { Attributes::Category::ENEMY, Attributes::Type::BODY }; };
	~StaticEnemy() {};

	// 初期化処理　更新処理　描画処理
	void Init() override;
	void Update() override;
	void Draw3D() override;
	void Draw2DFront() override;
	void Draw2DBack() override;

	// 衝突判定コールバック関数
	void onCollision([[maybe_unused]] IObject* object) override;


#pragma region Accessor アクセッサ

	// カラー
	Vector4 GetModelColor() const { return this->modelColor_; }
	void SetModelColor(Vector4 setColor) { this->modelColor_ = setColor; }

	// プレイヤー　
	void SetPlayer(Player* setPlayer) override { this->player_ = setPlayer; }
	// 死亡フラグ
	bool IsDead() const override { return this->isDead_; }
	void SetDeadFlag(bool setFlag) override { this->isDead_ = setFlag; }

	// HP
	uint32_t GetHP() override { return this->hp_; }

#pragma endregion 


private:

	// マークを死亡状態に設定
	void MarkAsDead();

private:

	// プレイヤー
	Player* player_ = nullptr;

	// サイズ
	Vector3 size_ = { 2.0f, 2.0f, 2.0f };

	// 死亡フラグ
	bool isDead_ = false;

	// ライト
	DirectionalLightData light_{};

	// コライダー
	std::unique_ptr<SphereCollider> sphere_;

	// カラー
	Vector4 modelColor_ = Vector4::one;

	// 回転スピード
	float addRadSpeed_ = 0.0f;

	// HP
	uint32_t hp_ = 0;

};

