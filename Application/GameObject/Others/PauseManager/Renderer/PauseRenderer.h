#pragma once

#include "GameObject/GameObject.h"


// 前方宣言
class PauseManager;


/* ポーズ時の描画クラス */
class PauseRenderer {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PauseRenderer() = default;
	PauseRenderer(PauseManager* pauseManager) : m_pauseManager_(pauseManager) {}

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PauseRenderer() = default;

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


private:

	/// <summary>
	/// BackのAlphaの計算
	/// </summary>
	void CalcBackAlpha();

	/// <summary>
	/// UIのAlphaの計算
	/// </summary>
	void CalcUIAlpha();

	/// <summary>
	/// UIのカラーを求める
	/// </summary>
	void CalcUIColor();

	/// <summary>
	/// UIの座標の計算
	/// </summary>
	void CalcUIPositionY();

	/// <summary>
	/// セレクトの座標
	/// </summary>
	void CalcSelectPosition();


private:

	// ポーズマネージャー
	PauseManager* m_pauseManager_ = nullptr;

	// 補間数値
	std::pair<float, float> m_BackCalcAlpha_{};
	std::pair<float, float> m_UICalcAlpha_{};
	std::pair<float, float> m_CalcPositionY_{};

	// InOutのAlpha数値
	// Back
	float m_BackUnPauseAlpha_ = 0.0f;
	float m_BackPauseAlpha_ = 0.0f;
	// UI
	float m_UIUnPauseAlpha_ = 0.0f;
	float m_UIPauseAlpha_ = 0.0f;

	// InOutの座標
	float m_UIUnPausePositionY_ = 0.0f;
	float m_UIPausePositionY_ = 0.0f;


	// カラー
	// Back
	Vector4 m_BackColor_{};
	// UI
	Vector4 m_GamePauseUIColor_{};
	Vector4 m_PlayUIColor_{};
	Vector4 m_ExitUIColor_{};

	// Back
	std::unique_ptr<Sprite> m_BackSprite_;
	Transform m_BackTransform_{};

	// GamePauseUI
	std::unique_ptr<Sprite> m_GamePauseUISprite_;
	Transform m_GamePauseUITransform_{};

	// PlayUI
	std::unique_ptr<Sprite> m_PlayUISprite_;
	Transform m_PlayUITransform_{};

	// ExitUI
	std::unique_ptr<Sprite> m_ExitUISprite_;
	Transform m_ExitUITransform_{};

	// Select
	std::unique_ptr<Sprite> m_SelectSprite_;
	Transform m_SelectTransform_{};
}; 
