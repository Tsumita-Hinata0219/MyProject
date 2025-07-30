#include "StageTransitionMenuBackScreen.h"



/// <summary>
/// 初期化処理
/// </summary>
void StageTransitionMenuBackScreen::Init()
{
	// テクスチャ
	textureHandle_ = TextureManager::LoadTexture("Texture/Game/ClearDirection", "ClearDirectionBackScreen.png");

	// スプライト
	Vector2 screenSize = { 1280.0f, 720.0f };
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(screenSize);
	sprite_->SetTexture(textureHandle_);

	// トランスフォーム
	trans_.Init();

	// カラー
	color_ = Temp::Color::BLACK;
	color_.w = initAlpha_;

	// タイマー(1秒)
	timer_.Init(0.0f, 1.0f * 60.0f);
}


/// <summary>
/// 更新処理
/// </summary>
void StageTransitionMenuBackScreen::Update()
{
	// ステートが処理中以外なら早期return
	if (state_ != MenuDirectionState::Processing) { return; }

	timer_.Update(); // タイマー更新
	DirectionUpdate(); // 演出更新

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
void StageTransitionMenuBackScreen::Draw2DFront()
{
	sprite_->SetColor(color_);
	sprite_->Draw(trans_);
}


/// <summary>
/// 演出開始
/// </summary>
void StageTransitionMenuBackScreen::DirectionStart()
{
	if (state_ == MenuDirectionState::Idle) {

		// ステートを処理中へ
		state_ = MenuDirectionState::Processing;
		// タイマースタート
		timer_.Start();
	}
}


/// <summary>
/// 演出更新
/// </summary>
void StageTransitionMenuBackScreen::DirectionUpdate()
{
	// 初期値から目標値へアルファ値を補間する
	color_.w =
		Interpolate(initAlpha_, targetAlpha_, timer_.GetRatio(), Ease::OutExpo);
}


/// <summary>
/// 演出終了
/// </summary>
void StageTransitionMenuBackScreen::DirectionExsit()
{
	// ステートを終了へ
	state_ = MenuDirectionState::Finished;
}


/// <summary>
/// ImGuiの描画
/// </summary>
void StageTransitionMenuBackScreen::DrawImGui()
{
	if (ImGui::TreeNode("StageTransitionMenu_BackScreen")) {

		ImGui::Text("Transform");
		trans_.DrawImGui();
		ImGui::Text("");

		ImGui::ColorEdit4("Color", &color_.x);
		ImGui::Text("");

		ImGui::TreePop();
	}
}


