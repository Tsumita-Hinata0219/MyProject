#pragma once

#include "IEntityCollider.h"


namespace Entity {
namespace Collision {

class CollisionSphere  : public IEntityCollider {

public:

	Vector3 m_Center{};
	float m_Radius = 0.0f;


public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CollisionSphere () = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CollisionSphere () = default;

	/// <summary>
	/// 識別IDのGetter
	/// </summary>
	ColliderTypeID GetTypeID() const override {
		return ColliderTypeID::Sphere;	
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