#include "RenderableLoader.h"

void RenderableLoader::Load(const std::string& rootPath, const std::string& fileName)
{
    // フルファイルパス（重複しないよう修正）
    std::string fullPath = "Resources/" + rootPath + "/" + fileName;
    std::uint32_t key = FNV1aHash(fileName);

    // 既存ならreturn
    if (renderableInfoMap_.find(key) != renderableInfoMap_.end()) {
        return;
    }

    try {
        RenderableInfo info = {};

        // Assimpでobj/gltf等を読む
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(fullPath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
        if (!scene || !scene->HasMeshes()) {
            throw std::runtime_error("Scene has no meshes or failed to load: " + fullPath);
        }

        // Meshの読み込み
        info.meshKey = LoadMesh(fileName, fullPath, scene);
        // Materialの読み込み
        info.texKey = LoadMaterial(rootPath, scene);

        // 管理マップに登録
        renderableInfoMap_[key] = info;
        // ログ出力
        Log("Renderable loaded: " + fullPath);
    }
    catch (const std::exception& e) {
        Log(std::string("Error loading renderable: ") + e.what());
    }
}

uint32_t RenderableLoader::LoadMesh(const std::string& fileName, const std::string& fullPath, const aiScene* scene)
{
    MeshInfo meshInfo{};
    for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
        aiMesh* mesh = scene->mMeshes[meshIndex];
        if (!mesh->HasNormals() || !mesh->HasTextureCoords(0)) {
            throw std::runtime_error("Mesh missing normals or texture coords: " + fullPath);
        }
        meshInfo.vertices.resize(mesh->mNumVertices);

        // 頂点解析
        for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
            aiVector3D& position = mesh->mVertices[vertexIndex];
            aiVector3D& normal = mesh->mNormals[vertexIndex];
            aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

            meshInfo.vertices[vertexIndex].position = { -position.x, position.y, position.z, 1.0f };
            meshInfo.vertices[vertexIndex].normal = { -normal.x, normal.y, normal.z };
            meshInfo.vertices[vertexIndex].texCoord = { texcoord.x, texcoord.y };
        }

        // インデックス解析
        for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
            aiFace& face = mesh->mFaces[faceIndex];
            if (face.mNumIndices != 3) {
                throw std::runtime_error("Non-triangle face detected");
            }
            for (uint32_t element = 0; element < face.mNumIndices; ++element) {
                uint32_t vertexIndex = face.mIndices[element];
                meshInfo.indices.push_back(vertexIndex);
            }
        }

        // ノード階層構築（再帰）
        meshInfo.rootNode = ReadNode(scene->mRootNode);
    }

    auto resource = std::make_unique<MeshResource>();
    resource->SetMeshInfo(meshInfo);
    resource->SetFullPath(fullPath);

    MeshManager::GetInstance()->Register(fileName, std::move(resource));
    return MeshManager::GetInstance()->GetIDByString(fileName);
}

MeshNode RenderableLoader::ReadNode(const aiNode* node) {
    MeshNode result;
    aiMatrix4x4 aiLocalMatrix = node->mTransformation;
    aiLocalMatrix.Transpose();

    aiVector3D scale, translate;
    aiQuaternion rotate;
    node->mTransformation.Decompose(scale, rotate, translate);

    result.transform.scale = { scale.x, scale.y, scale.z };
    result.transform.rotate = { rotate.w, rotate.x, -rotate.y, -rotate.z };
    result.transform.translate = { -translate.x, translate.y, translate.z };
    result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);

    result.name = node->mName.C_Str();
    result.Children.resize(node->mNumChildren);
    for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
        result.Children[childIndex] = ReadNode(node->mChildren[childIndex]);
    }
    return result;
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
    throw std::runtime_error("Diffuse texture not found in material: " + path);
}