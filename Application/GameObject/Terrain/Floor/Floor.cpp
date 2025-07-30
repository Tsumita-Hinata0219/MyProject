#include "Floor.h"



// 初期化処理
void Floor::Init()
{
	// モデルの読み込み
	modelManager_->LoadModel("Obj/Floor", "Floor.obj");
	model_ = modelManager_->GetModel("Floor");

	// ライトの初期設定
	light_.enable = true;
	light_.direction = Vector3::oneY;
	light_.intensity = 0.7f;
	model_->SetLightData(light_);

	// トランスフォームの初期化
	trans_.Init();
}


// 更新処理
void Floor::Update()
{

#ifdef _DEBUG
	if (ImGui::TreeNode("Floor")) {
		trans_.DrawImGui();
		ImGui::TreePop();
	}
#endif // _DEBUG
}


// 描画処理
void Floor::Draw3D()
{
	model_->SetLightData(light_);
	model_->Draw(trans_);
}

void Floor::Draw2DFront()
{
}

void Floor::Draw2DBack()
{
}


// 衝突判定コールバック関数
void Floor::onCollision([[maybe_unused]] IObject* object)
{
}

