#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <cassert>
#include <map>
#include <utility>
#include<vector>

#include "Math/MyMath.h"
#include "TextureResource.h"


/* テクスチャリソース管理クラス */
class TexManager {

private: // シングルトン

	TexManager() = default;
	~TexManager() = default;
	TexManager(const TexManager&) = delete;
	const TexManager& operator=(const TexManager&) = delete;

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static TexManager* GetInstance() {
		static TexManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// Mapに登録
	/// </summary>
	void Register(uint32_t key, std::unique_ptr<TexResource> resource) {
		// keyで検索し引っかかるならreturn
		auto it = texResourceMap_.find(key);
		if (it != texResourceMap_.end()) {
			return;
		}

		// なければMapに追加
		if (resource) {
			texResourceMap_[key] = std::move(resource);
		}
	}

	/// <summary>
	/// Map内に存在するか
	/// </summary>
	bool Exists(uint32_t key) const {
		return texResourceMap_.find(key) != texResourceMap_.end();
	}

	/// <summary>
	/// uint32_t型のキーを返す
	/// </summary>
	const uint32_t GetKey(uint32_t key) const {
		auto it = texResourceMap_.find(key);
		if (it != texResourceMap_.end()) {
			return it->first; // mapのキー
		}
		throw std::runtime_error("Key not found"); 
	}

private:

	// テクスチャのコンテナマップ
	std::unordered_map<uint32_t, std::unique_ptr<TexResource>> texResourceMap_;
};
