#include "IEntityCollider.h"
#include "../Manager/CollisionManager.h"


/// <summary>
/// 登録処理
/// </summary>
void Entity::Collision::IEntityCollider::OnRegister(const std::string& name)
{
	if (!pManager_) {
		pManager_ = CollisionManager::GetInstance();
	}
	pManager_->Register(GetSharedPtr());
	isActive_ = true;
	ownersName_ = name;
}


/// <summary>
/// 衝突時コールバック
/// </summary>
void Entity::Collision::IEntityCollider::OnCollision(const std::string& key)
{
	if (onColFunc_) {
		onColFunc_(key);
	}
}


/// <summary>
/// 関数登録
/// </summary>
void Entity::Collision::IEntityCollider::SetFunction(std::function<void(const std::string&)> func)
{
	onColFunc_ = std::move(func);
}
