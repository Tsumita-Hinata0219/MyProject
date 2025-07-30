#pragma once

#include "Math/MyMath.h"
#include "../Structure/CollisionStructures.h"
#include "../Geometry/GeometryCollision.h"
//using namespace GeometryCollision;


namespace Collision {

	// 球と球の当たり判定
	bool IsCollision(const Sphere& s1, const Sphere& s2);

	// AABBとAABBの当たり判定
	bool IsCollision(const AABB& aabb1, const AABB& aabb2);

	// AABBと球の当たり判定
	bool IsCollision(const AABB& aabb, const Sphere& s);

	// AABBと線の当たり判定
	bool IsCollision(const AABB& aabb, const Segment& s);

	// OBBと球の当たり判定
	bool IsCollision(const OBB& obb, const Sphere& s);

	// OBBと線の当たり判定
	bool IsCollision(const OBB& obb, const Segment& s);

	// OBBとOBBの当たり判定
	bool IsCollision(const OBB& obb1, const OBB& obb2);

	// CapsuleとCapsuleの当たり判定
	bool IsCollision(const Capsule& capsule1, const Capsule& capsule2);
}
