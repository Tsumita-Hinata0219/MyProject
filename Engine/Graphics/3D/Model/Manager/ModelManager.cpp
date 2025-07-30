#include "ModelManager.h"

namespace fs = std::filesystem;


/// <summary>
/// 解放処理
/// </summary>
void ModelManager::Finalize() {

}


// モデルデータの取得
std::unique_ptr<Model> ModelManager::GetModel(const string& name) const
{
	// 指定された名前で検索をかける
	auto it = modelsMap_.find(name);

	// ヒットしたらポインタを返す
	if (it != modelsMap_.end()) {
		// 作ってあるモデルリソースを基に新しくモデル作ってそれを返す
		std::unique_ptr<Model> model = std::make_unique<Model>(it->second);
		return model;
	}

	// 見つからなかった場合に例外を投げる
	return nullptr;
}


// 指定のモデルデータの破棄
void ModelManager::ModelRemove(string name)
{
	// 指定された名前で検索をかける
	const auto& model = modelsMap_.find(name);

	// なかったらリターン
	if (model == modelsMap_.end()) {
		return;
	}

	// ヒットしたら破棄する
	modelsMap_.erase(name);
}


// 全てのモデルデータの破棄
void ModelManager::AllRemove()
{
	modelsMap_.clear();
}


/// <summary>
/// Objファイルを読み込む
/// </summary>
void ModelManager::LoadModel(const std::string& path, const std::string fileName)
{
	// 拡張子なしの名前で検索
	std::string modelName = fileName.substr(0, fileName.size() - 4);
	auto it = modelsMap_.find(modelName);
	if (it != modelsMap_.end()) {
		return; // すでに存在していたらreturn
	}

	// モデルローダー
	ModelFileLoader* loader = ModelFileLoader::GetInstance();
	std::string format = GetExtension(fileName);

	// モデルデータの読み込み
	ModelDatas* newData = nullptr;
	if (format == ModelFileFormat::OBJ.first) {
		newData = loader->ParseLoadObj(path, fileName);
	}
	else if (format == ModelFileFormat::GLTF.first) {
		newData = loader->ParseLoadGLTF(path, fileName);
	}

	// nullチェック
	if (!newData) {
		return;
	}

	// モデルデータを格納
	modelsMap_[newData->name] = (*newData);

	// メモリ解放（ここが重要）
	delete newData;
}


/// <summary>
/// Nodeの階層構造からSkeletonを作る
/// </summary>
Skeleton ModelManager::CreateSkeleton(const Node& rootNode)
{
	Skeleton skeleton{};
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	// 名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}

	return skeleton;
}


/// <summary>
/// mtlファイルを読み込む関数
/// </summary>
MaterialData ModelManager::LoadMaterialTemplateFile(const std::string& filePath, const std::string& fileName, const std::string& routeFilePath) {

	/* 1. 中で必要となる変数の宣言 */

	MaterialData materialData{}; // 構築するMaterialData
	std::string line{};			 // ファイルから読んだ１行を格納するもの


	/* 2. ファイルを開く */

	// ファイルを開く
	std::ifstream file("Resources/Obj/" + routeFilePath + "/" + filePath + "/" + fileName);

	//とりあえず開けなかったら止める
	assert(file.is_open());


	/* 3. 実際にファイルを読み、MaterualDataを構築していく */

	while (std::getline(file, line)) {

		std::string identifier{};
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		// "map_Kd" = textureのファイル名が記載されている

		if (identifier == "map_Kd") {

			std::string textureFileName{};
			s >> textureFileName;

			// 連結してファイルパスにする
			materialData.textureFilePath = "Resources/Obj/" + filePath + "/" + textureFileName;
		}
	}

	/* 4. MaterialData を返す */
	return materialData;
}


/// <summary>
/// Nodeの情報を読む
/// </summary>
Node ModelManager::ReadNode(aiNode* node)
{
	Node result;

	aiMatrix4x4 aiLocalMatrix = node->mTransformation; // nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose(); // 列ベクトル形式を行ベクトル形式に転置


	aiVector3D scale, translate;
	aiQuaternion rotate;

	// assimpの行列からSRTを抽出する関数を利用
	node->mTransformation.Decompose(scale, rotate, translate);

	// scaleはそのまま
	result.transform.scale = { scale.x, scale.y,scale.z };

	// x軸を反転、さらに回転方向が逆なので軸を反転させる
	result.transform.rotate = { rotate.w, rotate.x, -rotate.y, -rotate.z };

	// x軸を反転
	result.transform.translate = { -translate.x, translate.y,translate.z };

	// 上記で読み込んだ情報を元にLocalMatrixを求める
	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);

	result.name = node->mName.C_Str(); // Mode名を格納
	result.Children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的によんで階層構造を作っていく
		result.Children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}
	return result;
}


/// <summary>
/// NodeからJointを作る
/// </summary>
int32_t ModelManager::CreateJoint(const Node& node, const optional<int32_t>& parent, vector<Joint>& joints)
{
	Joint joint{};
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = Matrix4x4::identity;
	joint.transform = node.transform;
	joint.index = int32_t(joints.size()); // 現在登録されている数をIndexに
	joint.parent = parent;

	// SkeletonのJoint列に追加
	joints.push_back(joint);

	for (const Node& child : node.Children) {

		// 子Jointを作成し、そのIndexを登録
		int32_t chileIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(chileIndex);
	}

	// 自身のIndexを返す
	return joint.index;
}



