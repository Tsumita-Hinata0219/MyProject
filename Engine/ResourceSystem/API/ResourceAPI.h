#pragma once

#include "ResourceSystem/Tex/TexManager.h"
#include "Loader/Texture/TextureLoader.h"
#include "ResourceSystem/Mesh/MeshManager.h"
#include "Loader/Mesh/MeshLoader.h"
#include "Loader/Renderable/RenderableLoader.h"

struct Texture {};
struct Model {};
struct Mesh{};
struct Material{};
struct Sound {};


/* 各リソースにアクセスするためのAPIクラス */
class ResourceAPI {

private: // シングルトン

	ResourceAPI();
	~ResourceAPI() = default;
	ResourceAPI(const ResourceAPI&) = delete;
	const ResourceAPI& operator=(const ResourceAPI&) = delete;

public:

	template <typename T>
	static uint32_t Get(const std::string& key);

	template <typename T>
	static void Load(const std::string& root, std::string name);
    
};


// ======================
// ResourceAPI::Get の特殊化
// ======================
template <>
uint32_t ResourceAPI::Get<Texture>(const std::string& key) {
	return TexManager::GetInstance()->GetKey(FNV1aHash(key));
}

template <>
uint32_t ResourceAPI::Get<Model>(const std::string& key) {
	return TexManager::GetInstance()->GetKey(FNV1aHash(key));
}


// ======================
// ResourceAPI::Load の特殊化
// ======================
template <>
void ResourceAPI::Load<Texture>(const std::string& root, std::string name) {
	TextureLoader::GetInstance()->Load(root, name);
}

template <>
void ResourceAPI::Load<Model>(const std::string& root, std::string name) {
	RenderableLoader::GetInstance()->Load(root, name);
}
