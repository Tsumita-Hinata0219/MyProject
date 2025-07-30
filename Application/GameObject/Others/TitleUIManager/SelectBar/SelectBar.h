#pragma once

#include "GameObject/GameObject.h"
#include "System/IObject/IObject.h"


enum class SelectOption {
	Start,
	Exit,
	Count,
};


/* タイトルバー */
class SelectBar : public IObject {

public:

	// コンストラクタ、デストラクタ
	SelectBar() = default;
	~SelectBar() = default;

	// 初期化処理　更新処理　描画処理
	void Init() override;
	void Update() override;
	void Draw3D() override;
	void Draw2DFront() override;
	void Draw2DBack() override;

	// 衝突判定コールバック関数
	void onCollision([[maybe_unused]] IObject* object) override;


#pragma region Accessor アクセッサ

	// 座標の設定
	void SetPosition(Vector3 setPos) { this->trans_.srt.translate = setPos; }

	// ラベルの座標の設定
	void SaveLabelPosition(Vector3 setPos) { this->labelPos_.push_back(setPos); }

	// 選択中のオプションを返す
	SelectOption GetNowSelectOption() const { return this->nowOption_; }

#pragma endregion 


private:

	// 操作
	void Operation();

	// 次のオプションへ
	void NextOption();

	// 前のオプションへ
	void PreOption();

	// 座標の変更
	void ChangeSelectBarPos();


private:

	// Input
	Input* input_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_;

	// トランスフォーム
	Transform trans_{};

	// 現在選択されているオプション
	SelectOption nowOption_ = SelectOption::Start;

	// 選択肢の座標
	std::vector<Vector3> labelPos_;

};

