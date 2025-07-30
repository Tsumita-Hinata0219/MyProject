#pragma once

#include "../Actor/IActor.h"


/* ゲーム内のEntityを一元管理するクラス */
class GameEntityManager {

private:
	
	// EntityのMap
	std::unordered_map<std::string, std::shared_ptr<IActor>> actorManp_;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameEntityManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameEntityManager() = default;

	/// <summary>
	/// Entityの追加処理
	/// </summary>
	void Add_NewEntity(std::shared_ptr<IActor> actor);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update_Entity();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render_Entity();

	/// <summary>
	/// クリア
	/// </summary>
	void Clear_Entity();


#pragma region Accessor

	/// <summary>
	/// 特定のActorを取得する
	/// </summary>
	std::weak_ptr<IActor> Get_Actor(const std::string& key);

	/// <summary>
	/// 特定のActorの状態を取得する
	/// </summary>
	IActor::State Get_ActorState(const std::string& key);

	/// <summary>
	/// 特定のActorの状態を設定する
	/// </summary>
	void Set_ActorState(const std::string& key, IActor::State state);

	/// <summary>
	/// 全てのActorの状態を設定する
	/// </summary>
	void Set_AllActorState(IActor::State state);

	/// <summary>
	/// 特定のTransformNodeを取得する
	/// </summary>
	std::weak_ptr<TransformNode> Get_TransformNode(const std::string& key);

#pragma endregion

};