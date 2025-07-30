#pragma once

#include "../IPostEffect.h"

/* SepiaToneEffectクラス */
class SepiaToneEffect : public IPostEffect<SepiaToneMtl> {

public:

	// コンストラクタ、デストラクタ
	SepiaToneEffect() {};
	~SepiaToneEffect() {};

	// 初期化処理、描画処理
	void Init() override;
	void Draw() override;

	// Imguiの描画
	void DrawImGui([[maybe_unused]] std::string name = "") override;

protected:

	// コマンドコール
	void CommandCall() override;
};


