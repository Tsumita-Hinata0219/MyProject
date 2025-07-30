#include "CollisionManager.h"
#include "System/IObject/IObject.h"


/// <summary>
/// ファクトリマップを作る関数
/// </summary>
static std::unordered_map<std::type_index, ColliderFactory> CreateColliderFactoryMap() {
	return {
		{ typeid(Collider::Sphere), [](IObject* owner) {
			return std::make_unique<SphereCollider>(owner); }
		},

			/* 他の型も同様に追加 */
	};
}


/// <summary>
/// 初期化処理
/// </summary>
void CollisionManager::Init()
{
	Logger::GetInstance().Init("CollisionLog"); // ログファイルの初期化
	
}


/// <summary>
/// コライダー登録
/// </summary>
void CollisionManager::Register(ICollider* pCollider)
{
	pColliders_.push_back(pCollider);
}


/// <summary>
/// 登録されているShapeを削除する
/// </summary>
void CollisionManager::UnRegister(ICollider* pCollider)
{
	// nullptrか指定されたポインタがリストに存在するか確認
	auto it = std::find(pColliders_.begin(), pColliders_.end(), pCollider);

	if (it != pColliders_.end()) {
		// 存在する場合はその要素を削除
		pColliders_.erase(it);
	}
}


/// <summary>
/// 更新処理
/// </summary>
void CollisionManager::Update()
{

	// コリジョン判定を行う
	CheckCollisions();

#ifdef _DEBUG
	DrawImGui(); // Debug表示
#endif // _DEBUG
}


/// <summary>
/// 配列のクリア
/// </summary>
void CollisionManager::Clear()
{
	// コライダーポインタリストの中身をクリア
	pColliders_.clear();  // リスト内の要素を削除
}


/// <summary>
/// nullチェック
/// </summary>
void CollisionManager::RemoveNullColliders()
{
	pColliders_.remove_if([](ICollider* collider) {
		return collider == nullptr;  // ポインタがnullptrなら削除
		}
	);
}


/// <summary>
/// コリジョン判定を行う
/// </summary>
void CollisionManager::CheckCollisions()
{
	for (auto itr1 = pColliders_.begin(); itr1 != pColliders_.end(); ++itr1) {
		for (auto itr2 = std::next(itr1); itr2 != pColliders_.end(); ++itr2) {

			// itr1とit2の属性が同じなら判定をスキップする
			if ((*itr1)->GetCategory() == (*itr2)->GetCategory()) {
				continue;
			}
			// どちらかが非アクティブなら判定をスキップする
			if (!(*itr1)->IsActive() || !(*itr2)->IsActive()) {
				continue;
			}

			// 衝突判定を実行
			totalCollisionChecks_++;
			collisionCheckCount_++;

			// 衝突判定をとる
			if ((*itr1)->Intersects((**itr2))) {

				// オーナーに衝突相手のコライダーデータを送る
				(*itr1)->GetOwner()->SetHitCollider((*itr2)->GetData());
				(*itr2)->GetOwner()->SetHitCollider((*itr1)->GetData());
				// オーナーのonCollision関数を呼び出して、衝突相手のownerを送る
				(*itr1)->GetOwner()->onCollision((*itr2)->GetOwner());
				(*itr2)->GetOwner()->onCollision((*itr1)->GetOwner());
			}
		}
	}

	LogCollisionCheck(); // 衝突判定回数を記録する
}


/// <summary>
/// 衝突判定回数のカウント
/// </summary>
void CollisionManager::LogCollisionCheck()
{
	if (collisionCheckCount_ >= 10) {
		// ログに記録
		Logger::GetInstance().Log("CollisionLog",
			"Collision checks executed : " + std::to_string(totalCollisionChecks_));

		// カウントをリセット
		collisionCheckCount_ = 0;
	}

	totalCollisionChecks_ = 0;
}


/// <summary>
/// ImGuiの描画
/// </summary>
void CollisionManager::DrawImGui()
{
	if (ImGui::TreeNode("CollisionManager")) {

		ImGui::Text("Collider Instance Count = %d", pColliders_.size());

		for (auto& element : pColliders_) {
			element->DrawImGui();
		}

		ImGui::TreePop();
	}
}


/// <summary>
/// Colliderのインスタンスの取得
/// </summary>
const std::unordered_map<std::type_index, ColliderFactory>& CollisionManager::GetColliderFactoryMap()
{
	static const std::unordered_map<std::type_index, ColliderFactory> factoryMap =
		CreateColliderFactoryMap();
	return factoryMap;
}