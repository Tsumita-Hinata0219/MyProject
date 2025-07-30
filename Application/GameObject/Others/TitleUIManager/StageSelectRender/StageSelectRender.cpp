#include "StageSelectRender.h"

void StageSelectRender::Init()
{
	// Modelの設定
	auto manager = ModelManager::GetInstance();
	manager->LoadModel("Obj/StageSelect", "StageSelect.obj");
	manager->LoadModel("Obj/Brackets", "Brackets.obj");

	selectModel_ = manager->GetModel("StageSelect");
	bracketsModel_ = manager->GetModel("Brackets");

	selectTrans_.Init();
	selectTrans_.srt.scale = { 2.0f, 2.0f, 2.0f };
	selectTrans_.srt.rotate = { 0.0f, 0.0f, 0.0f };
	selectTrans_.srt.translate = { 0.0f, -4.15f, 18.0f };

	selectUVTrans_.Init();
	selectUVTrans_.srt.scale = { 0.298f, 1.0f, 1.0f };
	selectUVTrans_.srt.rotate = { 0.0f, 0.0f, 0.0f };
	selectUVTrans_.srt.translate = { -0.049f, 0.0f, 0.0f };

	bracketsTrans_.Init();
	bracketsTrans_.srt.scale = { 1.0f, 2.0f, 1.0f };
	bracketsTrans_.srt.rotate = { 0.0f, 0.0f, 0.0f };
	bracketsTrans_.srt.translate = { 0.0f, -4.15f, 18.0f };

	color_ = Temp::Color::BLACK;
}

void StageSelectRender::Update()
{

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}

void StageSelectRender::Draw3D()
{
	selectUVTrans_.UpdateMatrix();
	Matrix4x4 mat = MakeAffineMatrix(
		selectUVTrans_.srt.scale, selectUVTrans_.srt.rotate, selectUVTrans_.srt.translate);
	selectModel_->SetMaterialUVMat(mat);
	selectModel_->SetMaterialColor(color_);
	selectModel_->Draw(selectTrans_);
	bracketsModel_->SetMaterialColor(color_);
	bracketsModel_->Draw(bracketsTrans_);
}

void StageSelectRender::Draw2DFront()
{
}

void StageSelectRender::Draw2DBack()
{
}

void StageSelectRender::onCollision([[maybe_unused]] IObject* object)
{
}

void StageSelectRender::UVUpdate(uint32_t num)
{
	selectUVTrans_.srt.translate.x = -0.049f + 0.2f * (num - 1);
}

void StageSelectRender::DrawImGui()
{
	if (ImGui::TreeNode("StageSelectRender")) {
		selectTrans_.DrawImGui("select", 0.01f);
		ImGui::Text("");
		selectUVTrans_.DrawImGui("selectUV", 0.01f);
		ImGui::Text("");
		bracketsTrans_.DrawImGui("render", 0.01f);
		ImGui::Text("");
		ImGui::TreePop();
	}
}
