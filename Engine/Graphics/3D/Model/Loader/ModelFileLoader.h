#pragma once


#include "Math/MyMath.h"
#include "Math/Struct.h"
#include "Resource/TextureManager/TextureManager.h"
#include "../Model.h"
#include "../Manager/ModelManager.h"

#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct aiScene;


/* Modelファイルのローダー */
class ModelFileLoader  {

private: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	ModelFileLoader() = default;
	~ModelFileLoader() = default;
	ModelFileLoader(const ModelFileLoader&) = delete;
	const ModelFileLoader& operator=(const ModelFileLoader&) = delete;


public:

	// インスタンスの取得
	static ModelFileLoader* GetInstance() {
		static ModelFileLoader instance;
		return &instance;
	}

	// OBJファイルローダー
	ModelDatas* ParseLoadObj(const std::string& path, const std::string& fileName);

	// GLTFファイルローダー
	ModelDatas* ParseLoadGLTF(const std::string& path, const std::string& fileName);

private:

	// MeshDataの解析
	MeshData ParseMeshData(const aiScene* scene, std::string fileFormat);

	// MaterialDataの解析
	MaterialDataN ParseMaterialData(const aiScene* scene, const std::string& filePath, const std::string& format);

	// Nodeの情報を読み込む
	Node ReadNode(aiNode* node);
};

