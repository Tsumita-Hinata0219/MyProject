#include "RenderState .h"
#include "System/Camera/Manager/CameraManager.h"
#include "Graphics/PipeLineManager/PipeLineManager.h"
#include "Graphics/RenderSystem/Asset/GraphicAssetManager.h"
#include "Entity/Actor/IActor.h"

/// <summary>
/// コンストラクタ
/// </summary>
RenderSystem::RenderState::RenderState(std::weak_ptr<IActor> owner)
{
	cameraManager_ = CameraManager::GetInstance();
	pipeLineManager_ = PipeLineManager::GetInstance();
	renderAssetManager_ = RenderSystem::GraphicAssetManager::GetInstance();

	transNode_ = owner.lock()->GetTransformNode();
	buffers_ = std::make_unique<RenderSystem::Rendering::BufferResources>();
}


/// <summary>
/// データの設定
/// </summary>
void RenderSystem::RenderState::SetRenderData(const std::string& assetName)
{
	// SceneDataを取得してdatas_に代入
	auto sceneDataOpt = renderAssetManager_->GetSceneData(assetName);

	if (sceneDataOpt.has_value()) {
		datas_ = sceneDataOpt.value();
	}
}


/// <summary>
/// Buffer生成
/// </summary>
void RenderSystem::RenderState::Create_RenderBuffer()
{
	// mesh
	buffers_->mesh.CreateCBV(UINT(datas_.mesh.vertices.size()));

	// VBV
	buffers_->vertex.CreateCBV(UINT(datas_.mesh.vertices.size()));
	buffers_->vertex.CreateVertexBufferView();

	// IBV
	buffers_->indices.CreateCBV(UINT(datas_.mesh.indices.size()));
	buffers_->indices.CreateIndexBufferView();

	// Material
	buffers_->material.CreateCBV();

	// Light
	buffers_->light.CreateCBV();

	// Environment
	buffers_->environment.CreateCBV();

	// ColorAddition
	buffers_->colorAddition.CreateCBV();
}


/// <summary>
/// 描画処理
/// </summary>
void RenderSystem::RenderState::Draw3D()
{
	// 描画データの更新
	Update_RenderData();
	// PipeLineのチェック
	Check_PipeLine();
	// 描画データのバインド
	Bind_RenderData();
	// Draw!
	Execute_DrawCommand();
}



/// <summary>
/// 描画データのバインド
/// </summary>
void RenderSystem::RenderState::Update_RenderData()
{
	// VBV
	buffers_->vertex.UpdateData(datas_.mesh.vertices.data());

	// IBV
	buffers_->indices.UpdateData(datas_.mesh.indices.data());

	// Material
	buffers_->material.UpdateData(&datas_.material);

	// Light
	buffers_->light.UpdateData(&datas_.light);

	// Environment
	buffers_->environment.UpdateData(&datas_.environment);

	// ColorAddition
	buffers_->colorAddition.UpdateData(&datas_.colorAddition);
}


/// <summary>
/// PipeLineのチェック
/// </summary>
void RenderSystem::RenderState::Check_PipeLine()
{
	int a = 0;

	if (a == 0) { // 背面カリング
		pipeLineManager_->SetPipeLine(PipeLine::Container::Graphic, 
			PipeLine::Category::Object3D, PipeLine::SubFilter::Cull_Mode_Back);
	}
	else if (a == 2) { // カリングなし
		pipeLineManager_->SetPipeLine(PipeLine::Container::Graphic, 
			PipeLine::Category::Object3D, PipeLine::SubFilter::Cull_Mode_None);
	}
	else if (a == 3) { // ワイヤーフレーム
		pipeLineManager_->SetPipeLine(PipeLine::Container::Graphic, 
			PipeLine::Category::Object3D, PipeLine::SubFilter::Fill_Mode_Wireframe);
	}
	else if (a == 4) { // Depth 0
		pipeLineManager_->SetPipeLine(PipeLine::Container::Graphic, 
			PipeLine::Category::Object3D, PipeLine::SubFilter::DepthWriteMask_Zero);
	}
}


/// <summary>
/// 描画データのバインド
/// </summary>
void RenderSystem::RenderState::Bind_RenderData()
{
	// VBV
	buffers_->vertex.IASetVertexBuffers(1);

	// IBV
	buffers_->indices.IASetIndexBuffer();

	// Transform
	transNode_.lock()->Bind_CBV(0);

	// Material
	buffers_->material.BindGraphicsCBV(1);

	// Camera
	cameraManager_->Bind_CameraData(2);

	// Light
	buffers_->light.BindGraphicsCBV(3);

	// Environment
	buffers_->environment.BindGraphicsCBV(4);

	// ColorAddition
	buffers_->colorAddition.BindGraphicsCBV(5);

	// Material Texture
	buffers_->material.BindGraphicsSRV(8, datas_.material.textureHandle);

	// Environment Texture
	buffers_->material.BindGraphicsSRV(9, datas_.environment.textureHandle);
}


/// <summary>
/// 描画
/// </summary>
void RenderSystem::RenderState::Execute_DrawCommand()
{
	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();
	// Draw!!
	commands.List->DrawIndexedInstanced(UINT(datas_.mesh.indices.size()), 1, 0, 0, 0);
}
