#pragma once

#include "Math/MyMath.h"
#include "../Collider/Structure/ColliderStructure.h"


namespace ColUtil {

	// OBBのワールドマトリックス作成
	Matrix4x4 CreateOBBWorldMatrix(const Collider::OBB& obb);

	// 射影の重複チェック
	bool TestAxis(const Vector3& axis, const Collider::OBB& obb1, const Collider::OBB& obb2);

	// 実際に重なってるかの計算
	bool projectionOverlap(const std::pair<float, float>& projection1, const std::pair<float, float>& projection2);

	// 頂点を軸に射影
	std::pair<float, float> OBBProjection(const Collider::OBB& obb, const Vector3& axis);

	// OBBの設定
	Collider::OBB SettingOBBProperties(Collider::OBB& obb);

	// AABBの設定
	Collider::AABB SettingAABBProperties(Collider::AABB& aabb);

	// Capsuleの設定
	//Collider::Sphere SettingCapusulePropertirs();

	// コライダー間の最寄点を計算する関数
	Vector3 GetClosestPoint(const Collider::Sphere& sphere, const Vector3& point);
	Vector3 GetClosestPoint(const Collider::AABB& aabb, const Vector3& point);
	Vector3 GetClosestPoint(const Collider::OBB& obb, const Vector3& point);
	Vector3 GetClosestPoint(const Collider::Segment& segment, const Vector3& point);
	Vector3 GetClosestPoint(const Collider::Capsule& capsule, const Vector3& point);

	// コライダー間の交差時に押し出すためのベクトルを計算する関数
	Vector3 GetPenetrationPush(const Collider::Sphere& sphere1, const Collider::Sphere& sphere2);
	Vector3 GetPenetrationPush(const Collider::Sphere& sphere, const Collider::AABB& aabb);
	Vector3 GetPenetrationPush(const Collider::Sphere& sphere, const Collider::OBB& obb);
	Vector3 GetPenetrationPush(const Collider::AABB& aabb1, const Collider::AABB& aabb2);
	Vector3 GetPenetrationPush(const Collider::AABB& aabb, const Collider::Sphere& sphere);
	Vector3 GetPenetrationPush(const Collider::AABB& aabb, const Collider::OBB& obb);
	Vector3 GetPenetrationPush(const Collider::OBB& obb1, const Collider::OBB& obb2);
	Vector3 GetPenetrationPush(const Collider::OBB& obb, const Collider::Sphere& sphere);
	Vector3 GetPenetrationPush(const Collider::OBB& obb, const Collider::AABB& aabb);
	Vector3 GetPenetrationPush(const Collider::Segment& segment, const Collider::Sphere& sphere);
	Vector3 GetPenetrationPush(const Collider::Capsule& capsule, const Collider::Sphere& sphere);
	Vector3 GetPenetrationPush(const Collider::Segment& segment, const Collider::AABB& aabb);
	Vector3 GetPenetrationPush(const Collider::Capsule& capsule, const Collider::AABB& aabb);
	Vector3 GetPenetrationPush(const Collider::Capsule& capsule1, const Collider::Capsule& capsule2);
	Vector3 GetPenetrationPush(const Collider::Segment& segment1, const Collider::Segment& segment2);
}
