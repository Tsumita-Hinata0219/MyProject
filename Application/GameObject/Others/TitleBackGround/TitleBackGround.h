#pragma once

#include "GameObject/GameObject.h"

// BackGroundの種類
enum class TitleSceneBGType {
	Monochromatic,
	Vignetting,
	Count,
};

/* TitleBackGroundクラス */
class TitleBackGround {

public:

	// コンストラクタ、デストラクタ
	TitleBackGround() = default;
	~TitleBackGround() = default;

	// 初期化、更新、描画
	void Init();
	void Update();
	void Draw2DBack();
	void Draw2DFront();


private:

	// 背景
	std::vector<SpriteProperty> back_;


};

