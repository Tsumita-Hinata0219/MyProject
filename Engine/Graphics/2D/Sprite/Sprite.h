#pragma once

#include "Platform/WinApp/WinApp.h"
#include "Core/DirectXManager/DirectXManager.h"
#include "Resource/TextureManager/TextureManager.h"
#include "Graphics/PipeLineManager/PipeLineManager.h"
#include "Resource/DescriptorManager/DescriptorManager.h"
#include "Resource/View/SRVManager/SRVManager.h"
#include "System/Camera/Manager/CameraManager.h"
#include "Math/MyMath.h"
#include "Math/Struct.h"

#include "../SpriteResource/SpriteResource.h"



// CameraManagerの前方宣言
class CameraManager;

enum class SpriteAnchor {
	TopLeft,
	Center,
};



class Sprite {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(Vector2 size = { 128.0f, 128.0f });

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(Transform& transform);


#pragma region Set 設定

	// サイズ
	void SetSize(const Vector2& size) { size_ = size; }

	// uvTransform
	void SetUVTransform(const Matrix4x4& uvMat) { datas_.material.uvTransform = uvMat; }

	// テクスチャ
	void SetTexture(uint32_t texHD) { this->datas_.material.textureHandle = texHD; }

	// カラー
	void SetColor(const Vector4& color) { this->datas_.material.color = color; }

	// SRC
	void SetSrc(const QuadVertex2& src) { this->src_ = src; }

	// 基準
	void SetAnchor(const SpriteAnchor& setOrigin) { this->anchor_ = setOrigin; }

	// DissolveData
	void SetDissolveData(const SP::DissolveData& setData) { this->datas_.dissolve = setData; }

#pragma endregion


private:

	// コマンドコール
	void CommandCall();

	// DatasをもとにBufferを作成
	void CreateBufferResource();

	// メッシュデータの設定
	void SetMeshData();


private:

	// サイズ
	Vector2 size_{};

	// 色データ
	Vector4 color_ = Temp::Color::WHITE;

	// SRC
	QuadVertex2 src_ = {
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
	};

	// バッファーに書き込むデータ
	SpriteDatas datas_{};

	// バッファー
	SpriteBuffers buffers_{};

	// カメラマネージャー
	CameraManager* cameraManager_ = nullptr;

	// スプライトの基準
	SpriteAnchor anchor_ = SpriteAnchor::TopLeft;

	// VerticesとIndicesのサイズ
	const uint32_t verticesSize_ = 4;
	const uint32_t indicesSize_ = 6;
};



// UIの描画に必要なもの
struct SpriteProperty {
	std::unique_ptr<Sprite> sprite;
	Transform transform;
	uint32_t texture = 0;
};
