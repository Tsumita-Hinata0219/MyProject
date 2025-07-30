#pragma once

#include "GameObject/GameObject.h"
#include "System/IObject/IObject.h"


/* ステージセレクト */
class StageSelect : public IObject {

public:

	// コンストラク、デストラクタ
	StageSelect() = default;
	~StageSelect() = default;

	// 初期化処理　更新処理　描画処理
	void Init() override;
	void Update() override;
	void Draw3D() override;
	void Draw2DFront() override;
	void Draw2DBack() override;

	// 衝突判定コールバック関数
	void onCollision([[maybe_unused]] IObject* object) override;

	int GetSelectNum() const { return selectNum_; }

private:

	// セレクト操作
	void OperationSelect();

	// ImGuiの描画
	void DrawImGui();

private:

	// Input
	Input* input_ = nullptr;

	// ステージのセレクト番号
	int selectNum_ = 0;

	//Stickの入力を取得
	Vector2 iLStick_{};
	// デッドゾーン
	const float DZone_ = 0.4f;

	// 操作を行ったかのフラグ
	bool isOperationFunc_ = false;

	// 選択したか
	bool isSelect_ = false;



};