#pragma once

#include "Math/MyMath.h"

#include "../Structure/RenderStructure.h"
#include "Resource/TextureManager/TextureManager.h"

#include <unordered_map>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct aiScene;



namespace RenderSystem {

namespace FileFormat {
const std::pair<std::string, uint32_t> OBJ = { ".obj", 0 };
const std::pair<std::string, uint32_t> GLTF = { ".gltf", 1 };
}

namespace Loader {

/// <summary>
/// OBJファイルローダー
/// </summary>
Rendering::SceneData LoadObj(const std::string& path, const std::string& fileName);

/// <summary>
/// GLTFファイルローダー
/// </summary>
Rendering::SceneData LoadGLTF(const std::string& path, const std::string& fileName);

/// <summary>
/// MeshDataの解析
/// </summary>
Mesh::MeshData ParseMeshData(const aiScene* scene, std::string fileFormat);

/// <summary>
/// MaterialDataの解析
/// </summary>
Material::MaterialData ParseMaterialData(const aiScene* scene, const std::string& filePath, const std::string& format);

/// <summary>
/// Nodeの情報を読み込む
/// </summary>
Skinning::Node ReadNode(aiNode* node);

}
}