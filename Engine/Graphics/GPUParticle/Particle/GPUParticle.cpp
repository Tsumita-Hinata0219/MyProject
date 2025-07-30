#include "GPUParticle.h"
#include "System/Camera/Manager/CameraManager.h"
#include "Graphics/3D/Model/Manager/ModelManager.h"


/// <summary>
/// 初期化処理
/// </summary>
void GPUParticle::Init(const std::string& rootPath, const std::string& fileName, uint32_t instanceNum)
{
	ModelManager* modelManager = ModelManager::GetInstance();
	modelManager->LoadModel(rootPath, fileName);
	model_ = modelManager->GetModel(RemoveNameSuffix(fileName));

	// CameraManagerのインスタンス取得
	cameraManager_ = CameraManager::GetInstance();

	// インスタンス数の設定
	instanceNum_ = instanceNum * minInstanceNum_;

	// BufferResourceを作成
	CreateBufferResource();

	// 初期化バインド
	Prope_Bind_Dispatch_Init(); 
}


/// <summary>
/// 更新処理
/// </summary>
void GPUParticle::Update()
{
	// パーティクル要素
	Prope_Bind_Dispatch_Update();
}


/// <summary>
/// 描画処理
/// </summary>
void GPUParticle::Draw()
{
	// ここで書き込み
	// VBV
	handles_.vertex.UpdateData(model_->GetMeshData().vertices.data());
	// IBV
	handles_.indeces.UpdateData(model_->GetMeshData().indices.data());

	// 描画バインド
	Bind_ExeDrawCommand();
}


/// <summary>
/// パーティクルの要素のバインド
/// </summary>
void GPUParticle::Bind_ParticleProp(UINT num)
{
	handles_.particleElement.BindComputeSRV_Instanced(num);
}


/// <summary>
/// パーティクルの生存時間のバインド
/// </summary>
void GPUParticle::Bind_ParticleLifeTime(UINT num)
{
	handles_.lifeTimeBuffer_.BindComputeSRV_Instanced(num);
}


/// <summary>
/// フリーリストのバインド
/// </summary>
void GPUParticle::Bind_FreeList(UINT num)
{
	freeListBuffer_.BindComputeSRV_Instanced(num);
}


/// <summary>
/// フリーリストインデックスのバインド
/// </summary>
void GPUParticle::Bind_FreeListIndex(UINT num)
{
	freeListIndexBuffer_.BindComputeSRV_Instanced(num);
}


/// <summary>
/// UAVBarrierを設定
/// </summary>
void GPUParticle::SetUAVBarrier()
{
	handles_.particleElement.SetUAVBarrier();
	handles_.lifeTimeBuffer_.SetUAVBarrier();
	freeListBuffer_.SetUAVBarrier();
	freeListIndexBuffer_.SetUAVBarrier();
}


/// <summary>
/// バインド
/// </summary>
void GPUParticle::Prope_Bind_Dispatch_Init()
{
	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::SetPipeLine(PipeLine::Container::Compute, PipeLine::Category::GPUParticle_Init);

	// Particleの要素
	handles_.particleElement.BindComputeSRV_Instanced(0);

	// FreeList
	freeListBuffer_.BindComputeSRV_Instanced(1);

	// FreeListIndex
	freeListIndexBuffer_.BindComputeSRV_Instanced(2);

	// Dispach
	commands.List->Dispatch(1, 1, 1);

	// Barrierを張る
	this->SetUAVBarrier();
}
void GPUParticle::Prope_Bind_Dispatch_Update()
{
	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::SetPipeLine(PipeLine::Container::Compute, PipeLine::Category::GPUParticle_Update);

	// Particleの要素
	handles_.particleElement.BindComputeSRV_Instanced(0);

	// 生存時間
	handles_.lifeTimeBuffer_.BindComputeSRV_Instanced(1);

	// FreeList
	freeListBuffer_.BindComputeSRV_Instanced(2);

	// FreeListIndex
	freeListIndexBuffer_.BindComputeSRV_Instanced(3);

	// Dispach
	commands.List->Dispatch(1, 1, 1);

	// Barrierを張る
	this->SetUAVBarrier();
}


/// <summary>
/// バインド&ドローコマンド
/// </summary>
void GPUParticle::Bind_ExeDrawCommand()
{
	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::SetPipeLine(PipeLine::Container::Graphic, PipeLine::Category::GPUParticle_Draw);

	// VertexBufferView
	handles_.vertex.IASetVertexBuffers(1);

	// IndexBufferView
	handles_.indeces.IASetIndexBuffer();

	// ParticleElement : VS
	handles_.particleElement.BindGraphicsSRV_Instanced(0);

	// Camera
	cameraManager_->Bind_CameraData(1);

	// ParticleElement : PS
	handles_.particleElement.BindGraphicsSRV_Instanced(2);

	// MaterialTexture
	handles_.material.BindGraphicsSRV(3, model_->GetMaterialData().textureHandle);

	// Draw!!
	commands.List->DrawIndexedInstanced(UINT(model_->GetMeshData().indices.size()), instanceNum_, 0, 0, 0);
}


/// <summary>
/// BufferResourceの作成
/// </summary>
void GPUParticle::CreateBufferResource()
{
	// mesh
	handles_.mesh.CreateCBV(UINT(model_->GetMeshData().vertices.size()));

	// vertexBufferView
	handles_.vertex.CreateCBV(UINT(model_->GetMeshData().vertices.size()));
	handles_.vertex.CreateVertexBufferView();

	// indexBufferView
	handles_.indeces.CreateCBV(UINT(model_->GetMeshData().indices.size()));
	handles_.indeces.CreateIndexBufferView();

	// ParticleElement
	handles_.particleElement.CreateUAV(instanceNum_);

	// PreView
	handles_.preView.CreateCBV();

	// material
	handles_.material.CreateCBV(instanceNum_);
	handles_.material.CreateInstancingResource(instanceNum_);

	// LifeTime
	handles_.lifeTimeBuffer_.CreateUAV(instanceNum_);

	// FreeList
	freeListBuffer_.CreateUAV(instanceNum_);

	// FreeListIndex
	freeListIndexBuffer_.CreateUAV(instanceNum_);
}

