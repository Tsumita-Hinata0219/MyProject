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
#include "MeshResource.h"


class MeshManager {

private: // シングルトン

	MeshManager() = default;
	~MeshManager() = default;
	MeshManager(const MeshManager&) = delete;
	const MeshManager& operator=(const MeshManager&) = delete;
	
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static MeshManager* GetInstance() {
		static MeshManager instance;
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
	void Register(uint32_t key, std::unique_ptr<MeshResource> resource) {
		// keyで検索し引っかかるならreturn
		auto it = meshResourceMap_.find(key);
		if (it != meshResourceMap_.end()) {
			return;
		}

		// なければMapに追加
		if (resource) {
			meshResourceMap_[key] = std::move(resource);
		}
	}

	/// <summary>
	/// Map内に存在するか
	/// </summary>
	bool Exists(uint32_t key) const {
		return meshResourceMap_.find(key) != meshResourceMap_.end();
	}

	/// <summary>
	/// uint32_t型のキーを返す
	/// </summary>
	const uint32_t GetKey(uint32_t key) const {
		auto it = meshResourceMap_.find(key);
		if (it != meshResourceMap_.end()) {
			return it->first; // mapのキー
		}
		throw std::runtime_error("Key not found");
	}

private:

	// メッシュのコンテナマップ
	std::unordered_map<uint32_t, std::unique_ptr<MeshResource>> meshResourceMap_;
};