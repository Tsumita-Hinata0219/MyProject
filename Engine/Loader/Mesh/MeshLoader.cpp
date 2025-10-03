#include "MeshLoader.h"
#include "ResourceSystem/Mesh/MeshManager.h"

MeshLoader::MeshLoader()
{
	meshMgr_ = MeshManager::GetInstance();
}

uint32_t MeshLoader::Load(const std::string& rootPath, const std::string& fileName)
{
	// フルファイルパス
	std::string fullPath = "Resources/" + rootPath + "/" + fileName;

	// すでに登録済みなら、そのIDを返す
	if (meshMgr_->Exists(fileName)) {
		return meshMgr_->GetIDByString(fileName);
	}

	// 新しくリソースを作成
	std::unique_ptr<MeshResource> resource = std::make_unique<MeshResource>();

	// 拡張子の抽出
	std::string ext = GetExtension(fileName);

	if (ext == MeshFileExt::OBJ.first) {
		LoadOBJ(fullPath, resource.get());
	}
	else if (ext == MeshFileExt::GLTF.first) {
		LoadGLTF(fullPath, resource.get());
	}

	// managerに登録
	meshMgr_->Register(fileName, std::move(resource));

	// 登録後にIDを返す
	return meshMgr_->GetIDByString(fileName);
}

void MeshLoader::LoadOBJ(const std::string& path, MeshResource* resource)
{
	// ========== 1. ファイル読み込み ==========
	Assimp::Importer importer;
	std::string fullPath = "Resources/" + path;
	//三角形の並び順を逆にする。UVをフリップする
	const aiScene* scene = importer.ReadFile(
		fullPath.c_str(),
		aiProcess_FlipWindingOrder | aiProcess_FlipUVs
	);
	assert(scene && scene->HasMeshes());

	// ========== 2. MeshInfo作成 ==========
	MeshInfo meshInfo{};

	// ========== 3. 頂点データ作成 ==========
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

	// ========== 4. 各種データの設定 ==========
	resource->SetMeshInfo(meshInfo);
	resource->SetFullPath(fullPath);
}

void MeshLoader::LoadGLTF(const std::string& path, MeshResource* resource)
{
	path, resource;
}


