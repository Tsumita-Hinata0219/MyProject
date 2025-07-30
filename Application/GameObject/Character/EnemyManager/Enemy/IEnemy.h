#pragma once

#include <list>
#include <memory>
#include <functional>

#include "System/IObject/IObject.h"
#include "../../../GameObject.h"
#include "Transform/Structure/Transform.h"

#include "../BulletContainer/EnemyBulletContainer.h"
#include "../EffectContainer/EnemyEffectContainer.h"
#include "../Movement/EnemyMovement.h"
#include "../Bullet/EnemyBullet.h"


// Player前方宣言
class Player;
class EnemyManager;
class EnemyOmniBackParticle;
class EnemyOmniBarstParticle;
class EnemyOmniExplosionParticle;
struct SRTData;


/* Enemyの基底クラス */
class IEnemy : public IObject {

public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IEnemy() = default;


#pragma region Accessor アクセッサ

	// Playerの設定
	virtual void SetPlayer(Player* setPlayer) = 0;

	// EnemyManagerの設定
	virtual void SetEnemyManager(EnemyManager* setManager) { this->enemyManager_ = setManager; }

	// OmniBackParticleの設定
	virtual void SetOmniBackParticle(std::weak_ptr<EnemyOmniBackParticle> ptr) {this->wp_BackParticle_ = ptr;}

	// OmniBarstParticleの設定
	virtual void SetOmniBarstParticle(std::weak_ptr<EnemyOmniBarstParticle> ptr) { this->wp_BarstParticle_ = ptr; }

	// OmniExplosionParticleの設定
	virtual void SetOmniExplosionParticle(std::weak_ptr<EnemyOmniExplosionParticle> ptr) { this->wp_explosionParticle_ = ptr; }

	// 死亡フラグ
	virtual bool IsDead() const = 0;
	virtual void SetDeadFlag(bool setFlag) = 0;

	// SRT
	SRTData GetSRT() const { return this->transform_.srt; }
	void SetSRT(const SRTData& setSRT) { this->transform_.srt = setSRT; }
	void SetInitSRT(const SRTData& setSRT) { this->initSRT_ = setSRT; }

	// Scale
	Vector3 GetScale() const { return this->transform_.srt.scale; }
	void SetScale(const Vector3& setScale) { this->transform_.srt.scale = setScale; }

	// Rotate
	Vector3 GetRotate() const { return this->transform_.srt.rotate; }
	void SetRotate(const Vector3& setRotate) { this->transform_.srt.rotate = setRotate; }
	void SetRotate_Y(float setRotate){this->transform_.srt.rotate.y = setRotate;}

	// Translate
	Vector3 GetTranslate() const { return this->transform_.srt.translate; }
	void SetTranslate(const Vector3& setTranslate) { this->transform_.srt.translate = setTranslate; }

	// WorldPos
	Vector3 GetWorldPos() { return this->transform_.GetWorldPos(); }

	// MatWorld
	Matrix4x4 GetMatWorld() const { return this->transform_.matWorld; }

	// HP
	virtual uint32_t GetHP() = 0;	

	// ShotProperty
	enemy::ShotFuncData GetShotProperty() { return this->shotFuncData_; }
	void SetShotProperty(enemy::ShotFuncData data) { this->shotFuncData_ = data; }

	// MovementProperty
	void SetMovementProperty(enemy::MovementFuncData data) { this->movementData_ = data; }

#pragma endregion 


protected:

	/// <summary>
	/// ヒットリアクション
	/// </summary>
	void HitReaction();


private:

	/// <summary>
	/// スケールのリアクション
	/// </summary>
	void HitReaction_Scale();

	/// <summary>
	/// 色のアクション
	/// </summary>
	void HitReaction_Color();


protected:

	// 親ポインタ
	EnemyManager* enemyManager_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_;

	// 色加算
	ColorAddition colorAdd_{};
	 
	// 初期値
	SRTData initSRT_{};

	// 射撃データ
	enemy::ShotFuncData shotFuncData_;

	// 移動処理
	std::unique_ptr<EnemyMovement> movement_;
	// 移動データ
	enemy::MovementFuncData movementData_;

	// バレット管理クラス
	std::unique_ptr<EnemyBulletContainer> bulletContainer_;

	// エフェクト管理クラス
	std::unique_ptr<EnemyEffectContainer> effectContainer_;

	// バックパーティクル
	std::weak_ptr<EnemyOmniBackParticle> wp_BackParticle_;
	// バーストパーティクル
	std::weak_ptr<EnemyOmniBarstParticle> wp_BarstParticle_;
	// エクスプロージョンパーティクル
	std::weak_ptr<EnemyOmniExplosionParticle> wp_explosionParticle_;

	// ヒットリアクションフラグ
	bool isHitReactioning_ = false;
	// ヒットリアクションタイマー
	Timer hitReactionTimer_{};
	// リアクション時に使用する値
	Vector3 hitReactionScale_{};
	std::pair<float, float> hitReactionColor_;
};
