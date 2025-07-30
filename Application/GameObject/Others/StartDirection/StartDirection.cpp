#include "StartDirection.h"



// 初期化処理
void StartDirection::Init()
{
	// テクスチャの読み込み
	textures_.resize(4);
	textures_ = {
		TextureManager::LoadTexture("Texture/Number", "3.png"),
		TextureManager::LoadTexture("Texture/Number", "2.png"),
		TextureManager::LoadTexture("Texture/Number", "1.png"),
		TextureManager::LoadTexture("Texture/Number", "start.png"),
	};
	texArrayNum_ = 0;

	// サイズ
	texSize_ = Vector2::zero;
	targetSize_ = { 256.0f, 256.0f };

	// 姿勢
	startRotateZ_ = ToRadians(-360.0f);
	targetRotateZ_ = ToRadians(0.0f);

	// スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Init(texSize_);
	sprite_->SetTexture(textures_[texArrayNum_]);
	sprite_->SetAnchor(SpriteAnchor::Center); // 中心原点

	// トランスフォーム
	trans_.Init();
	trans_.srt.rotate.z = ToRadians(-360.0f);
	trans_.srt.translate = {
		WinApp::kHalfWindowWidth,
		WinApp::kHalfWindowHeight,
		0.0f,
	};

	// Timer
	timer_.Init(0.0f, 0.8f * 60.0f);
	timer_.Start();
}


// 更新処理
void StartDirection::Update()
{
	if (isFinish_) { return; }

	// Timerの更新
	timer_.Update();
	// taimerが終了したらループ
	if (timer_.IsFinish()) {

		// textureの変更
		texArrayNum_++;
		if (texArrayNum_ < textures_.size()) {

			sprite_->SetTexture(textures_[texArrayNum_]);

			timer_.Init(0.0f, 0.8f * 60.0f);
			timer_.Start();
		}
		else if (texArrayNum_ >= textures_.size()) {
			isFinish_ = true;
		}
	}

	// サイズの移行処理
	SizeFunc();

	// 姿勢の移行処理
	RotateFunc();

#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG
}


// 描画処理
void StartDirection::Draw2DFront()
{
	if (isFinish_) { return; }
	sprite_->Draw(trans_);
}


// サイズの移行処理
void StartDirection::SizeFunc()
{
	// 補間でサイズ変更
	texSize_ = 
		Vector2::zero + (targetSize_ - Vector2::zero) * Ease::OutCirc(timer_.GetRatio());

	// サイズの設定
	sprite_->SetSize(texSize_);
}


// 姿勢の移行処理
void StartDirection::RotateFunc()
{
	// 補間でサイズ変更
	trans_.srt.rotate.z =
		startRotateZ_ + (targetRotateZ_ - startRotateZ_) * Ease::OutExpo(timer_.GetRatio());
}


// ImGuiの描画
void StartDirection::DrawImGui()
{
	if (ImGui::TreeNode("StartDirection")) {

		trans_.DrawImGui();

		ImGui::TreePop();
	}
}


