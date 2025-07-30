#pragma once

#include "GameObject/GameObject.h"
#include "System/IObject/IObject.h"

#include "GameNameBar/GameNameBar.h"
#include "TitleLabelObject/TitleLabelObject.h"
#include "SelectBar/SelectBar.h"
#include "StageSelect/StageSelect.h"
#include "StageSelectRender/StageSelectRender.h"


// Labelの種類
enum class TitleLabelType {
	Start,
	Exit,
	Count,
};


/* TitleUIManagerクラス */
class TitleUIManager {

public:

	// コンストラクタ、デストラクタ
	TitleUIManager() = default;
	~TitleUIManager() = default;

	// 初期化処理　更新処理　描画処理
	void Init();
	void Update();
	void Draw3D();
	void Draw2DFront();
	void Draw2DBack();

	int GetStageSelectNum() const { return stageSelect_->GetSelectNum(); }


private:

	// 何をセレクトしているかによって生じる処理
	void CheckNowSelect();


private:

	// ゲーム名
	std::unique_ptr<GameNameBar> gameName_;

	// ラベル
	std::vector<std::unique_ptr<TitleLabelObject>> labelObjs_;

	// ラベルのBase座標
	Vector3 labelBasePos_{};
	// オフセット
	Vector3 labelOffset_{};
	// スタートラベルの座標
	Vector3 startLabelPos_{};
	// 終了ラベルの座標
	Vector3 exsitLabelPos_{};

	// セレクトバー
	std::unique_ptr<SelectBar> selectBar_;

	// ステージセレクト
	std::unique_ptr<StageSelect> stageSelect_;
	std::unique_ptr<StageSelectRender> stageSelectRender_;

};

