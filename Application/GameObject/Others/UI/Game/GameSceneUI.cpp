#include "GameSceneUI.h"



// 初期化処理
void GameSceneUI::Init()
{
	// 配列のresize
	ui_.resize(EnumSize<GameSceneUIType>::value);

	// テクスチャの読み込み
	ui_[int(GameSceneUIType::ClearTask)].texture =
		TextureManager::LoadTexture("Texture/Game/GameSceneUI", "GameClearTask.png");

	// スプライトとトランスフォームの初期化
	for (int i = 0; i < ui_.size(); ++i) {
		ui_[i].sprite = std::make_unique<Sprite>();
		ui_[i].sprite->Init();
		ui_[i].sprite->SetTexture(ui_[i].texture);
		ui_[i].transform.Init();
	}

	// スプライトのオリジンを設定
	ui_[int(GameSceneUIType::ClearTask)].sprite->SetAnchor(SpriteAnchor::Center);

	// テクスチャのサイズ設定
	ui_[int(GameSceneUIType::ClearTask)].sprite->SetSize({ 480.0f, 96.0f });

	// 座標の設定
	ui_[int(GameSceneUIType::ClearTask)].transform.srt.translate = { 640.0f, 48.0f, 0.0f };
}


// 更新処理
void GameSceneUI::Update()
{
	// トランスフォームの更新
	for (int i = 0; i < ui_.size(); ++i) {
		ui_[i].transform.UpdateMatrix();
	}
}


// 描画処理
void GameSceneUI::Draw2DFront() // 前景
{
	for (int i = 0; i < ui_.size(); ++i) {
		DrawUI(i);
	}
}
void GameSceneUI::Draw2DBack() // 背景
{
}



// UIの描画
void GameSceneUI::DrawUI(int type)
{
	ui_[type].sprite->Draw(ui_[type].transform);
}

