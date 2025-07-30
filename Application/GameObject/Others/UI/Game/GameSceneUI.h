#pragma once

#include <list>
#include <memory>

#include "System/IObject/IObject.h"
#include "../../../GameObject.h"


// UIの種類
enum class GameSceneUIType {
	ClearTask,
	Count,
};


/* GameScene内のUI */
class GameSceneUI {

public:

	// コンストラクタ、デストラクタ
	GameSceneUI() = default;
	~GameSceneUI() = default;

	// 初期化処理、更新処理、描画処理
	void Init();
	void Update();
	void Draw2DFront();
	void Draw2DBack();


private:

	// UIの描画
	void DrawUI(int type);


private:

	// UI
	std::vector<SpriteProperty> ui_;
};

