#include "RenderableLoader.h"

void RenderableLoader::Load(const std::string& rootPath, const std::string& fileName)
{
	// フルファイルパス
	std::string fullPath = "Resources/" + rootPath + "/" + fileName;
	// アクセスキー
	std::uint32_t key = FNV1aHash(fileName);

	// keyが既存ならreturn
	if (renderableInfoMap_.find(key) != renderableInfoMap_.end()) {
		return;
	}

	// 読み込み処理を行う
	RenderableInfo info = {};

	// asssimpでobjを読む
	Assimp::Importer importer;
	string file = ("Resources/" + fullPath);
	//三角形の並び順を逆にする。UVをフリップする(texcoord.y = 1.0f - texcoord.y;の処理)
	const aiScene* scene = importer.ReadFile(file.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // メッシュがないのは対応しない

	// Meshの読み込み処理
	info.meshKey = LoadMesh(key, fullPath, scene);
	// Materialの読み込み
	info.texKey = LoadMaterial(rootPath, scene);

}

uint32_t RenderableLoader::LoadMesh(uint32_t key, const std::string& fullPath, const aiScene* scene)
{
	MeshInfo meshInfo{};

	// ========== 頂点データ作成 ==========
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));

		// 頂点分のサイズに変える
		meshInfo.vertices.resize(mesh->mNumVertices);

		// Verticesを解析する
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

			// 右手系->左手系への変換
			meshInfo.vertices[vertexIndex].position = { -position.x, position.y, position.z, 1.0f };
			meshInfo.vertices[vertexIndex].normal = { -normal.x, normal.y, normal.z };
			meshInfo.vertices[vertexIndex].texCoord = { texcoord.x, texcoord.y };
		}

		// Indexを解析する 
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				meshInfo.indices.push_back(vertexIndex);
			}
		}

		// TODO Nodeの読み込み処理を作る
		aiNode* node = scene->mRootNode;
		aiMatrix4x4 aiLocalMatrix = node->mTransformation; // nodeのlocalMatrixを取得
		aiLocalMatrix.Transpose(); // 列ベクトル形式を行ベクトル形式に転置

		aiVector3D scale, translate;
		aiQuaternion rotate;

		// assimpの行列からSRTを抽出する関数を利用
		node->mTransformation.Decompose(scale, rotate, translate);

		// scaleはそのまま
		meshInfo.rootNode.transform.scale = { scale.x, scale.y,scale.z };
		// x軸を反転、さらに回転方向が逆なので軸を反転させる
		meshInfo.rootNode.transform.rotate = { rotate.w, rotate.x, -rotate.y, -rotate.z };
		// x軸を反転
		meshInfo.rootNode.transform.translate = { -translate.x, translate.y,translate.z };

		// 上記で読み込んだ情報を元にLocalMatrixを求める
		meshInfo.rootNode.localMatrix = MakeAffineMatrix(
			meshInfo.rootNode.transform.scale,
			meshInfo.rootNode.transform.rotate,
			meshInfo.rootNode.transform.translate);

		meshInfo.rootNode.name = node->mName.C_Str(); // Mode名を格納
		meshInfo.rootNode.Children.resize(node->mNumChildren); // 子供の数だけ確保
		for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
			// 再帰的によんで階層構造を作っていく
			//meshInfo.rootNode.Children[childIndex] = ReadNode(node->mChildren[childIndex]);
		}
	}

	// 新しく作るresource
	std::unique_ptr<MeshResource> resource = std::make_unique<MeshResource>();
	// Dataの設定
	resource->SetMeshInfo(meshInfo);
	resource->SetFullPath(fullPath);

	// Managerに登録
	MeshManager::GetInstance()->Register(key, std::move(resource));

	return MeshManager::GetInstance()->GetKey(key);
}

uint32_t RenderableLoader::LoadMaterial(const std::string& path, const aiScene* scene)
{
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString texFileName;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &texFileName);

			return TextureLoader::GetInstance()->Load(path, texFileName.C_Str());
		}
	}
	// 例外を投げて処理を中断
	throw std::runtime_error("Diffuse texture not found in material: " + path);
}
