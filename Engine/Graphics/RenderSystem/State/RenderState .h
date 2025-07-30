#pragma once

#include "Math/MyMath.h"
#include "../Structure/RenderStructure.h"
#include "Entity/TransformNode/Node/TransformNode.h"


// 前方宣言
class IActor;
class CameraManager;
class PipeLineManager;

namespace RenderSystem {

// 前方宣言
class GraphicAssetManager;


class RenderState {

private:

	// 親ActorのTransformNode
	std::weak_ptr<TransformNode> transNode_;

	// 描画に必要なDeta
	RenderSystem::Rendering::SceneData datas_;
	// GPUに送るBuffer
	std::unique_ptr<RenderSystem::Rendering::BufferResources> buffers_;

	// CameraManager
	CameraManager* cameraManager_ = nullptr;
	// PipeLineManager
	PipeLineManager* pipeLineManager_ = nullptr;
	// RenderAssetManager
	GraphicAssetManager* renderAssetManager_ = nullptr;


public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	RenderState(std::weak_ptr<IActor> owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RenderState() = default;

	/// <summary>
	/// データの設定
	/// </summary>
	void SetRenderData(const std::string& assetName);

	/// <summary>
	/// Buffer生成
	/// </summary>
	void Create_RenderBuffer();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw3D();


private:

	/// <summary>
	/// 描画データの更新
	/// </summary>
	void Update_RenderData();

	/// <summary>
	/// PipeLineのチェック
	/// </summary>
	void Check_PipeLine();

	/// <summary>
	/// 描画データのバインド
	/// </summary>
	void Bind_RenderData();

	/// <summary>
	/// 描画
	/// </summary>
	void Execute_DrawCommand();


};

}