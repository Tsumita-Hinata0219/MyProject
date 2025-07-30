#pragma once

#include "Resource/BufferResource/BufferResource.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "Resource/View/SRVManager/SRVManager.h"
#include "Transform/Structure/Transform.h"
#include "../../../PipeLineManager/PipeLineManager.h"
#include "../Structure/ModelStructure.h"
#include "../Resources/ModelResources.h"


enum StateType {
	NONE,
	OBJ,
	GLTF,
};
enum class RenderState {
	None,			        // 普通の描画 (背面カリング, 全て書き込み)
	CullingMode_None,       //　カリングなし
	FillMode_WireFrame,     // ワイヤーフレーム
	DepthWriteMask_ZERO,    // DepthWriteMaskをZeroに設定
};

// Modelの前方宣言
class Model;

// CameraManagerの前方宣言
class CameraManager;


/* ModelState モデルのステートパターン */
class IModelState {

public: // メンバ変数

	// デストラクタ
	virtual ~IModelState() {};

	// 描画処理
	virtual void Draw(Transform& transform) = 0;
	
	// コマンドコール
	virtual void CommandCall() = 0;

#pragma region Accessor アクセッサ

	// RenderState
	void SetRenderState(const RenderState& setState) { this->renderState_ = setState; }

	// Data
	ModelDatas GetModelDatas() { return this->datas_; }
	void SetModelDatas(const ModelDatas& setData) { this->datas_ = setData; }
	
	// Mesh
	MeshData GetMeshData() const { return this->datas_.mesh; }
	void SetMeshData(const MeshData& setData) { this->datas_.mesh = setData; }

	// Material
	MaterialDataN GetMaterialData() const { return this->datas_.material; }
	void SetMaterialData(const MaterialDataN& seteData) { this->datas_.material = seteData; }
	// Color
	void SetMaterialColor(const Vector4& setColor) { this->datas_.material.color = setColor; }
	// Texture
	void SetMaterialTexture(uint32_t setTexture) { this->datas_.material.textureHandle = setTexture; }
	// uvTransform
	void SetMaterialUVMat(const Matrix4x4& setMat) { this->datas_.material.uvTransform = setMat; }

	// Light
	DirectionalLightData GetLightData() const { return this->datas_.light; }
	void SetLightData(const DirectionalLightData& setData) { this->datas_.light = setData; }

	// Environment
	EnvironmentData GetEnvironmentData() const { return this->datas_.environment; }
	void SetEnvironmentData(const EnvironmentData& setData) { this->datas_.environment = setData; }

	// ColorAddition
	ColorAddition GetColorAddition() const { return this->datas_.colorAddition; }
	void SetColorAddition(const ColorAddition& setData) { this->datas_.colorAddition = setData; }

	// NormalMap
	NormalMap GetNormalMap() const { return this->datas_.normalMap; }
	void SetNormalMap(const NormalMap& setData) { this->datas_.normalMap = setData; }

	// Decal
	DecalSetting GetDecal() const { return this->datas_.decal; }
	void SetDecal(const DecalSetting& setData) { this->datas_.decal = setData; }

#pragma endregion 


protected:

	// BufferResourceの生成
	void CreateBufferResource(ModelDatas datas) {
		// mesh
		buffers_.mesh.CreateCBV(UINT(datas.mesh.vertices.size()));
		// vertexBufferView
		buffers_.vertex.CreateCBV(UINT(datas.mesh.vertices.size()));
		buffers_.vertex.CreateVertexBufferView();
		// indexBufferView
		buffers_.indices.CreateCBV(UINT(datas.mesh.indices.size()));
		buffers_.indices.CreateIndexBufferView();
		// transform
		buffers_.transform.CreateCBV();
		// material
		buffers_.material.CreateCBV();
		// light
		buffers_.light.CreateCBV();
		// encironment
		buffers_.enviroment.CreateCBV();
		// colorAddition
		buffers_.colorAddition.CreateCBV();
		// normalMap
		buffers_.normalMap.CreateCBV();
		// decal
		buffers_.decal.CreateCBV();

		if (stateType_ == GLTF) {
			// influence
			buffers_.influence.CreateCBV(UINT(datas.mesh.vertices.size()));
			// palette
			//buffers_.palette.CreateCBV(UINT(datas_.skeleton.joints.size()));
		}
	}


protected: // メンバ変数

	// バッファーに書き込むデータ
	ModelDatas datas_{};

	// バッファー
	ModelBuffers buffers_{};

	// カメラマネージャー
	CameraManager* cameraManager_ = nullptr;

	// モデルステート
	StateType stateType_ = NONE;

	// レンダーステート
	RenderState renderState_ = RenderState::None;
};