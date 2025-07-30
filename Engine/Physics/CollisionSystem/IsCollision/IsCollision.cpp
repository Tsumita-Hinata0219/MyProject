#include "IsCollision.h"



namespace Collision {

	// 球と球の当たり判定
	bool IsCollision(const Sphere& s1, const Sphere& s2) {

		// 中心からの距離
		float distance = Length(s2.center - s1.center);

		// 距離と半径を比べる
		if (distance <= s1.radius + s2.radius) {

			// 当たってる
			return true;
		}
		// 当たってない
		return false;
	}


	// AABBとAABBの当たり判定
	bool IsCollision(const AABB& aabb1, const AABB& aabb2) {

		if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
			(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
			(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)
			) {

			// 当たっている
			return true;
		}
		// 当たってない
		return false;
	}


	// AABBと球の当たり判定
	bool IsCollision(const AABB& aabb, const Sphere& s) {

		// 最近接点を求める
		const Vector3 ClosestPoint = {
			std::clamp(s.center.x, aabb.min.x, aabb.max.x),
			std::clamp(s.center.y, aabb.min.y, aabb.max.y),
			std::clamp(s.center.z, aabb.min.z, aabb.max.z), };

		// 最近接点と球の中心と距離を求める
		float dist = Length(ClosestPoint - s.center);

		// 距離が半径よりも小さければ衝突
		if (dist <= s.radius) {

			// 当たってる
			return true;
		}
		else {
			// 当たってない
			return false;
		}
	}


	// AABBと線の当たり判定
	bool IsCollision(const AABB& aabb, const Segment& s) {

		Vector3 tMin = {
			(aabb.min.x - s.origin.x) / s.diff.x,
			(aabb.min.y - s.origin.y) / s.diff.y,
			(aabb.min.z - s.origin.z) / s.diff.z};
		Vector3 tMax = {
			(aabb.max.x - s.origin.x) / s.diff.x,
			(aabb.max.y - s.origin.y) / s.diff.y,
			(aabb.max.z - s.origin.z) / s.diff.z};


		if (std::isnan(tMin.x) || std::isnan(tMax.x) ||
			std::isnan(tMin.y) || std::isnan(tMax.y) ||
			std::isnan(tMin.z) || std::isnan(tMax.z)) {
			return true;
		}


		Vector3 tNear = {
			min(tMin.x, tMax.x),
			min(tMin.y, tMax.y),
			min(tMin.z, tMax.z)};
		Vector3 tFar = {
			max(tMin.x, tMax.x),
			max(tMin.y, tMax.y),
			max(tMin.z, tMax.z)};

		// AABBとの衝突点(貫通点)のtが小さいほう
		float ntMin = max(max(tNear.x, tNear.y), tNear.z);
		// AABBとの衝突点(貫通点)のtが大きいほう
		float ntMax = min(min(tFar.x, tFar.y), tFar.z);


		if (ntMin < 0.0f && ntMax < 0.0f || ntMin > 1.0f && ntMax > 1.0f) {
			return false;
		}

		if (ntMin < ntMax) {

			// 当たってる
			return true;
		}
		else {

			// 当たってない
			return false;
		}
	}


	// OBBと球の当たり判定
	bool IsCollision(const OBB& obb, const Sphere& s) {

		Vector3 centerInOBBLocalSpace = {
		TransformByMatrix(
			s.center, Inverse(Geometry::CreateOBBWorldMatrix(obb))) };

		AABB abbOBBLocal = {
			.min = { -obb.halfSize.x, -obb.halfSize.y, -obb.halfSize.z },
			.max = { obb.halfSize.x, obb.halfSize.y, obb.halfSize.z }
		};
		Sphere sphereOBBLocal = {
			centerInOBBLocalSpace,
			s.radius
		};

		// ローカル座標で衝突判定
		if (Collision::IsCollision(abbOBBLocal, sphereOBBLocal)) {

			// 当たってる
			return true;
		}
		else {

			// 当たってない
			return false;
		}

	}


	// OBBと線の当たり判定
	bool IsCollision(const OBB& obb, const Segment& s) {

		Matrix4x4 obbInverse = Inverse(Geometry::CreateOBBWorldMatrix(obb));

		AABB aabbOBBLocal = {
			.min = { -obb.halfSize.x, -obb.halfSize.y, -obb.halfSize.z },
			.max = { obb.halfSize.x, obb.halfSize.y, obb.halfSize.z }
		};


		Vector3 localOrigin = TransformByMatrix(s.origin, obbInverse);
		Vector3 LocalEnd = TransformByMatrix(s.origin +s.diff, obbInverse);

		Segment localSegment = {
			.origin = localOrigin,
			.diff = LocalEnd -localOrigin,
		};


		// AABBとSegmentの当たり判定を使う
		if (IsCollision(aabbOBBLocal, localSegment)) {

			// 当たってる
			return true;
		}
		else {

			// 当たってない
			return false;
		}
	}


	// OBBとOBBの当たり判定
	bool IsCollision(const OBB& obb1, const OBB& obb2) {

		// 分離軸テスト
		for (const auto& axis : obb1.orientations) {
			if (!Geometry::TestAxis(axis, obb1, obb2)) {
				return false;
			}
		}

		for (const auto& axis : obb2.orientations) {
			if (!Geometry::TestAxis(axis, obb1, obb2)) {
				return false;
			}
		}

		// OBB1の軸とOBB2の軸に垂直な軸をテスト
		for (const auto& axis : {
				Vector3{obb1.orientations[1].x * obb2.orientations[2].x - obb1.orientations[2].x * obb2.orientations[1].x,
						obb1.orientations[1].y * obb2.orientations[2].y - obb1.orientations[2].y * obb2.orientations[1].y,
						obb1.orientations[1].z * obb2.orientations[2].z - obb1.orientations[2].z * obb2.orientations[1].z},
				Vector3{obb1.orientations[2].x * obb2.orientations[0].x - obb1.orientations[0].x * obb2.orientations[2].x,
						obb1.orientations[2].y * obb2.orientations[0].y - obb1.orientations[0].y * obb2.orientations[2].y,
						obb1.orientations[2].z * obb2.orientations[0].z - obb1.orientations[0].z * obb2.orientations[2].z},
				Vector3{obb1.orientations[0].x * obb2.orientations[1].x - obb1.orientations[1].x * obb2.orientations[0].x,
						obb1.orientations[0].y * obb2.orientations[1].y - obb1.orientations[1].y * obb2.orientations[0].y,
						obb1.orientations[0].z * obb2.orientations[1].z - obb1.orientations[1].z * obb2.orientations[0].z} }) {
			if (!Geometry::TestAxis(axis, obb1, obb2)) {
				return false;
			}
		}

		return true;
	}


	// CapsuleとCapsuleの当たり判定
	bool IsCollision(const Capsule& capsule1, const Capsule& capsule2) {

		//オイラー角から回転行列を作成
		Matrix4x4 rotateMat1 = MakeRotateXYZMatrix(capsule1.rotate);
		Matrix4x4 rotateMat2 = MakeRotateXYZMatrix(capsule2.rotate);

		// ベクトル変換
		Vector3 v1 = TransformByMatrix((capsule1.s.origin + capsule1.s.diff) - capsule1.s.origin, rotateMat1);
		Vector3 v2 = TransformByMatrix((capsule2.s.origin + capsule2.s.diff) - capsule2.s.origin, rotateMat2);
		Vector3 startV = TransformByMatrix((capsule2.s.origin - capsule1.s.origin), rotateMat1);

		// 内積
		float dotProduct = Dot(v1, v2);

		// 長さ
		float length1 = Length(v1);
		float length2 = Length(v2);

		// 正規化
		Vector3 normalize1 = Normalize(v1);
		Vector3 normalize2 = Normalize(v2);

		// 距離 & 半径
		Vector3 val = { Dot(startV, normalize1), Dot(startV, normalize2), 0.0f };
		float dist = Length(startV - val);
		float rad = capsule1.radius + capsule2.radius;

		// 衝突しているかの判定
		if (dist <= rad && dotProduct >= 0.0f && dotProduct <= length1 + length2) {

			// 衝突している
			return true;
		}

		// 衝突していない
		return false;
	}
}