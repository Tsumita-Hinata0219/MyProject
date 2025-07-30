#include "PauseManager.h"



/// <summary>
/// 初期化処理
/// </summary>
void PauseManager::Init()
{
	// 操作クラス
	m_pauseController_ = std::make_unique<PauseController>(this);
	m_pauseController_->Init();

	// 描画クラス                        PuaseRenderer
	m_pauseRenderer_ = std::make_unique<PauseRenderer>(this);
	m_pauseRenderer_->Init();

	// タイマーの設定
	m_funcTimer_.Init(0.0f, 0.5f * 60.0f);


	// フラグは折っておく
	m_isPause_ = false;
	m_isSelectedExit_ = false;

	// ポーズステートはポーズ解除中
	m_pauseState_ = PauseState::UnPause;
}


/// <summary>
/// 更新処理
/// </summary>
void PauseManager::Update()
{
	// 入力処理
	m_pauseController_->Update();

	// ポーズの切り替え処理
	TogglePause();

	// 描画処理
	m_pauseRenderer_->Update();

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG

}


/// <summary>
/// 描画処理
/// </summary>
void PauseManager::Draw2DFront()
{
	m_pauseRenderer_->Draw2DFront();
}


/// <summary>
/// ポーズをかける
/// </summary>
void PauseManager::InPause()
{
	if (m_pauseState_ == PauseState::UnPause)
	{
		// ポーズ中にする
		m_pauseState_ = PauseState::Pausing;
		// ターゲットステート
		m_targetState_ = PauseState::Pause;
		// タイマースタート
		m_funcTimer_.Start();

		// 描画処理のポーズをかける処理
		m_pauseRenderer_->InPause();

		// ポーズ中のフラグを立てる
		m_isPause_ = true;
	}
}


/// <summary>
/// ポーズを解除する
/// </summary>
void PauseManager::OutPause()
{
	if (m_pauseState_ == PauseState::Pause)
	{
		// ポーズ解除中にする
		m_pauseState_ = PauseState::UnPausing;
		// ターゲットステート
		m_targetState_ = PauseState::UnPause;
		// タイマースタート
		m_funcTimer_.Start();

		// 描画処理のポーズを解除する処理
		m_pauseRenderer_->OutPause();
	}
}


/// <summary>
/// 選択を切り替える
/// </summary>
void PauseManager::ToggleSelect(PauseSelect toggleSelect)
{
	m_pauseSelect_ = toggleSelect;
}


/// <summary>
/// ゲームから出る
/// </summary>
void PauseManager::ExitGame()
{
	m_isSelectedExit_ = true;
}


/// <summary>
/// ポーズを切り替え
/// </summary>
void PauseManager::TogglePause()
{
	if (m_pauseState_ == PauseState::Pausing || 
		m_pauseState_ == PauseState::UnPausing)
	{
		// タイマー更新
		m_funcTimer_.Update();
		// タイマー終了
		if (m_funcTimer_.IsFinish())
		{
			// ターゲットステートを設定
			m_pauseState_ = m_targetState_;
			// ステートがPauseならフラグを立てる
			m_isPause_ = (m_pauseState_ == PauseState::Pause);
			// タイマーリセット
			m_funcTimer_.Reset();
		}
	}
}


/// <summary>
/// ImGuiの描画
/// </summary>
void PauseManager::DrawImGui()
{
	if (ImGui::TreeNode("PauseManager")) {

		if (m_pauseState_ == PauseState::Pause)
			ImGui::Text("PauseState : Pause");
		if (m_pauseState_ == PauseState::Pausing)
			ImGui::Text("PauseState : Pausing");
		if (m_pauseState_ == PauseState::UnPausing)
			ImGui::Text("PauseState : UnPausing");
		if (m_pauseState_ == PauseState::UnPause)
			ImGui::Text("PauseState : UnPause");
		ImGui::Text("");

		ImGui::Checkbox("IsPause", &m_isPause_);
		ImGui::Text("");

		if (m_pauseSelect_ == PauseSelect::GamePlay)
			ImGui::Text("PauseSelect : Play");
		if (m_pauseSelect_ == PauseSelect::GameExit)
			ImGui::Text("PauseSelect : Exit");

		ImGui::TreePop();
	}
}


