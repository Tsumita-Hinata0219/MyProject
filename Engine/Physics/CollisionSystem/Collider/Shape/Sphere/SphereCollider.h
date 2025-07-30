#pragma once

#include "../../Interface/ICollider.h"


/* Sphereコライダークラス */
class SphereCollider : public ICollider {

public:

	/// <summary>
	/// パラメータ付きコンストラクタ
	/// </summary>
	SphereCollider(IObject* setOwner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SphereCollider() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override {};

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
		if (const auto* data = dynamic_cast<const Collider::Sphere*>(&setData)) {
			this->data_ = *data;  // AABB型の場合、値をコピー
		}
	}

#pragma endregion 


	// コライダーのポインターデータ
	Collider::Sphere data_;
};
