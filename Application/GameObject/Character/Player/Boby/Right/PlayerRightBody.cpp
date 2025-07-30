#include "PlayerRightBody.h"



// 初期化処理
void PlayerRightBody::Init()
{
	// Modelの設定
	modelManager_ = ModelManager::GetInstance();
	model_ = modelManager_->GetModel("Player_Right_Body");

	// Transformの初期化
	transform_.Init();
}


// 更新処理
void PlayerRightBody::Update() {}


// 描画処理
void PlayerRightBody::Draw3D()
{
	model_->Draw(transform_);
}
void PlayerRightBody::Draw2DFront() {}
void PlayerRightBody::Draw2DBack() {}


// 衝突判定コールバック関数
void PlayerRightBody::onCollision([[maybe_unused]] IObject* object)
{
}
