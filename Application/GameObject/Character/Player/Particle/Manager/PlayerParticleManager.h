#pragma once

#include "../Interface/IPlayerParticle.h"
#include "../Movement/PlayerMovementParticle.h"


// 前方宣言
class Player;


/* プレイヤー関連のパーティクルマネージャー */
class PlayerParticleManager {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerParticleManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerParticleManager() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


#pragma region Accessor アクセッサ

	// Playerのポインタ
	void SetPlayer(Player* setPlayer) { this->player_ = setPlayer; }

#pragma endregion 


private:

	void MovementParticle();


	/// <summary>
	/// 移動パーティクルの追加
	/// </summary>
	void AddMovementPartiucle1();
	void AddMovementPartiucle2();

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui();


private:

	// Player
	Player* player_ = nullptr;

	// モデルマネージャー
	ModelManager* modelManager_ = nullptr;

	// パーティクルの配列
	std::list<PlayerMovementParticle*> particleList_;

	// オブジェクトプール
	ObjectPool<PlayerMovementParticle> particlePool_;


	// タイマー
	Timer movement1Timer_{};
	Timer movement2Timer_{};
};

