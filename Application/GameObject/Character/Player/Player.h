#pragma once

#include <list>
#include <memory>

#include "../../GameObject.h"
#include "System/IObject/IObject.h"

#include "Boby/IPlayerBody.h"
#include "Boby/Left/PlayerLeftBody.h"
#include "Boby/Main/PlayerMainBody.h"
#include "Boby/Right/PlayerRightBody.h"

#include "Bullet/PlayerBullet.h"

#include "Movement/PlayerMovement.h"

#include "Particle/Manager/PlayerParticleManager.h"

#include "BulletSweep/BulletSweepEraser.h"

#include "OperationUI/OperationUIManager.h"

#include "JsonManager/JsonEntityData.h"

// 前方宣言
class GameCamera;

/* Playerクラス */
class Player : public IObject {

public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    Player() {
        attribute_ = {Attributes::Category::PLAYER, Attributes::Type::BODY};
    }

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Player(){};

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Init() override;

    /// <summary>
    ///  更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw3D() override;
    void Draw2DFront() override;
    void Draw2DBack() override;

    /// <summary>
    /// 衝突判定コールバック関数
    /// </summary>
    void onCollision([[maybe_unused]] IObject *object) override;

    /// <summary>
    /// Jsonで読み込んだ情報を受け取る
    /// </summary>
    void LoadEntityData(const std::vector<std::unique_ptr<EntityData>> &data) {

        for (const auto &entityData : data) {
            if (entityData) {
                // SRTの設定
                transform_.srt = entityData->srt;
            }
        }
    }

#pragma region Accessor アクセッサ

    // ゲームカメラ
    void SetGameCamera(GameCamera *setCamera) { this->gameCamera_ = setCamera; }

    // 射撃フラグ
    bool IsShooting() const { return this->isShooting_; }

    // KillCount
    uint32_t GetKillCount() const { return this->killCount_; }

    // KillCount加算 & 減算
    void AddKillCount(uint32_t addCount = 1) { this->killCount_ += addCount; }
    void SubKillCount(uint32_t subCount = 1) { this->killCount_ -= subCount; }

    // 死亡フラグ
    bool IsDead() const { return this->isDead_; }
    void SetDeadFlag(bool setFlag) { this->isDead_ = setFlag; }

    // 移動フラグ
    bool IsMovement() const { return this->movement_->IsMovement(); }

    // HP
    uint32_t GetHP() const { return this->hp_; }
    void SetHP(uint32_t setHP) { this->hp_ = setHP; }

    // 勝敗のフラグ
    bool IsWin() const { return this->isWin_; }
    bool IsLose() const { return this->isLose_; }

#pragma endregion

#pragma region Collision 衝突判定

    // 衝突自コールバック関数
    void OnCollisionWithEnemy();
    void OnCollisionWithEnemyBullet();

#pragma endregion

private:
    /// <summary>
    /// 射撃処理
    /// </summary>
    void ExecuteShot();

    /// <summary>
    /// 新しいバレットを生成する
    /// </summary>
    void CreateNewBullet();

    /// <summary>
    /// 無敵状態のタイマーを減らす処理
    /// </summary>
    void SubtructInvincibilityTime();

    /// <summary>
    /// ImGuiの描画
    /// </summary>
    void DrawImGui();

private:

    // Light
    DirectionalLightData light_;

    // コライダー
    std::unique_ptr<AABBCollider> aabb_;

    // サイズ
    Vector3 size_ = {2.0f, 2.0f, 2.0f};

    // 死亡フラグ
    bool isDead_ = false;

    // HP
    uint32_t hp_ = 0;
    const uint32_t HP_MIN = 0;

    // キルカウント
    uint32_t killCount_ = 0;
    const uint32_t MaxKillCount_ = 15;

    // ゲームに勝利したかのフラグ
    bool isWin_ = false;
    bool isLose_ = false;

    // 無敵中
    bool isInvincibility_ = false;

    // 無敵時間 インターバル時間
    uint32_t invincibilityTime_ = 0;
    uint32_t invincibilityInterval_ = 2 * 60;   // 2秒
    const uint32_t invincibilityResetTime_ = 0; // リセット値

#pragma region System

    // Inputクラス
    Input *input_ = nullptr;

    // デッドゾーン
    const float DZone_ = 0.2f;

    // Stickの入力を取得
    Vector2 iLStick_{};

    // Keyの入力
    Vector2 iKeys_{};

#pragma endregion


#pragma region ボディ関連

    // リスト
    std::vector<std::shared_ptr<IPlayerBody>> iBodys_;

    // カラー　
    Vector4 color_ = Temp::Color::WHITE;

#pragma endregion

    
#pragma region パーティクル

    std::unique_ptr<PlayerParticleManager> particleManager_;

#pragma endregion

    
#pragma region 移動処理クラス

    std::unique_ptr<PlayerMovement> movement_;

#pragma endregion

    
#pragma region バレット関連

    // オブジェクトプール
    ObjectPool<PlayerBullet> bulletPool_;

    // バレットリスト
    std::list<PlayerBullet *> bulletList_;

    // 射撃中かのフラグ
    bool isShooting_ = false;

    // バレットの速度
    float kBulletSpeed_ = 0.5f;

    // 射撃ボタン押下フレーム&インターバル
    int shotPressFrame_ = 0;
    int kShotInterval_ = 5;
    const int kInitialShotPressFrame = 1;

#pragma endregion

    
#pragma region スイープ関連

    std::unique_ptr<BulletSweepEraser> sweepEraser_;

#pragma endregion

    
#pragma region ゲームカメラ

    GameCamera *gameCamera_ = nullptr;

#pragma endregion

};
