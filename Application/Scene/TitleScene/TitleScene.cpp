#include "TitleScene.h"
#include "System/GameManager/GameManager.h"



/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene() {}


/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene() {}


/// <summary>
/// 初期化処理
/// </summary>
void TitleScene::Initialize()
{
	/* ----- Input インプット ----- */
	input_ = Input::GetInstance();

	/* ----- Camera カメラ ----- */
	cameraManager_ = CameraManager::GetInstance();
	cameraManager_->ReSet();
	cameraData_ = cameraManager_->GetCameraDataWeak();
	cameraData_.lock()->Init({ 0.15f, 0.0f, 0.0f }, { 0.0f, 3.0f, -5.0f });


	/* ----- RetroCRT レトロエフェクト ----- */
	retroCRT_ = std::make_unique<RetroCRTEffect>();
	retroCRT_->Init();
	retroEffectData_ = {
		Temp::Color::WHITE,
		0.15f, true,
		{0.003f, 0.005f}, {-0.003f, -0.005f}, true,
		0.0f, false,
		0.3f, true,
		0.0f, false,
		WinApp::WindowSize(),
		0.0f
	};
	retroCRT_->SetMtlData(retroEffectData_);

	/* ----- TitleBackGround タイトルバックグラウンド ----- */
	titleBG_ = std::make_unique<TitleBackGround>();
	titleBG_->Init();

	/* ----- TitleUIManager タイトルラベルUI ----- */
	uiManager_ = std::make_unique<TitleUIManager>();
	uiManager_->Init();

	/* ----- SceneTransition シーントランジション ----- */
	sceneTransition_ = SceneTransition::GetInstance();
	sceneTransition_->Init();
	sceneTransition_->SetState(Cloased);
	sceneTransition_->StartFadeIn();
}


/// <summary>
/// 更新処理
/// </summary>
void TitleScene::Update()
{
	/* ----- Camera カメラ ----- */
	cameraData_.lock()->Update();

	/* ----- TitleBackGround タイトルバックグラウンド ----- */
	titleBG_->Update();
	
	/* ----- TitleUIManager タイトルラベルUI ----- */
	uiManager_->Update();

	/* ----- SceneTransition シーントランジション ----- */
	sceneTransition_->Update();

	// ボタン押下でトランジション開始
	if (input_->Trigger(PadData::A) || input_->Trigger(DIK_SPACE)) {

		// GameDataに選択したステージ番号を保存しておく
		GameData::GetInstance()->Set_StageSelectNum(uiManager_->GetStageSelectNum());
		sceneTransition_->StartFadeOut();
	}
	// 画面が閉じたらシーン変更
	if (sceneTransition_->GetNowState() == TransitionState::Cloased) {
		Manager_->ChangeSceneState(std::make_unique<GameScene>());
		return;
	}


#ifdef _DEBUG
	ImGui::Begin("TitleScene");
	cameraData_.lock()->DrawImGui();
	ImGui::End();
#endif // _DEBUG
}


/// <summary>
/// 背景スプライトの描画処理
/// </summary>
void TitleScene::BackSpriteDraw()
{
	/* ----- TitleBackGround タイトルバックグラウンド ----- */
	titleBG_->Draw2DBack();
}


/// <summary>
/// ３Dオブジェクトの描画処理
/// </summary>
void TitleScene::ModelDraw()
{
	/* ----- TitleUIManager タイトルラベルUI ----- */
	uiManager_->Draw3D();
}


/// <summary>
/// 前景スプライトの描画処理
/// </summary>
void TitleScene::FrontSpriteDraw()
{
	/* ----- RetroCRT レトロエフェクト ----- */
	retroCRT_->SetMtlData(retroEffectData_);
	retroCRT_->Draw();

	/* ----- TitleUIManager タイトルラベルUI ----- */
	uiManager_->Draw2DFront();
	
	/* ----- TitleBackGround タイトルバックグラウンド ----- */
	titleBG_->Draw2DFront();

	/* ----- SceneTransition シーントランジション ----- */
	sceneTransition_->Draw2DFront();
}
