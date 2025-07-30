#pragma once

#include "../Interface/IBoxObject.h"


/* 壊れるボックス */
class DestructibleBox : public IBoxObject {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DestructibleBox() { attribute_ = { Attributes::Category::TERRAIN, Attributes::Type::SOLID }; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DestructibleBox() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw3D() override;
	void Draw2DFront() override;
	void Draw2DBack() override;
	
	/// <summary>
	/// 衝突判定コールバック関数
	/// </summary>
	void onCollision([[maybe_unused]] IObject* object) override;


#pragma region Accessor アクセッサ

	// SRT
	void SetSRT(const SRTData& setSRT) override { this->trans_.srt = setSRT; }

	// Scale
	void SetScale(const Vector3& setScale) { this->trans_.srt.scale = setScale; }

	// Rotate
	void SetRotate(const Vector3& setRotate) { this->trans_.srt.rotate = setRotate; }

	// Translate
	void SetTranslate(const Vector3& setTranslate) { this->trans_.srt.translate = setTranslate; }

#pragma endregion 


private:

	// モデル
	std::unique_ptr<Model> model_;

	// トランスフォーム
	Transform trans_{};



};

