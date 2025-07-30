#include "SphereCollider.h"
#include "System/IObject/IObject.h"


/// <summary>
/// パラメータ付きコンストラクタ
/// </summary>
SphereCollider::SphereCollider(IObject* setOwner)
{
	// オーナーと属性の設定
	owner_ = setOwner;
	// 属性を設定
	attribute_ = {
		owner_->GetCategory(), owner_->GetType()
	};
}


/// <summary>
/// 衝突判定
/// </summary>
bool SphereCollider::Intersects(const ICollider& other)
{
	ColliderDataType type = other.GetData();
	
	// this Sphere x other Sphere
	if (auto sphere = std::get_if<Collider::Sphere>(&type)) {

		return Detect::Intersects(this->data_, *sphere);
	}
	else if (auto aabb = std::get_if<Collider::AABB>(&type)) {

		return Detect::Intersects(*aabb, this->data_);
	}
	return false;
}


/// <summary>
/// ImGuiの描画
/// </summary>
void SphereCollider::DrawImGui(const std::string& label)
{
	ImGui::DragFloat3((label + "_Center").c_str(), &this->data_.center.x, 0.0f);
	ImGui::DragFloat((label + "_Radius").c_str(), &this->data_.radius, 0.0f);
}
