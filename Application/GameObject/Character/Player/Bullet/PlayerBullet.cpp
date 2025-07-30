#include "PlayerBullet.h"
#include "../Player.h"
#include "../../EnemyManager/Bullet/EnemyBullet.h"


// 初期化処理
void PlayerBullet::Init()
{
	// BodyModelのロードと初期化
	modelManager_ = ModelManager::GetInstance();
	model_ = modelManager_->GetModel("PlayerBullet");

	// Transformの初期化。座標や姿勢の設定は呼び出し先でaccessorで設定
	transform_.Init();
	transform_.srt.rotate = Vector3::zero;

	// 速度の設定。呼び出し先でaccessorで設定

	// 寿命のタイマーをスタート。2秒で設定
	life_.Init(0.0f, 1.8f * 60.0f);
	life_.Start();

	// 回転速度の設定
	addRotateZ_ = 1.0f;

	// ライトの初期設定
	light_.enable = true;
	light_.direction = Vector3::one;

	// Colliderの初期化
	sphere_ = std::make_unique<SphereCollider>(this);
	sphere_->data_.center = transform_.GetWorldPos();
	sphere_->data_.radius = 1.7f;

	// 死亡フラグは折っておく
	isDead_ = false;
}


// 更新処理
void PlayerBullet::Update()
{
	// Transformの更新処理
	transform_.UpdateMatrix();

	// 移動処理
	Move();

	// Z軸を回転させる
	ZRotate();

	// 寿命の処理
	RemoveAfterlifeTime();

	// ColliderのSRTの設定
	sphere_->data_.center = transform_.GetWorldPos();
}


// 描画処理
void PlayerBullet::Draw3D()
{
	//model_->SetLightData(light_);
	model_->Draw(transform_);
}
void PlayerBullet::Draw2DFront() {}
void PlayerBullet::Draw2DBack() {}


// 衝突自コールバック関数
void PlayerBullet::onCollision([[maybe_unused]] IObject* object)
{
	if (object->GetCategory() == Attributes::Category::TERRAIN) {
		// 死亡状態に設定
		MarkAsDead();
	}
	if (object->GetCategory() == Attributes::Category::ENEMY && 
		object->GetType() == Attributes::Type::BODY) {
		// 死亡状態に設定
		MarkAsDead();
	}
	if (object->GetCategory() == Attributes::Category::ENEMY &&
		object->GetType() == Attributes::Type::BULLET) {

		// dynamic_castを使って、objectがEnemyBullet型か確認
		EnemyBullet* bullet = dynamic_cast<EnemyBullet*>(object);
		if (bullet) {
			// 死なない弾とは当たらない
			if (bullet->IsResistant()) {
				return;
			}
			// 死亡状態に設定
			MarkAsDead();
		}
	}
}


// プールに返却前のリセット処理
void PlayerBullet::Reset()
{
	// コライダーを無効にしておく
	sphere_->Deactivate();
}


// 移動処理
void PlayerBullet::Move()
{
	transform_.srt.translate += velocity_;
}


// Z軸を回転させる
void PlayerBullet::ZRotate()
{
	transform_.srt.rotate.z += ToRadians(addRotateZ_);
}


// 寿命の処理
void PlayerBullet::RemoveAfterlifeTime()
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
void PlayerBullet::MarkAsDead()
{
	// 死亡フラグを立てる
	isDead_ = true;
}
