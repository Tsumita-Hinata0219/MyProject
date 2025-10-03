#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include <cassert>
#include <map>
#include<vector>

#include "Math/MyMath.h"
#include "ResourceSystem/Tex/TextureResource.h"
#include <DirectXTex.h>

// 前方宣言
 class TexManager;
 class DirectXManager;
 class DescriptorHeapManager;
 class CommandManager;

namespace TexFileExt {
const std::pair<std::string, uint32_t> PNG = { ".png", 0 };
const std::pair<std::string, uint32_t> JPEG = { ".jpeg", 1 };
const std::pair<std::string, uint32_t> DDS = { ".dds", 2 };
};

/* テクスチャのローダー */
class TextureLoader {

private: // シングルトン

	// コンストラクタ、デストラクタ
	TextureLoader();
	~TextureLoader() = default;
	TextureLoader(const TextureLoader&) = delete;
	const TextureLoader& operator=(const TextureLoader&) =  delete;

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static TextureLoader* GetInstance() {
		static TextureLoader instance;
		return &instance;
	}

	/// <summary>
	/// テクスチャのロード
	/// </summary>
	uint32_t Load(const std::string& rootPath, const std::string& fileName);
	
private:

	/// <summary>
	/// 各種データを読み込む
	/// </summary>
	void LoadPNGorJPEG(const std::string& path, TexResource* resource);
	void LoadDDS(const std::string& path, TexResource* resource);

private:

	/// <summary>
	/// ファイル読み込み（WIC / DDS）
	/// </summary>
	DirectX::ScratchImage LoadTextureFile(const std::string& path);

	/// <summary>
	/// MipMap生成
	/// </summary>
	DirectX::ScratchImage GenerateMipMaps(const DirectX::ScratchImage& srcImage);

	/// <summary>
	/// Resource作成用のDesc作成
	/// </summary>
	D3D12_RESOURCE_DESC CreateResourceDesc(const DirectX::TexMetadata& metadata);
	D3D12_HEAP_PROPERTIES CreateHeapProperties();

	/// <summary>
	/// D3D12Resource作成
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateD3D12Resource(const D3D12_RESOURCE_DESC& desc,
		const D3D12_HEAP_PROPERTIES& heapProps);

	/// <summary>
	/// GPUへデータを転送
	/// </summary>
	void UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource>& texture, 
		const DirectX::ScratchImage& mipImages);

	/// <summary>
	/// コマンド実行
	/// </summary>
	void ExeCommand();

private:

	TexManager* texMgr_ = nullptr;
	DirectXManager* dxMgr_ = nullptr;
	DescriptorHeapManager* descHeapMgr_ = nullptr;
	CommandManager* comdMgr_  = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_{};

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> pendingIntermediateResources_;
	std::vector<UINT64> pendingFenceValues_;
};
