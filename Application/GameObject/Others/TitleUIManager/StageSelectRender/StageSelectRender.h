#pragma once

#include "GameObject/GameObject.h"
#include "System/IObject/IObject.h"


/* ステージセレクト */
class StageSelectRender : public IObject {

public:

	// コンストラク、デストラクタ
	StageSelectRender() = default;
	~StageSelectRender() = default;

	// 初期化処理　更新処理　描画処理
	void Init() override;
	void Update() override;
	void Draw3D() override;
	void Draw2DFront() override;
	void Draw2DBack() override;

	// 衝突判定コールバック関数
	void onCollision([[maybe_unused]] IObject* object) override;

	void UVUpdate(uint32_t num = 1);

private:

	// ImGuiの描画
	void DrawImGui();

private:

	// Input
	Input* input_ = nullptr;

	// モデル
	std::unique_ptr<Model> selectModel_;
	std::unique_ptr<Model> bracketsModel_;

	// トランスフォーム
	Transform selectTrans_{};
	Transform selectUVTrans_{};
	Transform bracketsTrans_{};

	Vector4 color_{};

};
