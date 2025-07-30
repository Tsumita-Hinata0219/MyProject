#pragma once

#include "../Collider/IEntityCollider.h"
#include "../Collider/CollisionSphere.h"
#include "../Collider/CollisionAABB.h"


namespace Entity {
namespace Collision {
namespace Detect {

/// <summary>
/// Sphere x Sphere
/// </summary>
inline bool Intersects(const CollisionSphere& s1, const CollisionSphere& s2)
{
    // 中心からの距離
    float distance = Length(s2.m_Center - s1.m_Center);
    // 距離を半径を比較
    if(distance <= s1.m_Radius + s2.m_Radius) {
        // 衝突
        return true;
    }
    // 非衝突
    return false;
}


/// <summary>
/// AABB x AABB
/// </summary>
inline bool Intersects(const CollisionAABB& a1, const CollisionAABB& a2)
{
    if ((a1.m_Min.x <= a2.m_Min.x && a1.m_Max.x >= a2.m_Min.x) &&
        (a1.m_Min.y <= a2.m_Max.y && a1.m_Max.y >= a2.m_Min.y) &&
        (a1.m_Min.z <= a2.m_Max.z && a1.m_Max.z >= a2.m_Min.z)
        ) {

        // 衝突
        return true;
    }
    // 非衝突
    return false;
}


/// <summary>
/// Sphere x AABB
/// </summary>
inline bool Intersects(const CollisionSphere& s, const CollisionAABB& a)
{
    // 最近接点
    const Vector3 closestPoint = {
        std::clamp(s.m_Center.x, a.m_Min.x, a.m_Max.x),
        std::clamp(s.m_Center.y, a.m_Min.y, a.m_Max.y),
        std::clamp(s.m_Center.z, a.m_Min.z, a.m_Max.z),
    };
    // 最近接点と球の中心との距離
    float distance = Length(closestPoint - s.m_Center);
    // 距離と半径を比較
    if (distance <= s.m_Radius) {
        // 衝突
        return true;
    }
    // 非衝突
    return false;
}
inline bool Intersects(const CollisionAABB& a, const CollisionSphere& s)
{
    return Intersects(s, a);
}

}
}
}