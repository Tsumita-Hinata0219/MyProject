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

// 前方宣言
class MeshManager;

namespace MeshFileExt {
const std::pair<std::string, uint32_t> OBJ = { ".obj", 0 };
const std::pair<std::string, uint32_t> GLTF = { ".gltf", 1 };
};

/* メッシュのローダー */
class MeshLoader {

private:

	// コンストラクタ、デストラクタ
	MeshLoader();
	~MeshLoader() = default;
	MeshLoader(const MeshLoader&) = delete;
	const MeshLoader& operator=(const MeshLoader&) = delete;

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static MeshLoader* GetInstance() {
		static MeshLoader instance;
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
	void LoadOBJ(const std::string& path, MeshResource* resource);
	void LoadGLTF(const std::string& path, MeshResource* resource);

private:

	MeshManager* meshMgr_ = nullptr;

};