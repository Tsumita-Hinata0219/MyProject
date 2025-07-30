#pragma once

#include "Math/MyMath.h"
#include "Math/Struct.h"

#include "Utilities/Logger/Logger.h"

#include "../Detect/CollisionDetect.h"
#include "../Penetration/CollisionPenetration.h"

#include "../Collider/Interface/ICollider.h"
#include "../Collider/Shape/Sphere/SphereCollider.h"
#include "../Collider/Shape/AABB/AABBCollider.h"

#include <list>
#include <vector>

// 前方宣言
class IObject;


// ファクトリ関数
using ColliderFactory = std::function<std::unique_ptr<ICollider>(IObject*)>;


/* CollisionManagerクラス */
class CollisionManager {

private: // シングルトン

	CollisionManager() = default;
	~CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	const CollisionManager& operator=(const CollisionManager&) = delete;


public:

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static CollisionManager* GetInstance() {
		static CollisionManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// コライダー登録
	/// </summary>
	void Register(ICollider* pCollider);

	/// <summary>
	/// 登録されているShapeを削除する
	/// </summary>
	void UnRegister(ICollider* pCollider);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 配列のクリア
	/// </summary>
	void Clear();


#pragma region Accessor アクセッサ


#pragma endregion 


private:

	/// <summary>
	/// nullチェック
	/// </summary>
	void RemoveNullColliders();

	/// <summary>
	/// コリジョン判定を行う
	/// </summary>
	void CheckCollisions();

	/// <summary>
	/// 衝突判定回数のカウント
	/// </summary>
	void LogCollisionCheck();

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui();

	/// <summary>
	/// Colliderのインスタンスの取得
	/// </summary>
	static const std::unordered_map<std::type_index, ColliderFactory>& GetColliderFactoryMap();


private:

	// コライダーポインタリスト
	std::list<ICollider*> pColliders_;

	int collisionCheckCount_ = 0;        // 衝突判定の回数カウンター
	int totalCollisionChecks_ = 0;      // 全体の衝突判定の総回数
};
