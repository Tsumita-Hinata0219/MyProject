#include "StageTransitionMenuBlur.h"


/// <summary>
/// 初期化処理
/// </summary>
void StageTransitionMenuBlur::Init()
{
	// ブラー
	blur_ = std::make_unique<RadialBlurEffect>();
	blur_->Init();

	blur_->SetMtlData(blurData_);

	// タイマー(1秒)
	timer_.Init(0.0f, 1.0f * 60.0f);
}


/// <summary>
/// 更新処理
/// </summary>
void StageTransitionMenuBlur::Update()
{
	// ステートが処理中
	if (state_ == MenuDirectionState::Processing) {
		timer_.Update(); // タイマー更新
		DirectionUpdate(); // 演出更新
	}

	// タイマーが終了したら終了時処理
	if (timer_.IsFinish()) {
		DirectionExsit();
	}

#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG
}


/// <summary>
/// 描画処理
/// </summary>
void StageTransitionMenuBlur::Draw2DFront()
{
	blur_->SetMtlData(blurData_);
	blur_->Draw();
}


/// <summary>
/// 演出開始
/// </summary>
void StageTransitionMenuBlur::DirectionStart() {}


/// <summary>
/// 演出更新
/// </summary>
void StageTransitionMenuBlur::DirectionUpdate() {}


/// <summary>
/// 演出終了
/// </summary>
void StageTransitionMenuBlur::DirectionExsit() {}


/// <summary>
/// ImGuiの描画
/// </summary>
void StageTransitionMenuBlur::DrawImGui()
{
	if (ImGui::TreeNode("StageTransitionMenu_Blur")) {

		blurData_.DrawImGui();
		ImGui::Text("");

		ImGui::TreePop();
	}
}

