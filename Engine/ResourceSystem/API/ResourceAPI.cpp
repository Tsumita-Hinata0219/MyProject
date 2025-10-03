#include "ResourceAPI.h"

// ======================
// ResourceAPI::Get の特殊化
// ======================
template <>
uint32_t ResourceAPI::Get<Tex>(const std::string& key) {
	return TexManager::GetInstance()->GetKey(FNV1aHash(key));
}

template <>
uint32_t ResourceAPI::Get<Mdl>(const std::string& key) {
	return MeshManager::GetInstance()->GetKey(FNV1aHash(key));
}

// ======================
// ResourceAPI::Load の特殊化
// ======================
template <>
void ResourceAPI::Load<Tex>(const std::string& root, std::string name) {
	TextureLoader::GetInstance()->Load(root, name);
}

template <>
void ResourceAPI::Load<Mdl>(const std::string& root, std::string name) {
	RenderableLoader::GetInstance()->Load(root, name);
}