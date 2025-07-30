#pragma once

#include <memory>

#include "Graphics/PipeLineManager/PipeLineManager.h"
#include "Transform/Structure/Transform.h"
#include "Resource/BufferResource/BufferResource.h"

// 前方宣言
class CameraManager;
class PipeLineManager;



struct DecalData {
	Matrix4x4 decalMatrix; // デカールの位置、回転、スケールを決定する行列
	Vector4 decalColor; // デカールの色
	float decalStrength; // デカールの適用強度
	int enable;
	SRTData srt{};
	int textureHandle = 1u;
};


/* デカール */
class Decal {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Decal();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Decal() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(uint32_t texHandle = 1u);

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
	/// Buffer生成
	/// </summary>
	void Create_Buffer();

	/// <summary>
	/// 描画データの更新
	/// </summary>
	void Update_Data();

	/// <summary>
	/// PipeLineのチェック
	/// </summary>
	void Check_PipeLine();

	/// <summary>
	/// 描画データのバインド
	/// </summary>
	void Bind_Data();

	/// <summary>
	/// 描画
	/// </summary>
	void Execute_Draw();


private:

	// CameraManager
	CameraManager* cameraMgr_ = nullptr;
	// PipeLineManager
	PipeLineManager* pipeLineMgr_ = nullptr;

	// テクスチャ
	uint32_t texHandle_ = 1u;

	// Data
	DecalData data_{};
	// Buffer
	std::unique_ptr<BufferResource<DecalData>> buffer_;

};

