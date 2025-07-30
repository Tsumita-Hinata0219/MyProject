#include "CollisionUtilities.h"




// OBBのワールドマトリックス作成
Matrix4x4 ColUtil::CreateOBBWorldMatrix(const Collider::OBB& obb)
{
	Matrix4x4 worldMatrix{};

	for (int i = 0; i < 3; ++i) {
		worldMatrix.m[i][0] = obb.orientations[i].x;
		worldMatrix.m[i][1] = obb.orientations[i].y;
		worldMatrix.m[i][2] = obb.orientations[i].z;
		worldMatrix.m[i][3] = 0.0f;
	}

	worldMatrix.m[3][0] = obb.center.x;
	worldMatrix.m[3][1] = obb.center.y;
	worldMatrix.m[3][2] = obb.center.z;
	worldMatrix.m[3][3] = 1.0f;

	return worldMatrix;
}

// 射影の重複チェック
bool ColUtil::TestAxis(const Vector3& axis, const Collider::OBB& obb1, const Collider::OBB& obb2)
{
	// OBBの射影を計算
	auto projection1 = ColUtil::OBBProjection(obb1, axis);
	auto projection2 = ColUtil::OBBProjection(obb2, axis);

	// 射影が重なっているかチェック
	return projectionOverlap(projection1, projection2);
}

// 実際に重なってるかの計算
bool ColUtil::projectionOverlap(const std::pair<float, float>& projection1, const std::pair<float, float>& projection2)
{

	// 射影が重なっているかチェック
	return projection1.second >= projection2.first && projection2.second >= projection1.first;
}

// 頂点を軸に射影
std::pair<float, float> ColUtil::OBBProjection(const Collider::OBB& obb, const Vector3& axis)
{
	float val = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z); // 正規化
	float newAxis = 0.0f;
	newAxis = newAxis / val;

	// OBB上の頂点を取得
	std::array<Vector3, 8> vertices{};
	for (int i = 0; i < 8; ++i) {
		Vector3 sign = { (i & 1) ? 1.0f : -1.0f, (i & 2) ? 1.0f : -1.0f, (i & 4) ? 1.0f : -1.0f };
		vertices[i] = {
			obb.center.x + obb.orientations[0].x * sign.x * obb.size.x +
						  obb.orientations[1].x * sign.y * obb.size.y +
						  obb.orientations[2].x * sign.z * obb.size.z,
			obb.center.y + obb.orientations[0].y * sign.x * obb.size.x +
						  obb.orientations[1].y * sign.y * obb.size.y +
						  obb.orientations[2].y * sign.z * obb.size.z,
			obb.center.z + obb.orientations[0].z * sign.x * obb.size.x +
						  obb.orientations[1].z * sign.y * obb.size.y +
						  obb.orientations[2].z * sign.z * obb.size.z
		};
	}

	// 頂点を軸に射影
	std::array<float, 8> projections{};
	for (int i = 0; i < 8; ++i) {
		projections[i] = vertices[i].x * axis.x + vertices[i].y * axis.y + vertices[i].z * axis.z;
	}

	auto minmax = std::minmax_element(projections.begin(), projections.end());
	return std::make_pair(*minmax.first, *minmax.second);
}

// OBBの設定
Collider::OBB ColUtil::SettingOBBProperties(Collider::OBB& obb) {
	Collider::OBB result{};

	result.center = obb.center;

	Matrix4x4 rotateMat = MakeRotateXYZMatrix(obb.rotate);
	result.orientations[0].x = rotateMat.m[0][0];
	result.orientations[0].y = rotateMat.m[0][1];
	result.orientations[0].z = rotateMat.m[0][2];
	result.orientations[1].x = rotateMat.m[1][0];
	result.orientations[1].y = rotateMat.m[1][1];
	result.orientations[1].z = rotateMat.m[1][2];
	result.orientations[2].x = rotateMat.m[2][0];
	result.orientations[2].y = rotateMat.m[2][1];
	result.orientations[2].z = rotateMat.m[2][2];

	result.size = obb.size / 2.0f;

	return result;
}

// AABBの設定
Collider::AABB ColUtil::SettingAABBProperties(Collider::AABB& aabb)
{
	Collider::AABB result{};

	result.min = aabb.center - (aabb.size / 2.0f);
	result.max = aabb.center + (aabb.size / 2.0f);

	return result;
}

////// Capsuleの設定
////Col::Sphere ColUtil::SettingCapusulePropertirs() 
////{
////	Col::Sphere result;
////	return result;
////}

// コライダー間の最寄点を計算する関数
Vector3 ColUtil::GetClosestPoint(const Collider::Sphere& sphere, const Vector3& point)
{
	Vector3 direction = point - sphere.center;
	float distance = Length(direction);
	if (distance < sphere.radius) {
		return point;  // コライダー内にある場合、その点が最寄点
	}
	return sphere.center + Normalize(direction) * sphere.radius;  // 半径外なら最寄点
}
Vector3 ColUtil::GetClosestPoint(const Collider::AABB& aabb, const Vector3& point)
{
	float closestX = Clamp(point.x, aabb.min.x, aabb.max.x);
	float closestY = Clamp(point.y, aabb.min.y, aabb.max.y);
	float closestZ = Clamp(point.z, aabb.min.z, aabb.max.z);
	return Vector3(closestX, closestY, closestZ);
}
Vector3 ColUtil::GetClosestPoint(const Collider::OBB& obb, const Vector3& point)
{
	obb, point;
	// OBB内での最寄点を計算するロジック
	// OBBに投影して最寄点を得る方法などを使う
	return Vector3();  // 仮の処理
}
Vector3 ColUtil::GetClosestPoint(const Collider::Segment& segment, const Vector3& point)
{
	Vector3 direction = segment.origin - segment.diff;
	float length = Length(direction);
	direction = Normalize(direction);
	float projection = Dot(point - segment.origin, direction);
	if (projection < 0.0f) return segment.origin;  // 始点より前
	if (projection > length) return segment.diff;  // 終点より後
	return segment.origin + direction * projection;
}
Vector3 ColUtil::GetClosestPoint(const Collider::Capsule& capsule, const Vector3& point)
{
	capsule, point;
	// カプセルの最寄点を計算する方法
	return Vector3();  // 仮の処理
}


// コライダー間の交差時に押し出すためのベクトルを計算する関数
Vector3 ColUtil::GetPenetrationPush(const Collider::Sphere& sphere1, const Collider::Sphere& sphere2)
{
	Vector3 direction = sphere1.center - sphere2.center;
	float distance = Length(direction);
	float overlap = sphere1.radius + sphere2.radius - distance;
	if (overlap > 0.0f) {
		direction = Normalize(direction);
		return direction * overlap;
	}
	return Vector3();
}
Vector3 ColUtil::GetPenetrationPush(const Collider::Sphere& sphere, const Collider::AABB& aabb)
{
	Vector3 closestPoint = GetClosestPoint(aabb, sphere.center);
	Vector3 direction = sphere.center - closestPoint;
	float distance = Length(direction);
	float overlap = sphere.radius - distance;
	if (overlap > 0.0f) {
		direction = Normalize(direction);
		return direction * overlap;
	}
	return Vector3();
}
Vector3 ColUtil::GetPenetrationPush(const Collider::Sphere& sphere, const Collider::OBB& obb)
{
	Vector3 closestPoint = GetClosestPoint(obb, sphere.center);
	Vector3 direction = sphere.center - closestPoint;
	float distance = Length(direction);
	float overlap = sphere.radius - distance;
	if (overlap > 0.0f) {
		direction = Normalize(direction);
		return direction * overlap;
	}
	return Vector3();
}
Vector3 ColUtil::GetPenetrationPush(const Collider::AABB& aabb1, const Collider::AABB& aabb2)
{
	// それぞれの AABB の最小・最大の座標を取得
	Vector3 overlap = Vector3(0.0f, 0.0f, 0.0f);

	// X軸でのオーバーラップチェック
	if (aabb1.max.x > aabb2.min.x && aabb1.min.x < aabb2.max.x) {
		float overlapX = min(aabb1.max.x - aabb2.min.x, aabb2.max.x - aabb1.min.x);
		overlap.x = overlapX;
	}

	// Y軸でのオーバーラップチェック
	if (aabb1.max.y > aabb2.min.y && aabb1.min.y < aabb2.max.y) {
		float overlapY = min(aabb1.max.y - aabb2.min.y, aabb2.max.y - aabb1.min.y);
		overlap.y = overlapY;
	}

	// Z軸でのオーバーラップチェック
	if (aabb1.max.z > aabb2.min.z && aabb1.min.z < aabb2.max.z) {
		float overlapZ = min(aabb1.max.z - aabb2.min.z, aabb2.max.z - aabb1.min.z);
		overlap.z = overlapZ;
	}

	// オーバーラップがない場合、押し出しは必要ない
	if (overlap.x <= 0.0f || overlap.y <= 0.0f || overlap.z <= 0.0f) {
		return Vector3();  // オーバーラップなし
	}

	// 最小のオーバーラップ量を基準に押し出しベクトルを決定
	Vector3 push = Vector3(0.0f, 0.0f, 0.0f);

	// どの軸に沿って押し出すかを決定
	if (overlap.x <= overlap.y && overlap.x <= overlap.z) {
		// X軸での押し出し
		if (aabb1.center.x < aabb2.center.x) {
			push.x = -overlap.x;  // aabb1 が aabb2 の右側にある
		}
		else {
			push.x = overlap.x;   // aabb1 が aabb2 の左側にある
		}
	}
	else if (overlap.y <= overlap.x && overlap.y <= overlap.z) {
		// Y軸での押し出し
		if (aabb1.center.y < aabb2.center.y) {
			push.y = -overlap.y;  // aabb1 が aabb2 の上側にある
		}
		else {
			push.y = overlap.y;   // aabb1 が aabb2 の下側にある
		}
	}
	else {
		// Z軸での押し出し
		if (aabb1.center.z < aabb2.center.z) {
			push.z = -overlap.z;  // aabb1 が aabb2 の手前側にある
		}
		else {
			push.z = overlap.z;   // aabb1 が aabb2 の奥側にある
		}
	}

	return push;
}
Vector3 ColUtil::GetPenetrationPush(const Collider::AABB& aabb, const Collider::Sphere& sphere)
{
	Vector3 closestPoint = GetClosestPoint(aabb, sphere.center);
	Vector3 direction = sphere.center - closestPoint;
	float distance = Length(direction);
	float overlap = sphere.radius - distance;
	if (overlap > 0.0f) {
		direction = Normalize(direction);
		return direction * overlap;
	}
	return Vector3();
}
Vector3 ColUtil::GetPenetrationPush(const Collider::AABB& aabb, const Collider::OBB& obb)
{
	aabb, obb;
	// AABB と OBB の交差処理
	Vector3 push = Vector3();  // 仮の処理
	return push;
}
Vector3 ColUtil::GetPenetrationPush(const Collider::OBB& obb1, const Collider::OBB& obb2)
{
	obb1, obb2;
	// OBB同士の交差処理
	Vector3 push = Vector3();  // 仮の処理
	return push;
}
Vector3 ColUtil::GetPenetrationPush(const Collider::OBB& obb, const Collider::Sphere& sphere)
{
	obb, sphere;
	// OBB と Sphere の交差処理
	Vector3 push = Vector3();  // 仮の処理
	return push;
}
Vector3 ColUtil::GetPenetrationPush(const Collider::OBB& obb, const Collider::AABB& aabb)
{
	obb, aabb;
	// OBB と AABB の交差処理
	Vector3 push = Vector3();  // 仮の処理
	return push;
}
Vector3 ColUtil::GetPenetrationPush(const Collider::Segment& segment, const Collider::Sphere& sphere)
{
	segment, sphere;
	// Segment と Sphere の交差処理
	Vector3 push = Vector3();  // 仮の処理
	return push;
}
Vector3 ColUtil::GetPenetrationPush(const Collider::Capsule& capsule, const Collider::Sphere& sphere)
{
	capsule, sphere;
	// Capsule と Sphere の交差処理
	Vector3 push = Vector3();  // 仮の処理
	return push;
}
Vector3 ColUtil::GetPenetrationPush(const Collider::Segment& segment, const Collider::AABB& aabb)
{
	segment, aabb;
	// Segment と AABB の交差処理
	Vector3 push = Vector3();  // 仮の処理
	return push;
}
Vector3 ColUtil::GetPenetrationPush(const Collider::Capsule& capsule, const Collider::AABB& aabb)
{
	capsule, aabb;
	// Capsule と AABB の交差処理
	Vector3 push = Vector3();  // 仮の処理
	return push;
}
Vector3 ColUtil::GetPenetrationPush(const Collider::Capsule& capsule1, const Collider::Capsule& capsule2)
{
	capsule1, capsule2;
	// Capsule と Capsule の交差処理
	Vector3 push = Vector3();  // 仮の処理
	return push;
}
Vector3 ColUtil::GetPenetrationPush(const Collider::Segment& segment1, const Collider::Segment& segment2)
{
	segment1, segment2;
	// Segment と Segment の交差処理
	Vector3 push = Vector3();  // 仮の処理
	return push;
}

