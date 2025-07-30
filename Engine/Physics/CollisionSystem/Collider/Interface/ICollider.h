#pragma once

#include "../Structure/ColliderStructure.h"
#include "GameObject/Config/AppComfig.h"
#include <variant>
#include <utility>


// 前方宣言
class IObject;
class CollisionManager;


/* コライダーの基底クラス */
class ICollider {

public:

	// variant型で構造体をまとめる
	using ColliderDataType =
		std::variant<
		Collider::Sphere,
		Collider::AABB,
		Collider::OBB,
		Collider::Segment,
		Collider::Capsule>;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ICollider();

	/// <summary>
	/// 仮想デストラク
	/// </summary>
	virtual ~ICollider();

	/// <summary>
	/// コライダー無効化
	/// </summary>
	void Deactivate();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 衝突判定
	/// </summary>
	virtual bool Intersects(const ICollider& other) = 0;

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	virtual void DrawImGui(const std::string& label = "") = 0;


#pragma region Accessor アクセッサ

	// データ
	virtual ColliderDataType GetData() const = 0;
	virtual void SetData(const Collider::ColliderData& setData) = 0;

	// オーナー
	virtual IObject* GetOwner() { return this->owner_; }

	// アクティブ
	virtual bool IsActive() const { return this->isActive_; }
	virtual void SetActive(bool isActive) { this->isActive_ = isActive; }

	// 属性
	Attributes::Category GetCategory() const { return this->attribute_.first; }
	Attributes::Type GetType() const { return this->attribute_.second; }

#pragma endregion 


protected:

	// マネージャ
	CollisionManager* manager_ = nullptr;

	// コライダーを持つオーナー
	IObject* owner_ = nullptr;

	// アクティブフラグ
	bool isActive_ = false;

	// オブジェクトの属性
	std::pair<Attributes::Category, Attributes::Type> attribute_
		= { Attributes::Category::OTHER, Attributes::Type::OTHER };
};

