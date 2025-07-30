#pragma once

#include "../IPostEffect.h"

/* RetroCRTEffectクラス */
class RetroCRTEffect : public IPostEffect<RetroCRTMtl> {

public:

	// コンストラクタ、デストラクタ
	RetroCRTEffect() {};
	~RetroCRTEffect() {};

	// 初期化処理、描画処理
	void Init() override;
	void Draw() override;

	// ImGuiの描画
	void DrawImGui([[maybe_unused]] std::string name = "") override;

protected:

	// コマンドコール
	void CommandCall() override;
};

