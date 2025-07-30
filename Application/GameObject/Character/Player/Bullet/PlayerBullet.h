#pragma once

#include <memory>

#include "System/IObject/IObject.h"
#include "../../../GameObject.h"


class Player;


/* PlayerBulletクラス */
class PlayerBullet : public IObject {

public: // メンバ関数

	// コンストラクタ、デストラクタ
	PlayerBullet() { attribute_ = { Attributes::Category::PLAYER, Attributes::Type::BULLET }; };
	~PlayerBullet() {};

	// 初期化処理　更新処理　描画処理
	void Init() override;
	void Update() override;
	void Draw3D() override;
	void Draw2DFront() override;
	void Draw2DBack() override;

	// 衝突判定コールバック関数
	void onCollision([[maybe_unused]] IObject* object) override;

	// プールに返却前のリセット処理
	void Reset();

#pragma region Accessor アクセッサ

	// プレイヤー
	void SetPlayer(Player* setPlayer) { this->player_ = setPlayer; }

	// 速度
	Vector3 GetVelocity() const { return  this->velocity_; }
	void SetVelocity(Vector3 setVel) { this->velocity_ = setVel; }

	// 必ずSetVelocityの後に呼び出すこと
	void SetRotationFromVelocity() {
		transform_.srt.rotate.y = std::atan2(velocity_.x, velocity_.z);
		float velZ = std::sqrt((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
		float height = -velocity_.y;
		transform_.srt.rotate.x = std::atan2(height, velZ);
	}

	// 死亡フラグ
	bool IsDead() const { return this->isDead_; }

	// Collider
	//OBBCollider* GetOBBCollider() { return this->collider_.get(); }

#pragma endregion 


private:

	// 移動処理
	void Move();

	// Z軸を回転させる
	void ZRotate();

	// 寿命の処理
	void RemoveAfterlifeTime();

	// マークを死亡状態に設定
	void MarkAsDead();


private: // メンバ変数

	// プレイヤー
	Player* player_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_;

	// ライト
	DirectionalLightData light_{};

	// サイズ
	Vector3 size_ = { 2.0f, 2.0f, 2.0f };

	// コライダー
	//Col::Sphere sphere_;
	std::unique_ptr<SphereCollider> sphere_;

	// 移動速度
	Vector3 velocity_;

	// 回転速度
	float addRotateZ_ = 0.0f;

	// 寿命
	Timer life_{};

	// 死亡フラグ
	bool isDead_ = false;
};

