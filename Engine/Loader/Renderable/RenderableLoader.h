#pragma once

#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Math/MyMath.h"
#include "ResourceSystem/Mesh/MeshResource.h"
#include "ResourceSystem/Mesh/MeshManager.h"
#include "Loader/Texture/TextureLoader.h"

struct RenderableInfo {
	uint32_t meshKey;
	uint32_t texKey;
};

/* 描画情報のローダー */
class RenderableLoader {

private: // シングルトン

	RenderableLoader() = default;
	~RenderableLoader() = default;
	RenderableLoader(const RenderableLoader&) = delete;
	const RenderableLoader& operator=(const RenderableLoader&) = delete;

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static RenderableLoader* GetInstance() {
		static RenderableLoader instance;
		return &instance;
	}

	/// <summary>
	/// 描画情報のロード
	/// </summary>
	void Load(const std::string& rootPath, const std::string& fileName);

private:

	/// <summary>
	/// メッシュデータを読み込み
	/// </summary>
	uint32_t LoadMesh(uint32_t key, const std::string& fullPath, const aiScene* scene);

	/// <summary>
	/// ノードの読み込み処理
	/// </summary>
	MeshNode ReadNode(const aiNode* node);

	/// <summary>
	/// マテリアルを読み込み
	/// </summary>
	uint32_t LoadMaterial(const std::string& path, const aiScene* scene);

private:

	std::unordered_map<uint32_t, RenderableInfo> renderableInfoMap_;

};