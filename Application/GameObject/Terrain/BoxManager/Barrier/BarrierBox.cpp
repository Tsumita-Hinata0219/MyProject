#include "BarrierBox.h"



/// <summary>
/// 初期化処理
/// </summary>
void BarrierBox::Init()
{
	// モデルのロードと初期化
	modelManager_->LoadModel("Obj/Boxs/BarrierBox", "BarrierBox.obj");
	model_ = modelManager_->GetModel("BarrierBox");

	// ライトの初期設定
	light_.enable = true;
	light_.direction = Vector3::one;
	light_.intensity = 0.7f;
	model_->SetLightData(light_);

	// トランスフォームの初期化
	trans_.Init();
	trans_.srt = initSRT_;

	// Colliderの初期化
	aabb_ = std::make_unique<AABBCollider>(this);
	aabb_->data_.center = trans_.GetWorldPos();
	aabb_->data_.size = { 
		2.0f * trans_.srt.scale.x, 
		2.0f * trans_.srt.scale.y,
		2.0f * trans_.srt.scale.z };
}


/// <summary>
/// 更新処理
/// </summary>
void BarrierBox::Update()
{
	// ColliderのSRTの設定
	aabb_->data_.center = trans_.GetWorldPos();
	aabb_->Update();
}


/// <summary>
/// 描画処理
/// </summary>
void BarrierBox::Draw3D()
{
	model_->Draw(trans_);
}
void BarrierBox::Draw2DFront()
{
}
void BarrierBox::Draw2DBack()
{
}


/// <summary>
/// 衝突判定コールバック関数
/// </summary>
void BarrierBox::onCollision([[maybe_unused]] IObject* object)
{
}
