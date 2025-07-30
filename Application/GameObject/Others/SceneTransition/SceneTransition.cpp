#include "SceneTransition.h"



// 初期化処理
void SceneTransition::Init()
{
	// テクスチャの読み込み
	textureHandle_ = TextureManager::LoadTexture("Texture", "Fade.png");

	// スプライトの初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init({ 1280.0f, 720.0f });
	// Textureの設定
	sprite_->SetTexture(textureHandle_);
	// Colorは黒
	sprite_->SetColor(Temp::Color::BLACK);

	// トランスフォームの初期化
	trans_.Init();

	// Dissolve関連の初期化
	dissolve_.isActive = true;
	dissolve_.maskTexHandle = 
		TextureManager::LoadTexture("Texture", "noise0.png");

	// Mask画像の読み込み
	std::array<std::string, 4> maskTexName = {
		"MaskTex_5",
		"MaskTex_5",
		"MaskTex_5",
		"MaskTex_5",
	};
	for (auto& tex : maskTexName) {
		maskTextures_.push_back(TextureManager::LoadTexture("Texture", tex + ".png"));
	}

	// TransitionTimerの初期設定
	transitionDuration_ = 0.5f * 60.0f; // 2秒
	transitionTimer_.Init(0.0f, transitionDuration_);

	// Delayの設定
	inDelay_ = false;
	delayTimer_.Init(0.0f, 0.0f * 1.0f);
}


// 更新処理
void SceneTransition::Update()
{
	// DissolveDataの設定
	sprite_->SetDissolveData(dissolve_);

	// 遷移処理
	FuncTransition();

	// ディレイ
	FuncDelay();

#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG
}


// 描画処理
void SceneTransition::Draw2DFront()
{
	sprite_->Draw(trans_);
}


// フェードイン開始
void SceneTransition::StartFadeIn()
{
	if (nowState_ == TransitionState::Cloased) {

		// ステート変更
		nowState_ = TransitionState::Opening;

		// 閾値の開始と目標を設定
		startThreshold_ = 0.0f;
		targetThreshold_ = 1.0f;

		// マスク画像をランダムで設定
		Scope scope = { 0.0f, 3.9f };
		uint32_t index = uint32_t(RandomGenerator::getRandom(scope));
		dissolve_.maskTexHandle = maskTextures_[index];

		// タイマースタート
		transitionTimer_.Start();
	}
}


// フェードアウト開始
void SceneTransition::StartFadeOut()
{
	if (nowState_ == TransitionState::Opened) {

		// ステート変更
		nowState_ = TransitionState::Closing;

		// 閾値の開始と目標を設定
		startThreshold_ = 1.0f;
		targetThreshold_ = 0.0f;

		// マスク画像をランダムで設定
		Scope scope = { 0.0f, 3.9f };
		uint32_t index = uint32_t(RandomGenerator::getRandom(scope));
		dissolve_.maskTexHandle = maskTextures_[index];

		// タイマースタート
		transitionTimer_.Start();
	}
}


// 遷移処理
void SceneTransition::FuncTransition()
{
	// フラグが立っていたら早期return
	if (inDelay_) { return; }

	if (nowState_ == Opening || nowState_ == Closing) {

		transitionTimer_.Update(); // タイマー更新

		// 補間で閾値を変更
		dissolve_.threshold =
			startThreshold_ + (targetThreshold_ - startThreshold_) * 
			Ease::InSine(transitionTimer_.GetRatio());

		// タイマー終了
		if (transitionTimer_.IsFinish()) {

			// タイマー停止 & 時間の再設定
			transitionTimer_.Clear();
			transitionTimer_.Init(0.0f, transitionDuration_);
			
			// 遅延処理に入るフラグを立てる
			inDelay_ = true;
			// 遅延タイマースタート
			delayTimer_.Start();
		}
	}
}


// 閉じた後の遅延
void SceneTransition::FuncDelay()
{
	// フラグが立っていなかったら早期return
	if (!inDelay_) { return; }

	// タイマー更新
	delayTimer_.Update();

	// タイマー終了
	if (delayTimer_.IsFinish()) {

		// ステート変更
		if (nowState_ == Opening) {
			nowState_ = Opened;
		}
		else if (nowState_ == Closing) {
			nowState_ = Cloased;
		}

		// フラグを折る
		inDelay_ = false;

		// タイマー停止 & 時間の再設定
		delayTimer_.Clear();
		delayTimer_.Init(0.0f, 1.0f * 60.0f);
	}
}


// ImGuiの描画
void SceneTransition::DrawImGui()
{
	if (ImGui::TreeNode("SceneTransition")) {

		ImGui::Text("Dissolve");
		ImGui::RadioButton("Disabled", &dissolve_.isActive, 0); ImGui::SameLine();
		ImGui::RadioButton("Enabled", &dissolve_.isActive, 1);
		ImGui::DragFloat("threshold", &dissolve_.threshold, 0.01f, 0.0f, 1.0f);
		ImGui::TreePop();
	}
}
