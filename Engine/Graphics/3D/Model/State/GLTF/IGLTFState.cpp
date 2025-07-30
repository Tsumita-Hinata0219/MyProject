#include "IGLTFState.h"
#include "System/Camera/Manager/CameraManager.h"


// コンストラクタ
IGLTFState::IGLTFState(ModelDatas datas)
{
	// CameraManagerのインスタンスの取得
	cameraManager_ = CameraManager::GetInstance();

	// ステートタイプの設定
	stateType_ = GLTF;

	// ModelDatasの設定
	datas_ = datas;

	// DatasをもとにBufferを作成
	CreateBufferResource(datas_);
}

// 描画処理
void IGLTFState::Draw(Transform& transform)
{
	// CameraResourceの取得
	auto cameraResource = cameraManager_->GetCameraDataWeak();

	// 諸々の計算
	transform.UpdateMatrix();
	transform.transformationMatData.World = transform.matWorld;
	transform.transformationMatData.WVP =
		transform.transformationMatData.World * cameraResource.lock()->viewMatrix * cameraResource.lock()->projectionMatrix;
	transform.transformationMatData.WorldInverseTranspose = Transpose(Inverse(transform.matWorld));

	// ここで書き込み
	// VBV
	buffers_.vertex.UpdateData(datas_.mesh.vertices.data());
	// IBV
	buffers_.indices.UpdateData(datas_.mesh.indices.data());
	// Material
	buffers_.material.UpdateData(&datas_.material);
	// Transform
	buffers_.transform.UpdateData((&transform.transformationMatData));
	// Light
	buffers_.light.UpdateData(&datas_.light);
	// Environment
	buffers_.enviroment.UpdateData(&datas_.environment);

	// コマンドコール
	CommandCall();
}


// コマンドコール
void IGLTFState::CommandCall()
{
	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	if (renderState_ == RenderState::None) {
		PipeLineManager::SetPipeLine(PipeLine::Container::Graphic, PipeLine::Category::Object3D, PipeLine::SubFilter::Cull_Mode_Back);
	}
	else if (renderState_ == RenderState::CullingMode_None) {
		PipeLineManager::SetPipeLine(PipeLine::Container::Graphic, PipeLine::Category::Object3D, PipeLine::SubFilter::Cull_Mode_None);
	}
	else if (renderState_ == RenderState::DepthWriteMask_ZERO) {
		PipeLineManager::SetPipeLine(PipeLine::Container::Graphic, PipeLine::Category::Object3D, PipeLine::SubFilter::DepthWriteMask_Zero);
	}

	// VertexBufferView
	buffers_.vertex.IASetVertexBuffers(1);
	// IndexBufferView
	buffers_.indices.IASetIndexBuffer();
	// Material
	buffers_.material.BindGraphicsCBV(0);
	// TransformationMatrix
	buffers_.transform.BindGraphicsCBV(1);
	// Camera
	cameraManager_->Bind_CameraData(2);
	// MaterialTexture
	//SRVManager::SetGraphicsRootDescriptorTable(3, datas_.material.textureHandle);
	buffers_.material.BindGraphicsSRV(3, datas_.material.textureHandle);
	// Light
	buffers_.light.BindGraphicsCBV(4);
	// Environment
	buffers_.enviroment.BindGraphicsCBV(5);
	// EnvironmentTexture
	//SRVManager::SetGraphicsRootDescriptorTable(6, datas_.environment.textureHandle);
	buffers_.material.BindGraphicsSRV(6, datas_.environment.textureHandle);
	// Draw!!
	commands.List->DrawIndexedInstanced(UINT(datas_.mesh.indices.size()), 1, 0, 0, 0);
}
