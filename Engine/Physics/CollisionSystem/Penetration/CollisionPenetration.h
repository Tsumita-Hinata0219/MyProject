#pragma once

#include "Math/MyMath.h"
#include "../Collider/Structure/ColliderStructure.h"
#include "../Collider/Interface/ICollider.h"



/* 衝突時の押し出し処理 */
namespace Penetration {

	// 押し出し処理
	Vector3 Execute(const ICollider::ColliderDataType& ownerData, const ICollider::ColliderDataType& otherData);

	// Sphere 同士の交差判定と押し出し
	Vector3 HandleSphereSpherePenetration(const Collider::Sphere& ownerSphere, const Collider::Sphere& hitSphere);

	// Sphere と AABB の交差判定と押し出し
	Vector3 HandleSphereAABBPpenetration(const Collider::Sphere& ownerSphere, const Collider::AABB& hitAABB);

	// Sphere と OBB の交差判定と押し出し
	Vector3 HandleSphereOBBPpenetration(const Collider::Sphere& ownerSphere, const Collider::OBB& hitOBB);

	// AABB 同士の交差判定と押し出し
	Vector3 HandleAABBAABBPpenetration(const Collider::AABB& ownerAABB, const Collider::AABB& hitAABB);

	// AABB と Sphere の交差判定と押し出し
	Vector3 HandleAABBSpherePenetration(const Collider::AABB& ownerAABB, const Collider::Sphere& hitSphere);

	// AABB と OBB の交差判定と押し出し
	Vector3 HandleAABBOBBPenetration(const Collider::AABB& ownerAABB, const Collider::OBB& hitOBB);

	// OBB 同士の交差判定と押し出し
	Vector3 HandleOBBOBBPenetration(const Collider::OBB& ownerOBB, const Collider::OBB& hitOBB);

	// OBB と Sphere の交差判定と押し出し
	Vector3 HandleOBBSpherePenetration(const Collider::OBB& ownerOBB, const Collider::Sphere& hitSphere);

	// OBB と AABB の交差判定と押し出し
	Vector3 HandleOBBAABBPpenetration(const Collider::OBB& ownerOBB, const Collider::AABB& hitAABB);

	// Segment と Sphere の交差判定と押し出し
	Vector3 HandleSegmentSpherePenetration(const Collider::Segment& ownerSegment, const Collider::Sphere& hitSphere);

	// Capsule と Sphere の交差判定と押し出し
	Vector3 HandleCapsuleSpherePenetration(const Collider::Capsule& ownerCapsule, const Collider::Sphere& hitSphere);

	// Segment と AABB の交差判定と押し出し
	Vector3 HandleSegmentAABBPpenetration(const Collider::Segment& ownerSegment, const Collider::AABB& hitAABB);

	// Capsule と AABB の交差判定と押し出し
	Vector3 HandleCapsuleAABBPpenetration(const Collider::Capsule& ownerCapsule, const Collider::AABB& hitAABB);

	// Capsule と Capsule の交差判定と押し出し
	Vector3 HandleCapsuleCapsulePenetration(const Collider::Capsule& ownerCapsule, const Collider::Capsule& hitCapsule);

	// Segment と Segment の交差判定と押し出し
	Vector3 HandleSegmentSegmentPenetration(const Collider::Segment& ownerSegment, const Collider::Segment& hitSegment);

}

