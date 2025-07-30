#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include <cassert>
#include <map>
#include<vector>

#include "Core/DirectXManager/DirectXManager.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "Resource/CreateResource/CreateResource.h"
#include "Resource/View/SRVManager/SRVManager.h"
#include "Math/MyMath.h"


// TextureData
struct TextureData {
	uint32_t index;
	ComPtr<ID3D12Resource> resource;
	Vector2 size;
};

namespace TextureFileFormat {
	const std::pair<std::string, uint32_t> PNG = { ".png", 0 };
	const std::pair<std::string, uint32_t> JPEG = { ".jpeg", 1 };
	const std::pair<std::string, uint32_t> DSS = { ".dds", 2 };
};

class TextureManager {

private: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	const TextureManager& operator=(const TextureManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// TextureManagerのインスタンス取得
	/// </summary>
	static TextureManager* GetInstance() {
		static TextureManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// Textuerデータを読み込む
	/// </summary>
	static uint32_t LoadTexture(const std::string& filePath, const std::string& fileName);

	/// <summary>
	/// Textureデータの解放
	/// </summary>
	void UnLoadTexture();


private: // メンバ関数

	/// <summary>
	/// Fenceを生成する
	/// </summary>
	void CreateFence();

	/// <summary>
	/// 一回読み込んだものは読み込まない
	/// </summary>
	bool CheckTextureData(std::string key);

	/// <summary>
	/// TextureDataを生成する
	/// </summary>
	void CreateTextureDataFormatPng(std::string filePath, std::string key);
	void CreateTextureDataFormatJpeg(std::string filePath, std::string key);
	void CreateTextureDataFormatDDS(std::string filePath, std::string key);

	/// <summary>
	/// Textureファイルを開く
	/// </summary>
	DirectX::ScratchImage CreateMipImage(const std::string& filePath, const uint32_t format);

	/// <summary>
	/// DirectX12のTextureResourceを作る
	/// </summary>
	ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// metadataを基にResourceの設定
	/// </summary>
	D3D12_RESOURCE_DESC SettingResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// 利用するHeapの設定
	/// </summary>
	D3D12_HEAP_PROPERTIES SettingUseHeap();

	/// <summary>
	/// Resourceを生成する
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateCBV(D3D12_RESOURCE_DESC resourceDesc, D3D12_HEAP_PROPERTIES heapProperties);

	/// <summary>
	/// TextureResourceにデータを転送する
	/// </summary>
	void UpdateTextureData(const DirectX::TexMetadata& metadata, DirectX::ScratchImage& mipImages, TextureData textureData);
	Microsoft::WRL::ComPtr<ID3D12Resource>  UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);

	/// <summary>
	/// Commandを実行する
	/// </summary>
	void ExeCommands();

private: // メンバ変数

	// テクスチャのコンテナマップ
	std::map < std::string, TextureData> textureMaps_;

	// Fence
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_{};
};