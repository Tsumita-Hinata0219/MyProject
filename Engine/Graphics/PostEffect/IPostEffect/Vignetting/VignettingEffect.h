#pragma once

#include "../IPostEffect.h"

/* VignettingEffectクラス */
class VignettingEffect : public IPostEffect<VignettingMtl> {

public:

	// コンストラクタ、デストラクタ
	VignettingEffect() {};
	~VignettingEffect() {};

	// 初期化処理、描画処理
	void Init() override;
	void Draw() override;

	// Imguiの描画
	void DrawImGui([[maybe_unused]] std::string name = "") override;

protected:

	// コマンドコール
	void CommandCall() override;
};


