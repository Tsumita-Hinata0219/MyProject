#include "EnemyBullet.h"



EnemyBullet::EnemyBullet()
{
	attribute_ = { Attributes::Category::ENEMY, Attributes::Type::BULLET };

	// BodyModelのロードと初期化
	model_ = modelManager_->GetModel("EnemyBullet");
}

// 初期化処理
void EnemyBullet::Init()
{
	// 弾のタイプでカラーを変える
	Vector4 modelColor{};
	if (!isResistant_) {
		// ノーマルはオレンジ
		modelColor = { 1.0f, 0.2f, 0.0f, 1.0f };
	}
	else {
		// レジスタントは深紫
		modelColor = { 0.15f, 0.0f, 0.2f, 1.0f };
	}
	model_->SetMaterialColor(modelColor);

	// Transformの初期化。座標や姿勢の設定は呼び出し先でaccessorで設定
	transform_.Init();

	// 速度の設定。呼び出し先でaccessorで設定

	// 寿命のタイマーをスタート。10秒で設定
	life_.Init(0.0f, 3.0f * 60.0f);
	life_.Start();

	// Colliderの初期化
	sphere_ = std::make_unique<SphereCollider>(this);
	sphere_->data_.center = transform_.GetWorldPos();
	sphere_->data_.radius = 1.3f;

	// 死亡フラグは折っておく
	isDead_ = false;
}


// 更新処理
void EnemyBullet::Update()
{
	// Transformの更新処理
	transform_.UpdateMatrix();

	// 移動処理
	Move();

	// 寿命の処理
	RemoveAfterlifeTime();

	// ColliderのSRTの設定
	sphere_->data_.center = transform_.GetWorldPos();

}


// 描画処理
void EnemyBullet::Draw3D()
{
	model_->Draw(transform_);
}
void EnemyBullet::Draw2DFront() {}
void EnemyBullet::Draw2DBack() {}


// 衝突時コールバック関数
void EnemyBullet::onCollision([[maybe_unused]] IObject* object)
{
	// PlayerのBodyかBulletと衝突したら
	if (object->GetCategory() == Attributes::Category::PLAYER &&
		object->GetType() == Attributes::Type::BODY || 
		object->GetCategory() == Attributes::Category::PLAYER &&
		object->GetType() == Attributes::Type::BULLET) {

		// タイプが消えない弾ならreturn
		if (isResistant_) { return; }
		// 死亡状態に設定
		MarkAsDead();
	}
	else if (object->GetCategory() == Attributes::Category::PLAYER && object->GetType() == Attributes::Type::SWEERER) {
		// スイーパー相手なら死亡
		// 死亡状態に設定
		MarkAsDead();
	}
	else if (object->GetCategory() == Attributes::Category::TERRAIN) {

		// 地形相手なら死亡
		MarkAsDead();
	}
}


// プールに返却前のリセット処理
void EnemyBullet::Reset()
{
	// コライダーを無効にしておく
	sphere_->Deactivate();
}


// 移動処理
void EnemyBullet::Move()
{
	transform_.srt.translate += velocity_;
}


// 寿命の処理
void EnemyBullet::RemoveAfterlifeTime()
{
	// 寿命の更新
	life_.Update();

	// タイマーが規定値になったら
	if (life_.IsFinish()) {

		// 死亡状態に設定
		MarkAsDead();
		// 寿命のタイマーをクリアしとく
		life_.Clear();
	}
}


// マークを死亡状態に設定
void EnemyBullet::MarkAsDead()
{
	// 死亡フラグを立てる
	isDead_ = true;
}
