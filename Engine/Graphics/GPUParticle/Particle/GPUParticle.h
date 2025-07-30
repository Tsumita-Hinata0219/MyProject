#pragma once

#include "Resource/BufferResource/BufferResource.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "Transform/Structure/Transform.h"
#include "Graphics/PipeLineManager/PipeLineManager.h"

#include "../Resources/ParticleResources.h"

#include "Math/MyMath.h"


// 前方宣言
class CameraManager;
class Model;


/* GPUParticleクラス */
class GPUParticle {

public:

	/// <summary>
	/// コンストラクト
	/// </summary>
	GPUParticle() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GPUParticle() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="instanceNum">インスタンス数</param>
	void Init(const std::string& rootPath, const std::string& fileName, uint32_t instanceNum = 1);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクルの要素のバインド
	/// </summary>
	void Bind_ParticleProp(UINT num);

	/// <summary>
	/// パーティクルの生存時間のバインド
	/// </summary>
	void Bind_ParticleLifeTime(UINT num);

	/// <summary>
	/// フリーリストのバインド
	/// </summary>
	void Bind_FreeList(UINT num);

	/// <summary>
	/// フリーリストインデックスのバインド
	/// </summary>
	void Bind_FreeListIndex(UINT num);

	/// <summary>
	/// UAVBarrierを設定
	/// </summary>
	void SetUAVBarrier();


#pragma region Accessor アクセッサ

	/// <summary>
	/// インスタンシング数
	/// </summary>
	uint32_t GetInstanceNum() const { return instanceNum_; }


#pragma endregion 


private:

	/// <summary>
	/// バインド&ディスパッチ
	/// </summary>
	void Prope_Bind_Dispatch_Init();
	void Prope_Bind_Dispatch_Update();

	/// <summary>
	/// バインド&ドローコマンド
	/// </summary>
	void Bind_ExeDrawCommand();

	/// <summary>
	/// BufferResourceの作成
	/// </summary>
	void CreateBufferResource();


private:

	// カメラマネージャー
	CameraManager* cameraManager_ = nullptr;

	// パーティクルのインスタンス数
	uint32_t instanceNum_ = 0;
	const uint32_t minInstanceNum_ = 1024; // 最低数

	// パーティクルの要素
	GpuParticle::ModelData modelData_{};
	GpuParticle::ResourceHandles handles_{};

	// フリーリスト
	BufferResource<uint32_t> freeListBuffer_;
	// フリーリストインデックス
	BufferResource<uint32_t> freeListIndexBuffer_;

	// 使用するモデル
	std::unique_ptr<Model> model_;

};

