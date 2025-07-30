#include "GameEntityManager.h"



/// <summary>
/// Entityの追加処理
/// </summary>
void GameEntityManager::Add_NewEntity(std::shared_ptr<IActor> actor)
{
	std::string name = actor->GetName();

	// 名前で検索をかける
	auto it = actorManp_.find(name);
	// 既に存在していたら早期return	
	if (it != actorManp_.end())
	{
		return;
	}

	// Actorの設定
	auto& newActor = actor;
	newActor->SetManager(this); // マネージャーの設定
	newActor->Init();            // 初期化処理

	// Mapに追加
	this->actorManp_[name] = newActor;
}


/// <summary>
/// 更新処理
/// </summary>
void GameEntityManager::Update_Entity()
{
	float deltaTime = 1.0f / 60.0f;

	for(auto& actor : this->actorManp_)
	{
		// Actorの更新
		actor.second->Update(deltaTime);
	}
}


/// <summary>
/// 描画処理
/// </summary>
void GameEntityManager::Render_Entity()
{
	for (auto& actor : this->actorManp_)
	{
		// Actorの描画
		actor.second->Render();
	}
}


/// <summary>
/// クリア
/// </summary>
void GameEntityManager::Clear_Entity()
{
	this->actorManp_.clear();
}


/// <summary>
/// 特定のActorを取得する
/// </summary>
std::weak_ptr<IActor> GameEntityManager::Get_Actor(const std::string& key)
{
	auto it = this->actorManp_.find(key);
	if (it != this->actorManp_.end())
	{
		return it->second;
	}
	return std::weak_ptr<IActor>(); // 見つからなければ空を返す
}


/// <summary>
/// 特定のActorの状態を取得する
/// </summary>
IActor::State GameEntityManager::Get_ActorState(const std::string& key)
{
	auto it = this->actorManp_.find(key);
	if (it != this->actorManp_.end())
	{
		return it->second->GetState();
	}
	return IActor::State::None; // 見つからなければNoneを返す
}


/// <summary>
/// 特定のActorの状態を設定する
/// </summary>
void GameEntityManager::Set_ActorState(const std::string& key, IActor::State state)
{
	auto it = this->actorManp_.find(key);
	if (it != this->actorManp_.end())
	{
		it->second->SetState(state);
	}
	return; // 見つからなければ何もしない
}


/// <summary>
/// 全てのActorの状態を設定する
/// </summary>
void GameEntityManager::Set_AllActorState(IActor::State state)
{
	for (auto& actor : this->actorManp_)
	{
		actor.second->SetState(state);
	}
}


/// <summary>
/// 特定のTransformNodeを取得する
/// </summary>
std::weak_ptr<TransformNode> GameEntityManager::Get_TransformNode(const std::string& key)
{
	auto it = this->actorManp_.find(key);
	if (it != this->actorManp_.end())
	{
		return it->second->GetTransformNode();
	}
	return std::weak_ptr<TransformNode>(); // 見つからなければ空を返す
}
