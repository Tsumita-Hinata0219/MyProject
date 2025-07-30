#pragma once

#include "IEntityCollider.h"


namespace Entity {
namespace Collision {

class CollisionAABB  : public IEntityCollider {

public:

	Vector3 m_Min{};
	Vector3 m_Max{};


public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CollisionAABB () = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CollisionAABB () = default;

	/// <summary>
	/// 識別IDのGetter
	/// </summary>
	ColliderTypeID GetTypeID() const override {
		return ColliderTypeID::AABB;
	}

	/// <summary>
	/// 判定処理
	/// </summary>
	bool Intersects(const IEntityCollider& other) const override;

	/// <summary>
	/// ディスパッチ
	/// </summary>
	bool IntersectsWith(const CollisionSphere& other) const override;
	bool IntersectsWith(const CollisionAABB& other) const override;
};
}
}