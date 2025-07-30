#pragma once

#include "Scene/IScene.h"
#include "System/GameManager/GameManager.h"
#include "Entity/Manager/GameEntityManager.h"
#include "Physics/Collision/Manager/CollisionManager.h"

#include "GameObject/Terrain/Floor/Floor.h"

#include "GameObject/Dev/DevActor/DevActor.h"


class DevScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DevScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DevScene();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 背景スプライトの描画処理
	/// </summary>
	void BackSpriteDraw() override;

	/// <summary>
	/// ３Dオブジェクトの描画処理
	/// </summary>
	void ModelDraw() override;

	/// <summary>
	/// 前景スプライトの描画処理
	/// </summary>
	void FrontSpriteDraw() override;


private:

	/// <summary>
	/// データのロード
	/// </summary>
	void LoadData();


private:

	// カメラ
	CameraManager* cameraManager_ = nullptr;
	std::weak_ptr<CameraData> cameraData_;

	// EntityManager
	std::unique_ptr<GameEntityManager> gameEntityManager_;

	// CollisionManager
	Entity::Collision::CollisionManager* collisionManager_ = nullptr;

	// Floor
	std::unique_ptr<Floor> floor_;

};

