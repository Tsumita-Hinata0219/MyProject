#include "AABBCollider.h"
#include "System/IObject/IObject.h"
#include "AABBCollider.h"


/// <summary>
/// パラメータ付きコンストラクタ
/// </summary>
AABBCollider::AABBCollider(IObject* setOwner)
{
	// オーナーと属性の設定
	owner_ = setOwner;
	// 属性を設定
	attribute_ = {
		owner_->GetCategory(), owner_->GetType()
	};
}


/// <summary>
/// 更新処理
/// </summary>
void AABBCollider::Update()
{
	// min と max を求める
	data_.min = data_.center - (data_.size / 2.0f);
	data_.max = data_.center + (data_.size / 2.0f);
}


/// <summary>
/// 衝突判定
/// </summary>
bool AABBCollider::Intersects(const ICollider& other)
{
	ColliderDataType type = other.GetData();

	// this AABB x other AABB
	if (auto aabb = std::get_if<Collider::AABB>(&type)) {

		return Detect::Intersects(this->data_, *aabb);
	}
	// this AABB x other Sphere
	else if (auto sphere = std::get_if<Collider::Sphere>(&type)) {

		return Detect::Intersects(this->data_, *sphere);
	}
	return false;
}


/// <summary>
/// ImGuiの描画
/// </summary>
void AABBCollider::DrawImGui(const std::string& label)
{
	ImGui::DragFloat3((label + "_Center").c_str(), &this->data_.center.x, 0.0f);
	ImGui::DragFloat3((label + "_Size").c_str(), &this->data_.size.x, 0.0f);
}

