#pragma once

#include "Graphics/CommandManager/CommandManager.h"
#include "Resource/CreateResource/CreateResource.h"
#include "Graphics/PipeLineManager/PipeLineManager.h"

#include "GameObject/GameObject.h"

#include "Resource/SkyBoxResource.h"


/* Skyboxクラス */
class Skybox {

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Skybox() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Skybox() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(uint32_t dds);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private:

	/// <summary>
	/// MeshDataの書き込み
	/// </summary>
	void WriteMeshData();

	/// <summary>
	/// コマンドコール
	/// </summary>
	void CommandCall();


private: // メンバ変数

	// データ
	SkyBoxDatas datas_{};

	// バッファー
	SkyBoxBuffers buffers_{};

	// Transform
	Transform transform_{};
	float initScale_ = 1.0f;

	// 使用するTextureHandle
	uint32_t texture_ = 0;

	// 定数の頂点数とインデックス数
	const size_t kNumVertices_ = 24; // 各面4頂点、6面で計24頂点
	const size_t kNumIndices_ = 36;  // 各面6インデックス、6面で計36インデックス

	// カメラマネージャー
	CameraManager* cameraManager_ = nullptr;
};

