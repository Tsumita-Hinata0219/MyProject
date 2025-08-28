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
#include "Math/MyMath.h"


namespace TexFormat {
const std::pair<std::string, uint32_t> PNG = { ".png", 0 };
const std::pair<std::string, uint32_t> JPEG = { ".jpeg", 1 };
const std::pair<std::string, uint32_t> DDS = { ".dds", 2 };
};

/* Texture管理クラス */
class TexManager {

private: // シングルトン

	TexManager() = default;
	~TexManager() = default;
	TexManager(const TexManager&) = delete;
	TexManager& operator=(const TexManager&) = delete;

public:

	struct TexData {
		uint32_t index;
		ComPtr<ID3D12Resource> resource;
		Vector2 size;
	};

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static TexManager* GetInstance() {
		TexManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// Textureの読み込み
	/// </summary>
	/// <param name="path">ルートパス</param>
	/// <param name="name">ファイル名</param>
	void LoadTexture(const std::string& path, const std::string& name);

private:

	/// <summary>
	/// 既に読み込んだかどうかの確認
	/// </summary>
	/// <param name="key">ファイル名</param>
	bool Check_LoadedData(const std::string& key);

	/// <summary>
	/// TexData作成
	/// </summary>
	/// <param name="fullpath">フルパス</param>
	void Create_TexData(const std::string& fullpath);

	/// <summary>
	/// MipImageの作成
	/// </summary>
	/// <param name="path">フルパス</param>
	/// <param name="format">フォーマット</param>
	DirectX::ScratchImage Create_MipImage(const std::string& fullPath, const uint32_t format);


#pragma region


#pragma endregion

private:

	// テクスチャのコンテナマップ
	std::unordered_map < std::string, TexData> texMaps_;

	// Fence
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_{};

};