#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <cassert>
#include <map>
#include <utility>
#include<vector>

#include "Math/MyMath.h"
#include "TextureResource.h"

/* テクスチャリソース管理クラス */
class TexManager {

private: // シングルトン

    TexManager() = default;
    ~TexManager() = default;
    TexManager(const TexManager&) = delete;
    const TexManager& operator=(const TexManager&) = delete;

public:

    /// <summary>
    /// インスタンスの取得
    /// </summary>
    static TexManager* GetInstance() {
        static TexManager instance;
        return &instance;
    }

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Init() {};

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() {};

    /// <summary>
    /// 登録（文字列キー）
    /// </summary>
    void Register(const std::string& key, std::unique_ptr<TexResource> resource) {
        texMap_.Register(key, std::move(resource));
    }

    /// <summary>
    /// 文字列キーで取得
    /// </summary>
    TexResource* GetByString(const std::string& key) {
        return texMap_.GetByString(key);
    }

    /// <summary>
    /// IDで取得
    /// </summary>
    TexResource* GetByID(uint32_t id) {
        return texMap_.GetByID(id);
    }

    /// <summary>
    /// 存在確認
    /// </summary>
    bool Exists(const std::string& key) const {
        return texMap_.Exists(key);
    }
    bool Exists(uint32_t id) const {
        return texMap_.Exists(id);
    }

    /// <summary>
    /// 逆引き
    /// </summary>
    uint32_t GetIDByString(const std::string& key) const {
        return texMap_.GetIDByString(key);
    }
    std::string GetStringByID(uint32_t id) const {
        return texMap_.GetStringByID(id);
    }

private:
    IDMap<TexResource> texMap_; 
};