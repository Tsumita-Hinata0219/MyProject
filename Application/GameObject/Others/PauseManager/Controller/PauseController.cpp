#include "PauseController.h"
#include "../PauseManager.h"


/// <summary>
/// 初期化処理
/// </summary>
void PauseController::Init()
{
	// 入力クラスのインスタンス取得
	input_ = Input::GetInstance();



}


/// <summary>
/// 更新処理
/// </summary>
void PauseController::Update()
{
	// ポーズ中でない場合
	if (m_pauseManager_->GetPauseState() == PauseState::UnPause)
	{
		if (input_->Trigger(PadData::START) || 
			input_->Trigger(DIK_ESCAPE) || input_->Trigger(DIK_TAB))
		{
			m_pauseManager_->InPause();
		}
	}

	// ポーズ中の場合
	if (m_pauseManager_->GetPauseState() == PauseState::Pause)
	{
		if (input_->Trigger(PadData::START) || 
			input_->Trigger(DIK_ESCAPE) || input_->Trigger(DIK_TAB)) 
		{
			m_pauseManager_->OutPause();
		}

		// LStickの入力を取得
		Vector2 iLStick_ = input_->GetLStick();
		// 左入力
		if (iLStick_.x < -DZone_ || input_->Trigger(PadData::LEFT) ||
			input_->Trigger(DIK_LEFT) || input_->Trigger(DIK_A)) 
		{
			m_pauseManager_->ToggleSelect(PauseSelect::GameExit);
		}
		// 右入力
		if (iLStick_.x > DZone_ || input_->Trigger(PadData::RIGHT) ||
			input_->Trigger(DIK_RIGHT) || input_->Trigger(DIK_D)) 
		{
			m_pauseManager_->ToggleSelect(PauseSelect::GamePlay);
		}

		// 決定ボタン
		if (input_->Trigger(PadData::A) || input_->Trigger(DIK_SPACE)) 
		{
			// ゲームに戻る
			if (m_pauseManager_->GetPauseSelect() == PauseSelect::GamePlay) {
				m_pauseManager_->OutPause();
			}
			// ゲームから出る
			if (m_pauseManager_->GetPauseSelect() == PauseSelect::GameExit) {
				m_pauseManager_->ExitGame();
			}
		}
	}
}
