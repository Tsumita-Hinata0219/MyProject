#include "Skydome.h"



// 初期化処理
void Skydome::Init()
{
	modelManager_ = ModelManager::GetInstance();
	modelManager_->LoadModel("Obj/Skydome", "Skydome.obj");
	model_ = modelManager_->GetModel("Skydome");

	trans_.Init();
	trans_.srt.scale = { 10000.0f, 10000.0f, 10000.0f };
}


// 更新処理
void Skydome::Update() {}


// 描画処理
void Skydome::Draw3D()
{
	model_->Draw(trans_);
}
void Skydome::Draw2DFront() {}
void Skydome::Draw2DBack() {}


// 衝突判定コールバック関数
void Skydome::onCollision([[maybe_unused]] IObject* object)
{
}
