#pragma once

#include "../IPostEffect.h"

/* AbsentEffectクラス */
class AbsentEffect : public IPostEffect<AbsentMtl> {

public:

	// コンストラクタ、デストラクタ
	AbsentEffect() {};
	~AbsentEffect() {};

	// 初期化処理、描画処理
	void Init() override;
	void Draw() override;

	// Imguiの描画
	void DrawImGui([[maybe_unused]] std::string name = "") override;

protected:

	// コマンドコール
	void CommandCall() override;
};
