#include "DestructibleBox.h"



/// <summary>
/// 初期化処理
/// </summary>
void DestructibleBox::Init()
{
	// モデルのロードと初期化
	modelManager_->LoadModel("Obj/Boxs/DestructibleBox", "DestructibleBox.obj");
	model_ = modelManager_->GetModel("DestructibleBox");

	// トランスフォームの初期化
	trans_.Init();
}


/// <summary>
/// 更新処理
/// </summary>
void DestructibleBox::Update()
{
}


/// <summary>
/// 描画処理
/// </summary>
void DestructibleBox::Draw3D()
{
	model_->Draw(trans_);
}
void DestructibleBox::Draw2DFront()
{
}
void DestructibleBox::Draw2DBack()
{
}


/// <summary>
/// 衝突判定コールバック関数
/// </summary>
void DestructibleBox::onCollision([[maybe_unused]] IObject* object)
{
}
