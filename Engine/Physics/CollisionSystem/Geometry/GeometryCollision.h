#pragma once

#include "Math/MyMath.h"
#include "../Collider/Structure/ColliderStructure.h"

#include <array>


namespace Geometry {

	// OBBのワールドマトリックス作成
	Matrix4x4 CreateOBBWorldMatrix(const OBB& obb);

	// 射影の重複チェック
	bool TestAxis(const Vector3& axis, const OBB& obb1, const OBB& obb2);

	// 実際に重なってるかの計算
	bool projectionOverlap(const std::pair<float, float>& projection1, const std::pair<float, float>& projection2);

	// 頂点を軸に射影
	std::pair<float, float> obbProjection(const OBB& obb, const Vector3& axis);

	//// OBBの設定
	//OBB SettingOBBProperties(OBBCollider* c);

	// AABBの設定
	//Collider::AABB SettingAABBPrope(const Collider::AABB& c);

	// Capsuleの設定
	Capsule SettingCapusulePropertirs();
}