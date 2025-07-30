#include "BossEnemy.h"
#include "../../../Player/Player.h"
#include "../../EnemyManager.h"
#include "../../Effect/OmniBarst/EnemyOmniBarstParticle.h"


/// <summary>
/// 初期化処理
/// </summary>
void BossEnemy::Init()
{
	// BodyModelのロードと初期化
	modelManager_ = ModelManager::GetInstance();
	modelManager_->LoadModel("Obj/Enemys/Boss", "Boss.obj");
	model_ = modelManager_->GetModel("Boss");

	// BodyTransfromの初期化
	transform_.Init();
	transform_.srt = initSRT_;

	// ライトの初期設定
	light_.enable = true;
	light_.direction = Vector3::oneY;
	light_.intensity = 0.7f;
	model_->SetLightData(light_);

	// 色加算の初期設定
	colorAdd_.enable = true;
	colorAdd_.addColor = Temp::Color::WHITE;
	colorAdd_.intensity = 0.0f;
	model_->SetColorAddition(colorAdd_);

	// 射撃処理クラス
	bulletContainer_ = std::make_unique<EnemyBulletContainer>();
	bulletContainer_->SetOwner(this);
	bulletContainer_->Init();

	// エフェクト管理クラス
	effectContainer_ = std::make_unique<EnemyEffectContainer>();
	effectContainer_->SetOwner(this);
	effectContainer_->Init();

	// 移動処理クラス
	movement_ = std::make_unique<EnemyMovement>(enemyManager_, this, player_);
	movement_->Init(movementData_);

	// ヒットリアクション関連数値の初期設定
	// ヒットリアクションフラグ
	isHitReactioning_ = false;
	// タイマー
	hitReactionTimer_.Init(0.0f, 0.4f * 60.0f);
	// スケール
	hitReactionScale_ = { 1.0f, 1.1f, 1.0f };
	// 色加算
	hitReactionColor_.first = 0.8f;
	hitReactionColor_.second = 0.0f;

	// シールドの初期化処理
	shield_ = std::make_unique<EnemyShield>();
	shield_->Init();
	shield_->SetParent(&transform_);

	// Colliderの初期化
	sphere_ = std::make_unique<SphereCollider>(this);
	sphere_->data_.center = transform_.GetWorldPos();
	sphere_->data_.radius = 1.8f * 1.0f;

	// HPの設定
	hp_ = 15;

	// デカールの初期設定
	decalData_.decalColor = Vector4(0.2f, 0.2f, 0.2f, 1.0f); // ひび割れの色 (濃いグレー)
	decalData_.decalStrength = 1.0f;                       // 強度
	decalData_.enable = 0;                                 // 初期状態ではデカールを無効に設定
	isDecalEnabled_ = false;

	// バリア関係まだちゃんと処理を作っていないので、ここで破壊しておく
	//CollapseShield();
}


/// <summary>
/// 更新処理
/// </summary>
void BossEnemy::Update()
{
	// シールドの更新
	shield_->Update();

	// 移動処理
	movement_->Update();

	// 射撃処理
	//bulletContainer_->Update();

	// エフェクト処理
	effectContainer_->Update();

	// コライダーの更新
	sphere_->data_.center = transform_.GetWorldPos();

	// particleEmitterの座標更新
	wp_BarstParticle_.lock()->SetEmitPos(transform_.GetWorldPos());
	wp_explosionParticle_.lock()->SetEmitPos(transform_.GetWorldPos());

	if (isDecalEnabled_) {
		// デカール行列を更新（ここではボスのワールド座標に合わせる）
		// MakeAffineMatrix はあなたのエンジンでMatrix4x4を生成する関数に置き換えてください
		Matrix4x4 decalWorldMatrix = MakeAffineMatrix(transform_.srt.scale, transform_.srt.rotate, transform_.srt.translate);

		// シェーダーのデカールUV計算 (decalPos.xy * 0.5f + 0.5f) に合わせるため、
		// ワールド座標をデカール空間に変換する行列 (デカールのワールド行列の逆行列) を設定します。
		decalData_.decalMatrix = Inverse(decalWorldMatrix);

		// デカールを有効化
		decalData_.enable = 1;
	}
	else {
		// デカールを無効化
		decalData_.enable = 0;
	}

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}


/// <summary>
/// 更新処理
/// </summary>
void BossEnemy::Draw3D()
{
	// BodyModelの描画
	model_->SetLightData(light_);
	model_->SetColorAddition(colorAdd_);
	model_->SetDecal(decalData_);
	model_->Draw(transform_);

	// バレットの描画
	bulletContainer_->Draw();

	// エフェクトの描画
	effectContainer_->Draw();

	// シールドの描画処理
	shield_->Draw3D();
}
void BossEnemy::Draw2DFront()
{
}
void BossEnemy::Draw2DBack()
{
}


/// <summary>
/// 衝突判定コールバック関数
/// </summary>
void BossEnemy::onCollision([[maybe_unused]] IObject* object)
{
	// 地形は押し出し
	if (object->GetCategory() == Attributes::Category::TERRAIN) {
		// 押し出しの処理
		transform_.srt.translate += Penetration::Execute(sphere_->GetData(), IObject::hitCollider_);
		transform_.UpdateMatrix();
	}
	else if (object->GetCategory() == Attributes::Category::PLAYER && object->GetType() == Attributes::Type::BULLET) {

		// HPを減らす
		//hp_--;

		// ヒットリアクション中にする
		isHitReactioning_ = true;

		// ヒットリアクションのタイマースタート
		hitReactionTimer_.Start();

		// エフェクトを出す
		effectContainer_->AddEffectInstance();

		isDecalEnabled_ = true;

		// HPが0以下なら死亡
		if (hp_ <= 0) {

			// particleEmitterの座標更新
			wp_BarstParticle_.lock()->SetEmitPos(transform_.GetWorldPos());
			wp_explosionParticle_.lock()->SetEmitPos(transform_.GetWorldPos());

			// particleを出す
			wp_BarstParticle_.lock()->Update();
			wp_explosionParticle_.lock()->Update();
			wp_BarstParticle_.lock()->Emit();
			wp_explosionParticle_.lock()->Emit();

			// 死亡状態に設定
			MarkAsDead();
			player_->AddKillCount();
		}
	}
}


/// <summary>
/// バリアを壊す処理
/// </summary>
void BossEnemy::CollapseShield()
{
	// シールドが壊れた時の処理
	shield_->OnShieldBroken();
}


/// <summary>
/// マークを死亡状態に設定
/// </summary>
void BossEnemy::MarkAsDead()
{
	// 死亡フラグを立てる
	isDead_ = true;
}


/// <summary>
/// DrawImGuiの描画
/// </summary>
void BossEnemy::DrawImGui()
{
	if (ImGui::TreeNode("BossEnemy")) {

		transform_.DrawImGui();
		ImGui::Text("");

		if (ImGui::Button("Shield_Break")) {
			CollapseShield();
		}
		ImGui::Text("");

		ImGui::TreePop();
	}
}
