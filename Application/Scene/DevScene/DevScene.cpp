#include "DevScene.h"


/// <summary>
/// コンストラクタ
/// </summary>
DevScene::DevScene()
{
	cameraManager_ = CameraManager::GetInstance();
	gameEntityManager_ = std::make_unique<GameEntityManager>();
	collisionManager_ = Entity::Collision::CollisionManager::GetInstance();

	floor_ = std::make_unique<Floor>();
}


/// <summary>
/// デストラクタ
/// </summary>
DevScene::~DevScene()
{
}


/// <summary>
/// 初期化処理
/// </summary>
void DevScene::Initialize()
{
	LoadData();

	cameraManager_->ReSet();
	cameraData_ = cameraManager_->GetCameraDataWeak();
	cameraData_.lock()->Init({ 0.15f, 0.0f, 0.0f }, { 0.0f, 4.0f, -20.0f });

	floor_->Init();
	floor_->SetScale(Vector3{ 20.0f, 1.0f, 20.0f });
	floor_->SetTranslate(Vector3{ 0.0f, -2.0f, 0.0f });

	gameEntityManager_->Add_NewEntity(std::make_shared<DevActor>());
}


/// <summary>
/// 更新処理
/// </summary>
void DevScene::Update()
{
	cameraData_.lock()->Update();
	cameraData_.lock()->DrawImGui();

	floor_->Update();

	gameEntityManager_->Update_Entity();

#ifdef _DEBUG

	ImGui::Begin("DevScene");

	collisionManager_->DrawimGui();

	ImGui::End();

#endif // _DEBUG

}


/// <summary>
/// 背景スプライトの描画処理
/// </summary>
void DevScene::BackSpriteDraw()
{
}


/// <summary>
/// ３Dオブジェクトの描画処理
/// </summary>
void DevScene::ModelDraw()
{
	floor_->Draw3D();
	gameEntityManager_->Render_Entity();
}


/// <summary>
/// 前景スプライトの描画処理
/// </summary>
void DevScene::FrontSpriteDraw()
{
}


/// <summary>
/// データのロード
/// </summary>
void DevScene::LoadData()
{
	auto graphicManager = RenderSystem::GraphicAssetManager::GetInstance();
	graphicManager->LoadData("Obj/Dev/Axis", "Axis.obj");
}
