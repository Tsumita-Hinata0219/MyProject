#pragma once

#include "Math/MyMath.h"
#include "Math/Struct.h"
#include "Resource/TextureManager/TextureManager.h"
#include "../Loader/ModelFileLoader.h"
#include "../Model.h"


#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



namespace ModelFileFormat {
	const std::pair<std::string, uint32_t> OBJ = { ".obj", 0 };
	const std::pair<std::string, uint32_t> GLTF = { ".gltf", 1 };
}

struct aiScene;



/* ModelManagerクラス */
class ModelManager {

private: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	const ModelManager& operator=(const ModelManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static ModelManager* GetInstance() {
		static ModelManager instance;
		return &instance;
	}

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// モデルデータの取得
	/// </summary>
	std::unique_ptr<Model> GetModel(const std::string& name) const;

	/// <summary>
	/// 指定のモデルデータの破棄
	/// </summary>
	void ModelRemove (std::string name);

	/// <summary>
	/// 全てのモデルデータの破棄
	/// </summary>
	void AllRemove();

	/// <summary>
	/// Objファイルを読み込む
	/// </summary>
	void LoadModel(const std::string& path, const std::string fileName);
	
	/// <summary>
	/// Nodeの階層構造からSkeletonを作る
	/// </summary>
	Skeleton CreateSkeleton(const Node& rootNode);


#pragma region Get

#pragma endregion 


private: // メンバ関数

	/// <summary>
	/// mtlファイルを読み込む関数
	/// </summary>
	MaterialData LoadMaterialTemplateFile(const std::string& filePath, const std::string& fileName, const std::string& routeFilePath);

	/// <summary>
	/// Nodeの情報を読む
	/// </summary>
	Node ReadNode(aiNode* node);

	/// <summary>
	/// NodeからJointを作る
	/// </summary>
	int32_t CreateJoint(const Node& node, const optional<int32_t>& parent, vector<Joint>& joints);


private: // メンバ変数

	uint32_t objHandle_ = 0;

	ModelData modelData_{};


	// モデルのマップ
	//unordered_map<string, unique_ptr<Model>> modelsMap_;
	std::unordered_map < std::string, ModelDatas> modelsMap_;
};

