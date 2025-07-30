#include "PlayerLeftBody.h"



// 初期化処理
void PlayerLeftBody::Init()
{
	// Modelの設定
	modelManager_ = ModelManager::GetInstance();
	model_ = modelManager_->GetModel("Player_Left_Body");

	// Transformの初期化
	transform_.Init();
}


// 更新処理
void PlayerLeftBody::Update() {}


// 描画処理
void PlayerLeftBody::Draw3D()
{
	model_->Draw(transform_);
}
void PlayerLeftBody::Draw2DFront() {}
void PlayerLeftBody::Draw2DBack() {}


// 衝突判定コールバック関数
void PlayerLeftBody::onCollision([[maybe_unused]] IObject* object)
{
}
