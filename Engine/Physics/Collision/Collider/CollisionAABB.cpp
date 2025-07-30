#include "CollisionAABB.h"
#include "../Intersects/CollisionIntersects.h"
using namespace Entity::Collision::Detect;


/// <summary>
/// 判定処理
/// </summary>
bool Entity::Collision::CollisionAABB::Intersects(const IEntityCollider& other) const
{
    return other.Intersects(*this);
}


/// <summary>
/// ディスパッチ
/// </summary>
bool Entity::Collision::CollisionAABB::IntersectsWith(const CollisionSphere& other) const
{
    return Detect::Intersects(*this, other);
}
bool Entity::Collision::CollisionAABB::IntersectsWith(const CollisionAABB& other) const
{
    return Detect::Intersects(*this, other);
}
