#pragma once

#include "Scene/IScene.h"
#include "System/GameManager/GameManager.h"
#include "JsonManager/JsonManager.h"
#include "Physics/CollisionSystem/Manager/CollisionManager.h"
#include "Utilities/FlagManager/FlagManager.h"

#include "System/Camera/Manager/CameraManager.h"
#include "GameObject/Camera/GameCamera/GameCamera.h"

#include "GameObject/Others/SceneTransition/SceneTransition.h"
#include "GameObject/Others/StartDirection/StartDirection.h"
#include "GameObject/Others/PauseManager/PauseManager.h"

#include "GameObject/Terrain/SkyboxObj/SkyboxObj.h"
#include "GameObject/Terrain/Floor/Floor.h"
#include "GameObject/Terrain/BoxManager/BoxManager.h"

#include "GameObject/Character/Player/Player.h"
#include "GameObject/Character/EnemyManager/EnemyManager.h"

#include "Graphics/PostEffect/IPostEffect/Absent/AbsentEffect.h"
#include "GameObject/Effect/TestPostEffect/TestPostEffect.h"

#include "GameObject/Others/StageTransitionMenu/StageTransitionMenuManager.h"
#include "GameObject/Character/Player/OperationUI/OperationUIManager.h"

#include "GameObject/Dev/DummyParticle/DummyParticle.h"


class GameScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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
	/// シーンチェンジチェック
	/// </summary>
	void SceneChangeCheck();


private: // クラス

	Input* input_ = nullptr;

	// CollisionManager
	CollisionManager* CollisionManager_ = nullptr; // シングルトン

	// FlagManager
	FlagManager* flagManager_ = nullptr; // シングルトン

	// LuaManager
	std::weak_ptr<LuaScript> stageMapLua_;

	// GameCamera
	std::unique_ptr<GameCamera> gameCamera_;

	// StartDirection
	std::unique_ptr<StartDirection> startDirection_;

	// PauseManager
	std::unique_ptr<PauseManager> pauseManager_;

	// Skybox
	std::unique_ptr<SkyboxObj> skybox_;

	// Floor
	std::unique_ptr<Floor> floor_;

	// BoxManager
	std::unique_ptr<BoxManager> boxManager_;

	// Player
	std::unique_ptr<Player> player_;

	// EnemyManager
	std::unique_ptr<EnemyManager> enemyManager_;

	// StageTransitionMenuManager
	std::unique_ptr<StageTransitionMenuManager> STMenuManager_;

	// 操作UI
	std::unique_ptr<OperationUIManager> opUIManager_;

	// シーンチェンジにかかる時間
	Timer sceneChange_;
	// SceneTransition
	SceneTransition* sceneTransition_ = nullptr;

	// RetroCRT
	std::unique_ptr<RetroCRTEffect> retroCRT_;
	RetroCRTMtl retroEffectData_{};

	// DummyParticle
	std::unique_ptr<DummyParticle> dummyParticle_;
};

