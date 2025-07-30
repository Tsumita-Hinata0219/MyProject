#pragma once

#include "Scene/IScene.h"
#include "System/GameManager/GameManager.h"
#include "GameObject/GameObject.h"
#include "Graphics/3D/Model/Manager/ModelManager.h"
#include "Graphics/PipeLineManager/PipeLineManager.h"
#include "Utilities/ConfigManager/ConfigManager.h"

#include "System/Camera/Manager/CameraManager.h"

#include "GameObject/Terrain/Skydome/Skydome.h"
#include "Graphics/3D/Skybox/Skybox.h"
#include "GameObject/Effect/TestPostEffect/TestPostEffect.h"

#include "GameObject/Terrain/Floor/Floor.h"
#include "GameObject/Dev/DummyParticle/DummyParticle.h"


class LuaManager;

class DebugScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DebugScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DebugScene();

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

	// カメラ
	CameraManager* cameraManager_ = nullptr;
	std::weak_ptr<CameraData> cameraData_;

	// LuaManager
	LuaManager* luaManager_ = nullptr;

	// DummyParticle
	std::unique_ptr<DummyParticle> dummyParticle_;

	// Floor
	std::unique_ptr<Floor> floor_;
};

