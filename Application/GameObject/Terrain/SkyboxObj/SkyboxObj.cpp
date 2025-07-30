#include "SkyboxObj.h"


/// <summary>
/// 初期化処理
/// </summary>
void SkyboxObj::Init()
{
	// モデルの読み込み
	modelManager_->LoadModel("Obj/Skybox", "Skybox.obj");
	model_ = modelManager_->GetModel("Skybox");

	// トランスフォームの初期化
	trans_.Init();
	float initScale = 1000.0f;
	trans_.srt.scale = { initScale, initScale, initScale };
}


/// <summary>
///  更新処理
/// </summary>
void SkyboxObj::Update()
{
}


/// <summary>
/// 描画処理
/// </summary>
void SkyboxObj::Draw3D()
{
	model_->Draw(trans_);
}
void SkyboxObj::Draw2DFront()
{
}
void SkyboxObj::Draw2DBack()
{
}


/// <summary>
/// 衝突判定コールバック関数
/// </summary>
void SkyboxObj::onCollision([[maybe_unused]] IObject* object)
{
}

