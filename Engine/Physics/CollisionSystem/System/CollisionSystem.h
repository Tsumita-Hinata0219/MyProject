//#pragma once
//
//#include "Math/MyMath.h"
//
//#include "../Util/CollisionUtilities.h"
//#include "../Detect/CollisionDetect.h"
//#include "../Penetration/CollisionPenetration.h"
//
//#include "../Collider/Interface/ICollider.h"
//#include "../Collider/Shape/Sphere/SphereCollider.h"
//#include "../Collider/Shape/AABB/AABBCollider.h"
//
//
///* 衝突判定システム */
//class CollisionSystem {
//
//private:
//
//
//
//
//
//public:
//
//	/// <summary>
//	/// コンストラクタ
//	/// </summary>
//	CollisionSystem();
//
//	/// <summary>
//	/// デストラクタ
//	/// </summary>
//	~CollisionSystem();
//
//	/// <summary>
//	/// 登録
//	/// </summary>
//	void Register(std::weak_ptr<ICollider> collider);
//
//	/// <summary>
//	/// 特定のColliderの登録解除
//	/// </summary>
//	void UnRegister(uint32_t id);
//
//	/// <summary>
//	/// 更新処理
//	/// </summary>
//	void Update();
//
//	/// <summary>
//	/// クリア
//	/// </summary>
//	void Clear();
//
//
//private:
//
//	/// <summary>
//	/// nullチェック
//	/// </summary>
//	void RemoveNullColliders();
//
//	/// <summary>
//	/// コリジョン判定を行う
//	/// </summary>
//	void CheckCollisions();
//
//	/// <summary>
//	/// 衝突判定回数のカウント
//	/// </summary>
//	void LogCollisionCheck();
//
//	/// <summary>
//	/// ImGuiの描画
//	/// </summary>
//	void DrawImGui();
//
//
//};
