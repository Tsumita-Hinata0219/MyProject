#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>
#include <cstdint>

// FNV1aハッシュ
inline uint32_t FNV1aHash(const std::string& str) {
    uint32_t hash = 2166136261u;
    for (char c : str) {
        hash ^= static_cast<uint32_t>(c);
        hash *= 16777619u;
    }
    return hash;
}

// ===============================
// 汎用リソースマネージャ
// ===============================
template <typename T>
class IDMap {

public:
    using ResourcePtr = std::unique_ptr<T>;

    /// <summary>
    /// 登録
    /// </summary>
    void Register(const std::string& key, ResourcePtr resource) {
        uint32_t id = FNV1aHash(key);
        if (resources_.find(id) != resources_.end()) {
            return; // 既に登録済み
        }
        resources_[id] = std::move(resource);
        str_to_id_[key] = id;
        id_to_str_[id] = key;
    }

    /// <summary>
    /// 文字列キーで取得
    /// </summary>
    T* GetByString(const std::string& key) {
        auto it = str_to_id_.find(key);
        if (it == str_to_id_.end()) return nullptr;
        return GetByID(it->second);
    }

    /// <summary>
    /// IDで取得
    /// </summary>
    T* GetByID(uint32_t id) {
        auto it = resources_.find(id);
        if (it == resources_.end()) return nullptr;
        return it->second.get();
    }

    /// <summary>
    /// 存在確認
    /// </summary>
    bool Exists(const std::string& key) const {
        return str_to_id_.find(key) != str_to_id_.end();
    }
    bool Exists(uint32_t id) const {
        return resources_.find(id) != resources_.end();
    }

    /// <summary>
    /// 逆引き
    /// </summary>
    uint32_t GetIDByString(const std::string& key) const {
        auto it = str_to_id_.find(key);
        if (it != str_to_id_.end()) return it->second;
        return 0;
    }
    std::string GetStringByID(uint32_t id) const {
        auto it = id_to_str_.find(id);
        if (it != id_to_str_.end()) return it->second;
        return "";
    }

private:

    std::unordered_map<uint32_t, ResourcePtr> resources_;
    std::unordered_map<std::string, uint32_t> str_to_id_;
    std::unordered_map<uint32_t, std::string> id_to_str_;
};