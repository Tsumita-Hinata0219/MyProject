#pragma once

#include "ResourceSystem/Tex/TexManager.h"
#include "Loader/Texture/TextureLoader.h"
#include "ResourceSystem/Mesh/MeshManager.h"
#include "Loader/Mesh/MeshLoader.h"
#include "Loader/Renderable/RenderableLoader.h"

struct Tex {};
struct Mdl {};
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

template <>
uint32_t ResourceAPI::Get<Tex>(const std::string& key);
template <>
uint32_t ResourceAPI::Get<Mdl>(const std::string& key);

template <>
void ResourceAPI::Load<Tex>(const std::string& root, std::string name);
template <>
void ResourceAPI::Load<Mdl>(const std::string& root, std::string name);