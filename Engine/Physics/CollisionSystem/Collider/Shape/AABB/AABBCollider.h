#pragma once

#include "../../Interface/ICollider.h"


/* AABBコライダークラス */
class AABBCollider : public ICollider {

public:

	/// <summary>
	/// パラメータ付きコンストラクタ
	/// </summary>
	AABBCollider(IObject* setOwner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AABBCollider() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突判定
	/// </summary>
	bool Intersects(const ICollider& other) override;

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui(const std::string& label = "") override;


#pragma region Accessor アクセッサ

	// データ
	ColliderDataType GetData() const override { return this->data_; }
	void SetData(const Collider::ColliderData& setData) override {
		if (const auto* data = dynamic_cast<const Collider::AABB*>(&setData)) {
			this->data_ = *data;  // AABB型の場合、値をコピー
		}
	}

#pragma endregion 


	// コライダーのポインターデータ
	Collider::AABB data_;
};
