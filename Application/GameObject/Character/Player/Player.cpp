#include "Player.h"
#include "../../Camera/GameCamera/GameCamera.h"

/// <summary>
/// 初期化処理
/// </summary>
void Player::Init() 
{
    // Inputクラス
    input_ = Input::GetInstance();

    // BodyModelのロード
    modelManager_->LoadModel("Obj/Player/Body/Main", "Player_Main_Body.obj");
    modelManager_->LoadModel("Obj/Player/Body/Center",
                             "Player_Center_Body.obj");
    modelManager_->LoadModel("Obj/Player/Body/Left", "Player_Left_Body.obj");
    modelManager_->LoadModel("Obj/Player/Body/Right", "Player_Right_Body.obj");
    // BulletModelのロード
    modelManager_->LoadModel("Obj/Player/Bullet", "PlayerBullet.obj");

    // 各ボディの初期化とペアレントを結ぶ
    iBodys_.resize(EnumSize<PlayerBodyTyep>::value);
    iBodys_[enum_val(PlayerBodyTyep::MAIN)] =
        std::make_unique<PlayerMainBody>();
    iBodys_[enum_val(PlayerBodyTyep::LEFT)] =
        std::make_unique<PlayerLeftBody>();
    iBodys_[enum_val(PlayerBodyTyep::RIGHTM)] =
        std::make_unique<PlayerRightBody>();
    for (std::shared_ptr<IPlayerBody> body : iBodys_) {
        body->Init();
        body->SetParent(&transform_);
    }

    // ライトの初期設定
    light_.enable = true;
    light_.direction = Vector3::oneY;
    light_.intensity = 0.7f;

    // Transformの初期化
    transform_.Init();
    transform_.srt.translate.z = -5.0f;

    // パーティクルマネージャー
    particleManager_ = std::make_unique<PlayerParticleManager>();
    particleManager_->Init();
    particleManager_->SetPlayer(this);

    // 移動処理クラス
    movement_ = std::make_unique<PlayerMovement>();
    movement_->Init(this, gameCamera_, &transform_);

    // BulletのObjectPoolを先に作っておく
    bulletPool_.Create(50);

    // Colliderの初期化
    aabb_ = std::make_unique<AABBCollider>(this);
    aabb_->data_.center = transform_.GetWorldPos();
    aabb_->data_.size = {1.7f, 1.7f, 1.7f};

    // スイープイレイサーの初期化
    sweepEraser_ = std::make_unique<BulletSweepEraser>();
    sweepEraser_->Init();
    sweepEraser_->SetParent(&transform_); // ペアレントを結ぶ

    // キルカウントを0で初期化
    killCount_ = 0;

    // HPの設定
    hp_ = 3;

    // 無敵状態の時間を設定しておく
    invincibilityTime_ = invincibilityInterval_;
}

/// <summary>
///  更新処理
/// </summary>
void Player::Update() 
{
    // Transformの更新処理
    transform_.UpdateMatrix();

    // パーティクルマネージャー
    particleManager_->Update();

    // プレイヤーの操作関連
    movement_->Update();

    // 射撃の処理
    ExecuteShot();

    // Bulletの更新処理
    for (PlayerBullet *bullet : bulletList_) {
        bullet->Update();
    }
    // 死亡フラグが立っていたら削除
    bulletList_.remove_if([this](PlayerBullet *bullet) {
        if (bullet->IsDead()) {
            // 返却する前にリセット処理を入れておく
            bullet->Reset();
            // 死亡したバレットをプールに返却
            bulletPool_.Return(bullet);
            return true;
        }
        return false;
    });

    // ColliderのSRTの設定
    aabb_->data_.center = transform_.GetWorldPos();
    aabb_->Update();

    // スイープイレイサーの更新
    sweepEraser_->Update();

    // キルカウントが一定を超えていたら勝利フラグを立てる
    if (killCount_ >= MaxKillCount_) {
        isWin_ = true;
        isLose_ = false;
    }
    // 体力が0なら敗北フラグを立てる
    if (hp_ <= HP_MIN) {
        isWin_ = false;
        isLose_ = true;
        isDead_ = true;
    }

    // 無敵状態のタイマーを減らす処理
    if (isInvincibility_) {
        SubtructInvincibilityTime();
    }

    if (input_->Trigger(DIK_RETURN)) {
        isWin_ = true;
    }


#ifdef _DEBUG
    // ImGuiの描画
    DrawImGui();
#endif // _DEBUG
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw3D() 
{
    // BodyModelの描画
    for (std::shared_ptr<IPlayerBody> body : iBodys_) {
        body->SetModelColor(color_);
        body->SetModelLight(light_);
        body->Draw3D();
    }

    // ParticleManagerの描画
    particleManager_->Draw();

    // Bulletsの描画
    for (PlayerBullet *bullet : bulletList_) {
        bullet->Draw3D();
    }
}
void Player::Draw2DBack() {}
void Player::Draw2DFront() {}

/// <summary>
/// 衝突判定コールバック関数
/// </summary>
void Player::onCollision([[maybe_unused]] IObject *object) 
{
    // 地形は押し出し
    if (object->GetCategory() == Attributes::Category::TERRAIN) {

        // 押し出しの処理
        transform_.srt.translate +=
            Penetration::Execute(aabb_->GetData(), IObject::hitCollider_);
        transform_.UpdateMatrix();
    }
    if (object->GetCategory() == Attributes::Category::ENEMY &&
        object->GetType() == Attributes::Type::BULLET) {

        // 体力が0以下なら通らない
        if (hp_ <= HP_MIN)
            return;
        // 無敵状態なら通らない
        if (isInvincibility_)
            return;

		// 敵の弾との衝突処理
        OnCollisionWithEnemyBullet();

        // 近くにある敵の弾を消す
        sweepEraser_->StartSweep();

        // カメラのシェイク処理
        gameCamera_->ActivateShake();
    }
}
void Player::OnCollisionWithEnemy() {}
void Player::OnCollisionWithEnemyBullet() 
{
    // 無敵時間にする
    isInvincibility_ = true;

    // HP減少
    if (hp_ > HP_MIN) {
        hp_--;
    }

    // 体力がなければ消すモデルもないので通らない
    if (hp_ >= HP_MIN) {

        // 体力減少具合でボディを減らす
        iBodys_[hp_].reset();

        // nullになった要素を削除
        iBodys_.erase(
            std::remove_if(iBodys_.begin(), iBodys_.end(),
                           [](const std::shared_ptr<IPlayerBody> &body) {
                               return body == nullptr;
                           }),
            iBodys_.end());

        // ボディモデルのカラーを赤にする
        color_ = Temp::Color::RED;
    }
}

/// <summary>
/// 射撃処理
/// </summary>
void Player::ExecuteShot() 
{
    // キーorボタン押下でタイマーをデクリメント
    if (input_->Press(DIK_SPACE) || input_->Press(PadData::RIGHT_SHOULDER)) {

        shotPressFrame_--;

        // 射撃中のフラグを立てる
        isShooting_ = true;

        if (shotPressFrame_ <= 0) {
            // 0以下でタイマー再設定
            shotPressFrame_ = kShotInterval_;
            // バレット生成
            CreateNewBullet();
        }
    }
    // キーorボタンを離したら、最初の1発目がすぐ出るように1フレームを入れておく
    else if (input_->Release(DIK_SPACE) ||
             input_->Release(PadData::RIGHT_SHOULDER)) {

        shotPressFrame_ = kInitialShotPressFrame;

        // フラグを折る
        isShooting_ = false;
    }
}

/// <summary>
/// 新しいバレットを生成する
/// </summary>
void Player::CreateNewBullet() 
{
    // オブジェクトプール空新しいバレットを取得
    PlayerBullet *newBullet = bulletPool_.Get();

    // 初期座標
    Vector3 initPos = transform_.GetWorldPos();
    // 初期速度
    Vector3 initVel = Vector3::oneZ;
    initVel = TransformNormal(initVel, transform_.matWorld);

    // newBulletの初期化
    newBullet->Init();
    newBullet->SetTranslate(initPos);
    newBullet->SetVelocity(initVel);
    newBullet->SetRotationFromVelocity();
    newBullet->SetPlayer(this);

    // リストに追加
    bulletList_.push_back(newBullet);
}

/// <summary>
/// 無敵状態のタイマーを減らす処理
/// </summary>
void Player::SubtructInvincibilityTime() 
{
    // 定数定義
    constexpr int kColorResetThreshold =
        7; // モデルカラーをリセットするまでのフレーム数

    // 無敵状態の時間を減らす
    invincibilityTime_--;

    // 一定フレームたったら
    if (invincibilityTime_ == invincibilityInterval_ - kColorResetThreshold) {

        // モデルのカラーを白に戻す
        color_ = Temp::Color::WHITE;
    }

    if (invincibilityTime_ <= invincibilityResetTime_) {
        // 状態解除
        isInvincibility_ = false;
        // タイマーリセット
        invincibilityTime_ = invincibilityInterval_;
    }
}

/// <summary>
/// ImGuiの描画
/// </summary>
void Player::DrawImGui() 
{
    if (ImGui::TreeNode("Player")) {

        ImGui::Text("Transform");
        transform_.DrawImGui();
        ImGui::Text("");

        ImGui::Text("BodyColor");
        ImGui::ColorEdit4("color", &color_.x);
        ImGui::Text("");

        ImGui::Text("BodyLight");
        light_.DrawImGui();
        ImGui::Text("");

        ImGui::Text("KillCount = %d", killCount_);
        ImGui::Text("");

        ImGui::Text("HP = %d", hp_);
        ImGui::Text("");

        // ImGui::Text("入力関連");
        ImGui::Text("Input");
        ImGui::DragFloat2("L_Stick", &iLStick_.x, 0.0f);
        ImGui::Text("");

        ImGui::Text("Shoot");
        ImGui::Checkbox("isShooting", &isShooting_);
        ImGui::Text("");

        ImGui::Text("Bullet");
        int instance = int(bulletList_.size());
        ImGui::DragInt("Bullet_InstanceSize", &instance, 0);

        ImGui::Text("Invincibility");
        ImGui::Checkbox("Invincibility", &isInvincibility_);
        ImGui::Text("InvincibilityTime = %d", invincibilityTime_);
        ImGui::Text("");

        ImGui::Text("Movement");
        ImGui::Checkbox("isMovement", &movement_->IsMovement());
        ImGui::Text("");

        ImGui::TreePop();
    }
}
