#pragma once

#include "GameObject/GameObject.h"
#include "System/IObject/IObject.h"

/* タイトルバー */
class GameNameBar : public IObject {

public:

	// コンストラクタ、デストラクタ
	GameNameBar() {};
	~GameNameBar() {};

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

#pragma endregion 


private:

	// モデル
	std::unique_ptr<Model> model_;

	// トランスフォーム
	Transform trans_{};

};

