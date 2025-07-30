#pragma once

#include "../IPostEffect.h"

/* RandomEffectクラス */
class RandomEffect : public IPostEffect<RandomMtl> {

public:

	// コンストラクタ、デストラクタ
	RandomEffect() {};
	~RandomEffect() {};

	// 初期化処理、描画処理
	void Init() override;
	void Draw() override;

	// Imguiの描画
	void DrawImGui([[maybe_unused]] std::string name = "") override;

protected:

	// コマンドコール
	void CommandCall() override;
};