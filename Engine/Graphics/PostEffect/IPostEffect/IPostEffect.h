#pragma once

#include <iostream>
#include <memory>

#include "Resource/BufferResource/BufferResource.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "Resource/View/SRVManager/SRVManager.h"
#include "Resource/View/RTVManager/RTVManager.h"
#include "Resource/DescriptorManager/DescriptorManager.h"
#include "Graphics/PipeLineManager/PipeLineManager.h"
#include "Resource/TextureManager/TextureManager.h"
#include "../Materials/PostEffectMaterials.h"
#include "System/Camera/Manager/CameraManager.h"
#include "Math/MyMath.h"


/* IPostEffectクラス */
template <typename T>
class IPostEffect {

public: // メンバ関数

	// 仮想デストラクタ
	virtual ~IPostEffect() = default;

	// 初期化処理、更新処理、描画処理
	virtual void Init() = 0;
	virtual void Draw() = 0;

	// ImGuiの描画
	virtual void DrawImGui([[maybe_unused]] std::string name = "") = 0;

	// MtlDataの設定
	virtual void SetMtlData(T setData) { this->mtlData_ = setData; }


protected:

	// コマンドコール
	virtual void CommandCall() = 0;

	// SRVやBufferの作成
	virtual void Create() {
		// インスタンス取得
		descriptor_ = DescriptorManager::GetInstance();
		cameraManager_ = CameraManager::GetInstance();

		// SRV作成
		Microsoft::WRL::ComPtr<ID3D12Resource> stv = 
			RTVManager::GetRTV("PostEffect")->GetRTVPrope().Resources.Get();
		srv_ = descriptor_->CreatePostEffectSRV(stv);

		// バッファー作成
		mtlBuffer_.CreateCBV();
	}


protected: // メンバ変数

	// SRV
	uint32_t srv_ = 0;

	// レンダーテクスチャバッファー
	BufferResource<uint32_t> renderTexBuffer_;

	// バッファーにセットするMaterialData
	T mtlData_{};

	// バッファー
	BufferResource<T> mtlBuffer_{};

	// マスクテクスチャバッファー
	BufferResource<uint32_t> maskTexBuffer_;

	// カメラマネージャー
	CameraManager* cameraManager_ = nullptr;

	// ディスクリプマネージャー
	DescriptorManager* descriptor_ = nullptr;
};
