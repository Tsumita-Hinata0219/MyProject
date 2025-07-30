#include "CollisionPenetration.h"
#include "../Util/CollisionUtilities.h"

#include <functional>
#include <typeindex>
#include <variant>



// 押し出し処理
Vector3 Penetration::Execute(const ICollider::ColliderDataType& ownerData, const ICollider::ColliderDataType& otherData)
{
	// 結果を格納する変数
	Vector3 result;

	// std::visit を使って、二つのコライダーの型に応じた処理を実行
	result = std::visit([&](auto&& ownerCollider, auto&& hitCollider_) -> Vector3 {
		using T1 = std::decay_t<decltype(ownerCollider)>;
		using T2 = std::decay_t<decltype(hitCollider_)>;

		if constexpr (std::is_same_v<T1, Collider::Sphere> && std::is_same_v<T2, Collider::Sphere>) {
			// Sphere と Sphere の場合の処理
			return Penetration::HandleSphereSpherePenetration(ownerCollider, hitCollider_);
		}
		else if constexpr (std::is_same_v<T1, Collider::Sphere> && std::is_same_v<T2, Collider::AABB>) {
			// Sphere と AABB の場合の処理
			return Penetration::HandleSphereAABBPpenetration(ownerCollider, hitCollider_);
		}
		else if constexpr (std::is_same_v<T1, Collider::Sphere> && std::is_same_v<T2, Collider::OBB>) {
			// Sphere と OBB の場合の処理
			return Penetration::HandleSphereOBBPpenetration(ownerCollider, hitCollider_);
		}
		else if constexpr (std::is_same_v<T1, Collider::AABB> && std::is_same_v<T2, Collider::AABB>) {
			// AABB と AABB の場合の処理
			return Penetration::HandleAABBAABBPpenetration(ownerCollider, hitCollider_);
		}
		else if constexpr (std::is_same_v<T1, Collider::AABB> && std::is_same_v<T2, Collider::Sphere>) {
			// AABB と Sphere の場合の処理
			return Penetration::HandleAABBSpherePenetration(ownerCollider, hitCollider_);
		}
		else if constexpr (std::is_same_v<T1, Collider::AABB> && std::is_same_v<T2, Collider::OBB>) {
			// AABB と OBB の場合の処理
			return Penetration::HandleAABBOBBPenetration(ownerCollider, hitCollider_);
		}
		else if constexpr (std::is_same_v<T1, Collider::OBB> && std::is_same_v<T2, Collider::Sphere>) {
			// OBB と Sphere の場合の処理
			return Penetration::HandleOBBSpherePenetration(ownerCollider, hitCollider_);
		}
		else if constexpr (std::is_same_v<T1, Collider::OBB> && std::is_same_v<T2, Collider::AABB>) {
			// OBB と AABB の場合の処理
			return Penetration::HandleOBBAABBPpenetration(ownerCollider, hitCollider_);
		}
		else if constexpr (std::is_same_v<T1, Collider::OBB> && std::is_same_v<T2, Collider::OBB>) {
			// OBB と OBB の場合の処理
			return Penetration::HandleOBBOBBPenetration(ownerCollider, hitCollider_);
		}
		else {
			// 他の組み合わせがない場合のデフォルト
			return Vector3::zero;
		}
		}, ownerData, otherData);

	return result; // 結果を返す
}


// Sphere 同士の交差判定と押し出し
Vector3 Penetration::HandleSphereSpherePenetration(const Collider::Sphere& ownerSphere, const Collider::Sphere& hitSphere)
{
	Vector3 direction = ownerSphere.center - hitSphere.center;
	float distance = Length(direction);
	float overlap = ownerSphere.radius + hitSphere.radius - distance;

	if (overlap > 0.0f) {
		direction = Normalize(direction);
		return direction * overlap;  // 押し出し
	}
	return Vector3();
}


// Sphere と AABB の交差判定と押し出し
Vector3 Penetration::HandleSphereAABBPpenetration(const Collider::Sphere& ownerSphere, const Collider::AABB& hitAABB)
{
	Vector3 closestPoint = ColUtil::GetClosestPoint(hitAABB, ownerSphere.center);
	Vector3 direction = ownerSphere.center - closestPoint;
	float distance = Length(direction);
	float overlap = ownerSphere.radius - distance;

	if (overlap > 0.0f) {
		direction = Normalize(direction);
		return direction * overlap;  // 押し出し
	}
	return Vector3();
}


// Sphere と OBB の交差判定と押し出し
Vector3 Penetration::HandleSphereOBBPpenetration(const Collider::Sphere& ownerSphere, const Collider::OBB& hitOBB)
{
	Vector3 closestPoint = ColUtil::GetClosestPoint(hitOBB, ownerSphere.center);
	Vector3 direction = ownerSphere.center - closestPoint;
	float distance = Length(direction);
	float overlap = ownerSphere.radius - distance;

	if (overlap > 0.0f) {
		direction = Normalize(direction);
		return direction * overlap;  // 押し出し
	}
	return Vector3();
}


// AABB 同士の交差判定と押し出し
Vector3 Penetration::HandleAABBAABBPpenetration(const Collider::AABB& ownerAABB, const Collider::AABB& hitAABB)
{
	// AABB同士の押し出し処理（例：最小のオーバーラップを計算し、押し出す）
	Vector3 push = ColUtil::GetPenetrationPush(ownerAABB, hitAABB);
	return push;
}


// AABB と Sphere の交差判定と押し出し
Vector3 Penetration::HandleAABBSpherePenetration(const Collider::AABB& ownerAABB, const Collider::Sphere& hitSphere)
{
	Vector3 closestPoint = ColUtil::GetPenetrationPush(ownerAABB, hitSphere);
	Vector3 direction = hitSphere.center - closestPoint;
	float distance = Length(direction);
	float overlap = hitSphere.radius - distance;

	if (overlap > 0.0f) {
		direction = Normalize(direction);
		return direction * overlap;  // 押し出し
	}
	return Vector3();
}


// AABB と OBB の交差判定と押し出し
Vector3 Penetration::HandleAABBOBBPenetration(const Collider::AABB& ownerAABB, const Collider::OBB& hitOBB)
{
	// AABB と OBB の交差判定処理
	Vector3 push = ColUtil::GetPenetrationPush(ownerAABB, hitOBB);
	return push;
}


// OBB 同士の交差判定と押し出し
Vector3 Penetration::HandleOBBOBBPenetration(const Collider::OBB& ownerOBB, const Collider::OBB& hitOBB)
{
	// OBB同士の交差判定処理
	Vector3 push = ColUtil::GetPenetrationPush(ownerOBB, hitOBB);
	return push;
}


// OBB と Sphere の交差判定と押し出し
Vector3 Penetration::HandleOBBSpherePenetration(const Collider::OBB& ownerOBB, const Collider::Sphere& hitSphere)
{
	// OBB と Sphere の交差判定処理
	Vector3 closestPoint = ColUtil::GetPenetrationPush(ownerOBB, hitSphere);
	Vector3 direction = hitSphere.center - closestPoint;
	float distance = Length(direction);
	float overlap = hitSphere.radius - distance;

	if (overlap > 0.0f) {
		direction = Normalize(direction);
		return direction * overlap;  // 押し出し
	}
	return Vector3();
}


// OBB と AABB の交差判定と押し出し
Vector3 Penetration::HandleOBBAABBPpenetration(const Collider::OBB& ownerOBB, const Collider::AABB& hitAABB)
{
	// OBB と AABB の交差判定処理
	Vector3 push = ColUtil::GetPenetrationPush(ownerOBB, hitAABB);
	return push;
}


// Segment と Sphere の交差判定と押し出し
Vector3 Penetration::HandleSegmentSpherePenetration(const Collider::Segment& ownerSegment, const Collider::Sphere& hitSphere)
{
	// Segment と Sphere の交差判定処理
	Vector3 push = ColUtil::GetPenetrationPush(ownerSegment, hitSphere);
	return push;
}


// Capsule と Sphere の交差判定と押し出し
Vector3 Penetration::HandleCapsuleSpherePenetration(const Collider::Capsule& ownerCapsule, const Collider::Sphere& hitSphere)
{
	// Capsule と Sphere の交差判定処理
	Vector3 push = ColUtil::GetPenetrationPush(ownerCapsule, hitSphere);
	return push;
}


// Segment と AABB の交差判定と押し出し
Vector3 Penetration::HandleSegmentAABBPpenetration(const Collider::Segment& ownerSegment, const Collider::AABB& hitAABB)
{
	// Segment と AABB の交差判定処理
	Vector3 push = ColUtil::GetPenetrationPush(ownerSegment, hitAABB);
	return push;
}


// Capsule と AABB の交差判定と押し出し
Vector3 Penetration::HandleCapsuleAABBPpenetration(const Collider::Capsule& ownerCapsule, const Collider::AABB& hitAABB)
{
	// Capsule と AABB の交差判定処理
	Vector3 push = ColUtil::GetPenetrationPush(ownerCapsule, hitAABB);
	return push;
}


// Capsule と Capsule の交差判定と押し出し
Vector3 Penetration::HandleCapsuleCapsulePenetration(const Collider::Capsule& ownerCapsule, const Collider::Capsule& hitCapsule)
{
	// Capsule と Capsule の交差判定処理
	Vector3 push = ColUtil::GetPenetrationPush(ownerCapsule, hitCapsule);
	return push;
}


// Segment と Segment の交差判定と押し出し
Vector3 Penetration::HandleSegmentSegmentPenetration(const Collider::Segment& ownerSegment, const Collider::Segment& hitSegment)
{
	// Segment と Segment の交差判定処理
	Vector3 push = ColUtil::GetPenetrationPush(ownerSegment, hitSegment);
	return push;
}
