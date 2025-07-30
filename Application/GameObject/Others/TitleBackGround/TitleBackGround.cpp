#include "TitleBackGround.h"



// 初期化処理
void TitleBackGround::Init()
{
	// 配列のresize
	back_.resize(EnumSize<TitleSceneBGType>::value);

	// テクスチャの読み込み　
	back_[enum_val(TitleSceneBGType::Monochromatic)].texture =
		TextureManager::LoadTexture("Texture/Title", "TitleBackGround.png");
	back_[enum_val(TitleSceneBGType::Vignetting)].texture =
		TextureManager::LoadTexture("Texture/Title", "TitleVignetting.png");

	// スプライトとトランスフォームの初期化と設定
	for (auto& element : back_) {

		element.sprite = std::make_unique<Sprite>();
		element.sprite->Init();
		// オリジンの設定
		element.sprite->SetAnchor(SpriteAnchor::Center);
		// サイズの設定
		element.sprite->SetSize(WinApp::WindowSize());
		// テクスチャの設定
		element.sprite->SetTexture(element.texture);

		element.transform.Init();
		// 座標の設定
		element.transform.srt.translate = { WinApp::kHalfWindowWidth, WinApp::kHalfWindowHeight, 0.0f };
	}
}


// 更新処理
void TitleBackGround::Update()
{
	
}


// 描画処理
void TitleBackGround::Draw2DBack()
{
	back_[enum_val(TitleSceneBGType::Monochromatic)].sprite->Draw(
		back_[enum_val(TitleSceneBGType::Monochromatic)].transform);
}

void TitleBackGround::Draw2DFront()
{
	back_[enum_val(TitleSceneBGType::Vignetting)].sprite->Draw(
		back_[enum_val(TitleSceneBGType::Vignetting)].transform);
}
