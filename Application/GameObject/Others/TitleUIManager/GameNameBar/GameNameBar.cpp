#include "GameNameBar.h"



// 初期化処理
void GameNameBar::Init()
{
	// Modelの設定
	modelManager_ = ModelManager::GetInstance();
	modelManager_->LoadModel("Obj/GameNameBar", "GameNameBar.obj");
	model_ = modelManager_->GetModel("GameNameBar");

	uint32_t dds = TextureManager::LoadTexture("Obj/GameNameBar", "GameNameBar.dds");
	model_->SetMaterialTexture(dds);

	// Transformの初期化
	trans_.Init();
	float initScale = 6.0f;
	trans_.srt.scale = { initScale, initScale, initScale };
	trans_.srt.translate.y = 1.2f;
	trans_.srt.translate.z = 20.0f;
}


// 更新処理
void GameNameBar::Update()
{

#ifdef _DEBUG

	if (ImGui::TreeNode("GameNameBar")) {

		trans_.DrawImGui();
		ImGui::TreePop();
	}

#endif // _DEBUG
}


// 描画処理
void GameNameBar::Draw3D()
{
	model_->Draw(trans_);
}
void GameNameBar::Draw2DFront() {}
void GameNameBar::Draw2DBack() {}


// 衝突判定コールバック関数
void GameNameBar::onCollision([[maybe_unused]] IObject* object)
{
}
