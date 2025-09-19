#pragma once

#include "ResourceSystem/Tex/TexManager.h"


struct Texture {};
struct Model {};
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

private:

};


// ======================
// ResourceAPI::Get の特殊化
// ======================
template <>
uint32_t ResourceAPI::Get<Texture>(const std::string& key) {
	return TexManager::GetInstance()->GetKey(FNV1aHash(key));
}
