#include "PauseRenderer.h"
#include "../PauseManager.h"


/// <summary>
/// 初期化処理
/// </summary>
void PauseRenderer::Init()
{
	// テクスチャハンドル
	uint32_t backTexHD = TextureManager::LoadTexture("Texture/Game/Pause", "Pause_Back.png");
	uint32_t gamePauseUIHD = TextureManager::LoadTexture("Texture/Game/Pause", "GamePause_UI.png");
	uint32_t play_UITexHD = TextureManager::LoadTexture("Texture/Game/Pause", "Play_UI.png");
	uint32_t exit_UITexHD = TextureManager::LoadTexture("Texture/Game/Pause", "Exit_UI.png");
	uint32_t selectTexHD = TextureManager::LoadTexture("Texture/Game/Pause", "Select.png");


	// インとアウトのAlpha数値
	// Back
	m_BackUnPauseAlpha_ = 0.0f;
	m_BackPauseAlpha_ = 0.8f;
	// UI
	m_UIUnPauseAlpha_ = 0.0f;
	m_UIPauseAlpha_ = 1.0f;

	// 初期カラー
	// Back
	m_BackColor_ = Temp::Color::BLACK;
	m_BackColor_.w = m_BackUnPauseAlpha_;
	// UI
	m_GamePauseUIColor_ = Temp::Color::WHITE;
	m_GamePauseUIColor_.w = m_UIUnPauseAlpha_;
	m_PlayUIColor_ = Temp::Color::WHITE;
	m_PlayUIColor_.w = m_UIUnPauseAlpha_;
	m_ExitUIColor_ = Temp::Color::WHITE;
	m_ExitUIColor_.w = m_UIUnPauseAlpha_;


	// InOutのY軸座標
	m_UIUnPausePositionY_ = 580.0f;
	m_UIPausePositionY_ = 500.0f;


	// Back
	m_BackSprite_ = std::make_unique<Sprite>();
	m_BackSprite_->Init({ 1280.0f, 720.0f });
	m_BackSprite_->SetTexture(backTexHD);
	m_BackSprite_->SetAnchor(SpriteAnchor::TopLeft);
	m_BackTransform_.Init();

	// GamePauseUI
	m_GamePauseUISprite_ = std::make_unique<Sprite>();
	m_GamePauseUISprite_->Init({ 1280.0f, 720.0f });
	m_GamePauseUISprite_->SetTexture(gamePauseUIHD);
	m_GamePauseUISprite_->SetAnchor(SpriteAnchor::TopLeft);
	m_GamePauseUITransform_.Init();

	// Play
	m_PlayUISprite_ = std::make_unique<Sprite>();
	m_PlayUISprite_->Init({ 216.0f, 104.0f });
	m_PlayUISprite_->SetTexture(play_UITexHD);
	m_PlayUISprite_->SetAnchor(SpriteAnchor::Center);
	m_PlayUITransform_.Init();
	// 初期X軸座標
	m_PlayUITransform_.srt.translate.x = 760.0f;
	// 初期Y軸座標
	m_PlayUITransform_.srt.translate.y = m_UIPausePositionY_;

	// Exit
	m_ExitUISprite_ = std::make_unique<Sprite>();
	m_ExitUISprite_->Init({ 216.0f, 104.0f });
	m_ExitUISprite_->SetTexture(exit_UITexHD);
	m_ExitUISprite_->SetAnchor(SpriteAnchor::Center);
	m_ExitUITransform_.Init();
	// 初期X軸座標
	m_ExitUITransform_.srt.translate.x = 520.0f;
	// 初期Y軸座標
	m_ExitUITransform_.srt.translate.y = m_UIPausePositionY_;

	// Select
	m_SelectSprite_ = std::make_unique<Sprite>();
	m_SelectSprite_->Init({ 216.0f, 104.0f });
	m_SelectSprite_->SetTexture(selectTexHD);
	m_SelectSprite_->SetAnchor(SpriteAnchor::Center);
	m_SelectTransform_.Init();
	m_SelectTransform_.srt.scale.x = 1.1f;
	m_SelectTransform_.srt.scale.y = 0.6f;
	m_SelectTransform_.srt.translate.y = m_UIPausePositionY_;
}


/// <summary>
/// 更新処理
/// </summary>
void PauseRenderer::Update()
{
	// ポーズ処理中でなければ早期return
	if (!m_pauseManager_->IsPause()) {
		return;
	}

	// BackのAlpha
	CalcBackAlpha();
	// UIのAlpha
	CalcUIAlpha();
	// UIの座標の計算
	CalcUIPositionY();

	m_PlayUIColor_.x = m_PlayUIColor_.y = m_PlayUIColor_.z = 1.0f;
	m_ExitUIColor_.x = m_ExitUIColor_.y = m_ExitUIColor_.z = 1.0f;
	// タイマーが終了
	if (m_pauseManager_->GetPauseState() == PauseState::Pause) 
	{
		// UIのカラーを求める
		CalcUIColor();
		// セレクトの座標
		CalcSelectPosition();
	}
}


/// <summary>
/// 描画処理
/// </summary>
void PauseRenderer::Draw2DFront()
{
	// ポーズじゃない時は通らない
	if (m_pauseManager_->GetPauseState() == PauseState::UnPause) {
		return;
	}

	// Back
	m_BackSprite_->SetColor(m_BackColor_);
	m_BackSprite_->Draw(m_BackTransform_);

	// タイマーが終了していたら、セレクトの描画をする
	if (m_pauseManager_->GetPauseState() == PauseState::Pause) {
		m_SelectSprite_->Draw(m_SelectTransform_);
	}

	// GamePauseUI
	m_GamePauseUISprite_->SetColor(m_GamePauseUIColor_);
	m_GamePauseUISprite_->Draw(m_GamePauseUITransform_);
	// PlayUI
	m_PlayUISprite_->SetColor(m_PlayUIColor_);
	m_PlayUISprite_->Draw(m_PlayUITransform_);
	// ExitUI
	m_ExitUISprite_->SetColor(m_ExitUIColor_);
	m_ExitUISprite_->Draw(m_ExitUITransform_);
}


/// <summary>
/// ポーズをかける
/// </summary>
void PauseRenderer::InPause()
{
	// BackのAlphaの設定
	m_BackCalcAlpha_ = { m_BackUnPauseAlpha_, m_BackPauseAlpha_ };
	// UIのAlphaの設定
	m_UICalcAlpha_ = { m_UIUnPauseAlpha_, m_UIPauseAlpha_ };
	// UIのY軸座標
	m_CalcPositionY_ = { m_UIUnPausePositionY_, m_UIPausePositionY_ };
}


/// <summary>
/// ポーズを解除する
/// </summary>
void PauseRenderer::OutPause()
{
	// BackのAlphaの設定
	m_BackCalcAlpha_ = { m_BackPauseAlpha_, m_BackUnPauseAlpha_ };
	// UIのAlphaの設定
	m_UICalcAlpha_ = { m_UIPauseAlpha_, m_UIUnPauseAlpha_ };
	// UIのY軸座標
	m_CalcPositionY_ = { m_UIPausePositionY_, m_UIUnPausePositionY_ };
}


/// <summary>
/// BackのAlphaの計算
/// </summary>
void PauseRenderer::CalcBackAlpha()
{
	m_BackColor_.w = Interpolate(
		m_BackCalcAlpha_.first, m_BackCalcAlpha_.second,
		m_pauseManager_->GetFuncTimer().GetRatio(), Ease::OutSine);
}


/// <summary>
/// UIのAlphaの計算
/// </summary>
void PauseRenderer::CalcUIAlpha()
{
	m_GamePauseUIColor_.w = Interpolate(
		m_UICalcAlpha_.first, m_UICalcAlpha_.second,
		m_pauseManager_->GetFuncTimer().GetRatio(), Ease::OutSine);
	m_PlayUIColor_.w = Interpolate(
		m_UICalcAlpha_.first, m_UICalcAlpha_.second,
		m_pauseManager_->GetFuncTimer().GetRatio(), Ease::OutSine);
	m_ExitUIColor_.w = Interpolate(
		m_UICalcAlpha_.first, m_UICalcAlpha_.second,
		m_pauseManager_->GetFuncTimer().GetRatio(), Ease::OutSine);
}


/// <summary>
/// UIのカラーを求める
/// </summary>
void PauseRenderer::CalcUIColor()
{
	if (m_pauseManager_->GetPauseSelect() == PauseSelect::GamePlay) 
	{
		m_PlayUIColor_ = Temp::Color::BLACK;
		m_ExitUIColor_ = Temp::Color::WHITE;
	}
	else if (m_pauseManager_->GetPauseSelect() == PauseSelect::GameExit)
	{
		m_PlayUIColor_ = Temp::Color::WHITE;
		m_ExitUIColor_ = Temp::Color::BLACK;
	}
}


/// <summary>
/// UIの座標の計算
/// </summary>
void PauseRenderer::CalcUIPositionY()
{
	// Play
	m_PlayUITransform_.srt.translate.y = Interpolate(
		m_CalcPositionY_.first, m_CalcPositionY_.second,
		m_pauseManager_->GetFuncTimer().GetRatio(), Ease::OutExpo);
	// Exit
	m_ExitUITransform_.srt.translate.y = Interpolate(
		m_CalcPositionY_.first, m_CalcPositionY_.second,
		m_pauseManager_->GetFuncTimer().GetRatio(), Ease::OutExpo);
}


/// <summary>
/// セレクトの座標
/// </summary>
void PauseRenderer::CalcSelectPosition()
{
	if (m_pauseManager_->GetPauseSelect() == PauseSelect::GamePlay)
	{
		m_SelectTransform_.srt.translate = m_PlayUITransform_.srt.translate;
	}
	else if (m_pauseManager_->GetPauseSelect() == PauseSelect::GameExit) 
	{
		m_SelectTransform_.srt.translate = m_ExitUITransform_.srt.translate;
	}
	
}
