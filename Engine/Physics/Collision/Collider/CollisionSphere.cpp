#include "CollisionSphere.h"
#include "../Intersects/CollisionIntersects.h"
using namespace Entity::Collision::Detect;


/// <summary>
/// 判定処理
/// </summary>
bool Entity::Collision::CollisionSphere::Intersects(const IEntityCollider& other) const 
{
	return other.IntersectsWith(*this);
}


/// <summary>
/// ディスパッチ
/// </summary>
bool Entity::Collision::CollisionSphere::IntersectsWith(const CollisionSphere& other) const
{
	return Detect::Intersects(*this, other);
}
bool Entity::Collision::CollisionSphere::IntersectsWith(const CollisionAABB& other) const
{
	return Detect::Intersects(*this, other);
}
