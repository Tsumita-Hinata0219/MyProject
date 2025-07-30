#pragma once

#include "Scene/IScene.h"
#include "System/GameManager/GameManager.h"
#include "GameObject/GameObject.h"

#include "Graphics/PostEffect/IPostEffect/Absent/AbsentEffect.h"
#include "GameObject/Effect/TestPostEffect/TestPostEffect.h"

#include "Graphics/3D/Skybox/Skybox.h"

#include "GameObject/Others/TitleBackGround/TitleBackGround.h"
#include "GameObject/Others/TitleUIManager/TitleUIManager.h"

#include "GameObject/Others/SceneTransition/SceneTransition.h"


class TitleScene : public IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

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

	// Input
	Input* input_ = nullptr;

	// カメラ
	CameraManager* cameraManager_ = nullptr;
	std::weak_ptr<CameraData> cameraData_;

	// RetroCRT
	std::unique_ptr<RetroCRTEffect> retroCRT_;
	RetroCRTMtl retroEffectData_{};

	// TitleBackGround
	std::unique_ptr<TitleBackGround> titleBG_;

	// TitleUIManager
	std::unique_ptr<TitleUIManager> uiManager_;

	// SceneTransition
	SceneTransition* sceneTransition_ = nullptr;

};

