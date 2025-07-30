#pragma once

#include "Math/MyMath.h"

namespace Collider {

	struct ColliderData {
		virtual ~ColliderData() {

		}
	};

	struct Sphere : public ColliderData {
		Vector3 center{};
		float radius = 0.0f;
	};

	struct AABB : public ColliderData {
		Vector3 center{};
		Vector3 size{};
		Vector3 min{};
		Vector3 max{};
	};

	struct OBB : public ColliderData {
		Vector3 center{};
		Vector3 rotate{};
		Vector3 orientations[3]{};
		Vector3 size{};
	};

	struct Segment : public ColliderData {
		Vector3 origin{};
		Vector3 diff{};
	};
	
	struct Capsule : public ColliderData {
		Vector3 center{};
		Segment segment{};
		Vector3 rotate{};
		float radius = 0.0f;
	};
}