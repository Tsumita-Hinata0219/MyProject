#pragma once

#include <list>
#include <memory>

#include "System/IObject/IObject.h"
#include "../../../GameObject.h"
#include "../IStageTransitionMenu.h"
#include "Graphics/PostEffect/IPostEffect/RadialBlur/RadialBlurEffect.h"


/* ゲーム終了メニューのブラー */
class StageTransitionMenuBlur : public IStageTransitionMenu {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageTransitionMenuBlur() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageTransitionMenuBlur() = default;

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

	// Blur
	std::unique_ptr<RadialBlurEffect> blur_;
	RadialBlurMtl blurData_;

	// タイマー
	Timer timer_{};
};

