#include "PlayerMainBody.h"



// 初期化処理
void PlayerMainBody::Init()
{
	// Modelの設定
	modelManager_ = ModelManager::GetInstance();
	models_.resize(2);
	models_[0] = modelManager_->GetModel("Player_Main_Body");
	models_[1] = modelManager_->GetModel("Player_Center_Body");

	// Transformの初期化
	transform_.Init();
}


// 更新処理
void PlayerMainBody::Update() {}


// 描画処理
void PlayerMainBody::Draw3D()
{
	for (int i = 0; i < 2; ++i) {
		models_[i]->Draw(transform_);
	}
}
void PlayerMainBody::Draw2DFront() {}
void PlayerMainBody::Draw2DBack() {}


// 衝突判定コールバック関数
void PlayerMainBody::onCollision([[maybe_unused]] IObject* object)
{
}
