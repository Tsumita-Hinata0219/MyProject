#pragma once

#include "System/IObject/IObject.h"
#include "../../../GameObject.h"
#include "../IStageTransitionMenu.h"
#include "Graphics/PostEffect/IPostEffect/RadialBlur/RadialBlurEffect.h"


/* ゲーム終了メニューのテキストライン */
class StageTransitionMenuTextLine : public IStageTransitionMenu {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageTransitionMenuTextLine() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageTransitionMenuTextLine() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw2DFront() override;

private:

	/// <summary>
	/// 演出開始
	/// </summary>
	void DirectionStart() override;

	/// <summary>
	/// 演出更新
	/// </summary>
	void DirectionUpdate() override;

	/// <summary>
	/// 演出終了
	/// </summary>
	void DirectionExsit() override;

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui() override;


private:

	// マスク画像
	uint32_t maskTexture_;

	// SpriteDissolve
	SP::DissolveData dissolve_{};

	// タイマー
	Timer timer_{};

	// Alpha値の初期値、目標値
	float initAlpha_ = 0.0f;
	float targetAlpha_ = 1.0f;

	// Threshold値の初期値、目標値
	float initThreshold_ = 1.0f;
	float targetThreshold_ = 0.0f;
};

