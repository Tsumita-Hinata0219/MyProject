#pragma once

#include "GameObject/GameObject.h"

#include "Controller/PauseController.h"
#include "Renderer/PauseRenderer.h"


enum class PauseState {
	// ポーズ中
	Pause,
	// ポーズ処理中
	Pausing,
	// ポーズ解除処理中
	UnPausing,
	// ポーズ解除中
	UnPause,
};
enum class PauseSelect {
	// プレイに戻る
	GamePlay,
	// セレクトに戻る
	GameExit,
};


/* ポーズ処理マネージャー */
class PauseManager {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PauseManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PauseManager() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw2DFront();

	/// <summary>
	/// ポーズをかける
	/// </summary>
	void InPause();

	/// <summary>
	/// ポーズを解除する
	/// </summary>
	void OutPause();
	
	/// <summary>
	/// 選択を切り替える
	/// </summary>
	void ToggleSelect(PauseSelect toggleSelect);

	/// <summary>
	/// ゲームから出る
	/// </summary>
	void ExitGame();

#pragma region Accessor

	// ポーズ中かどうか
	bool IsPause() const { return m_isPause_; }

	// Exitが選択されたか
	bool IsSelectedExit() const { return m_isSelectedExit_; }

	// ポーズ処理ステート
	PauseState GetPauseState() const { return m_pauseState_; }

	// ポーズセレクト
	PauseSelect GetPauseSelect() const { return m_pauseSelect_; }

	// タイマー
	Timer GetFuncTimer() const { return m_funcTimer_; }

#pragma endregion 


private:

	/// <summary>
	/// ポーズを切り替え
	/// </summary>
	void TogglePause();

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui();

private:

	// ポーズ中かどうか
	bool m_isPause_ = false;
	// exitが押されたか
	bool m_isSelectedExit_ = false;

	// ポーズ処理ステート
	PauseState m_pauseState_ = PauseState::Pause;
	// ターゲットステート
	PauseState m_targetState_ = PauseState::Pause;

	// ポーズセレクト
	PauseSelect m_pauseSelect_ = PauseSelect::GamePlay;

	// 操作クラス
	std::unique_ptr<PauseController> m_pauseController_ = nullptr;
	// 描画クラス
	std::unique_ptr<PauseRenderer> m_pauseRenderer_ = nullptr;

	// タイマー
	Timer m_funcTimer_;
};
