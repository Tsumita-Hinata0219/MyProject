#pragma once

#include "GameObject/GameObject.h"
#include "System/IObject/IObject.h"

/* タイトルのラベルオブジェクト */
class TitleLabelObject : public IObject {

public:

	// コンストラクタ、デストラクタ
	TitleLabelObject() {};
	~TitleLabelObject() {};

	// パラメータ付きコンストラクタ
	TitleLabelObject(std::pair<std::string, std::string> file);

	// 初期化処理　更新処理　描画処理
	void Init() override;
	void Update() override;
	void Draw3D() override;
	void Draw2DFront() override;
	void Draw2DBack() override;

	// 衝突判定コールバック関数
	void onCollision([[maybe_unused]] IObject* object) override;

	// セレクトされていない時の処理
	void NonSelecting();

	// セレクトされているときの処理
	void OnSelecting();


#pragma region Accessor アクセッサ

	// 座標の設定
	void SetPosition(Vector3 setPos) { this->trans_.srt.translate = setPos; }

	// カラーの設定
	void SetColor(Vector4 setColor) { this->model_->SetMaterialColor(setColor); }

#pragma endregion 


private:

	// ファイルパスとファイル名
	std::pair<std::string, std::string> file_;

	// モデル
	std::unique_ptr<Model> model_;

	// トランスフォーム
	Transform trans_{};

	// セレクトされているときのスケール
	float selectingScale_ = 0.0f;

};
