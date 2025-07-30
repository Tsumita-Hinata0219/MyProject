#pragma once

#include "Resource/BufferResource/BufferResource.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "Resource/DescriptorManager/DescriptorManager.h"
#include "Transform/Structure/Transform.h"
#include "Graphics/PipeLineManager/PipeLineManager.h"
#include "Graphics/Animation/AnimationManager/AnimationManager.h"
#include "Structure/ModelStructure.h"
#include "Resources/ModelResources.h"

#include "State/IModelState.h"
#include "State/OBJ/IOBJState.h"
#include "State/GLTF/IGLTFState.h"


// 前方宣言
class ModelManager;
class KeyFrameAnimation;
class CameraManager;
struct aiScene;


/* Modelクラス */
class Model {

public:

	// コンストラクタ、デストラクタ
	Model();
	Model(ModelDatas datas);
	~Model() {};
	
	// 描画処理
	void Draw(Transform& transform);

	// アニメーションの再生
	void PlayAnimation(Animation animation, float time);
	
	// スケルトンの生成
	Skeleton CreateSkeleton();

	// スケルトンの更新
	void UpdateSkeleton(Skeleton& skeleton);

	// アニメーション再生 <- 最新
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);

	// スキンクラスターの生成
	SkinCluster CreateSkinCluster(const Skeleton& skeleton);

	// スキンクラスターの更新
	void UpdateSkinCluster(SkinCluster& skinCluster, const Skeleton& skeleton);


#pragma region Accessor アクセッサ

	// RenderState
	void SetRenderState(const RenderState& setState) { this->modelState_->SetRenderState(setState); }

	// ModelResources
	ModelDatas GetModelResources() const { return this->modelState_->GetModelDatas(); }

	// Mesh
	MeshData GetMeshData() const { return this->modelState_->GetMeshData(); }
	void SetMeshData(const MeshData& setData) { this->modelState_->SetMeshData(setData); }

	// Material
	MaterialDataN GetMaterialData() const { return this->modelState_->GetMaterialData(); }
	void SetMaterialData(const MaterialDataN& seteData) { this->modelState_->SetMaterialData(seteData); }
	void SetMaterialColor(Vector4 color) { this->modelState_->SetMaterialColor(color); }
	void SetMaterialTexture(uint32_t setTexture) { this->modelState_->SetMaterialTexture(setTexture); }
	void SetMaterialUVMat(const Matrix4x4& setMat) { this->modelState_->SetMaterialUVMat(setMat); }

	// Light
	DirectionalLightData GetLightData() const { return this->modelState_->GetLightData(); }
	void SetLightData(const DirectionalLightData& setData) { this->modelState_->SetLightData(setData); }

	// Environment
	EnvironmentData GetEnvironmentData() const { return this->modelState_->GetEnvironmentData(); }
	void SetEnvironmentData(const EnvironmentData& setData) { this->modelState_->SetEnvironmentData(setData); }

	// ColorAddition
	ColorAddition GetColorAddition() const { return this->modelState_->GetColorAddition(); }
	void SetColorAddition(const ColorAddition& setData) { this->modelState_->SetColorAddition(setData); }

	// NormalMap
	NormalMap GetNormalMap() const { return this->modelState_->GetNormalMap(); }
	void SetNormalMap(const NormalMap& setData) { this->modelState_->SetNormalMap(setData); }

	// Decal
	DecalSetting GetDecal() const { return this->modelState_->GetDecal(); }
	void SetDecal(const DecalSetting& setData) { this->modelState_->SetDecal(setData); }

#pragma endregion 


private:

	// カメラマネージャー
	CameraManager* cameraManager_ = nullptr;

	// State
	IModelState* modelState_ = nullptr;
};