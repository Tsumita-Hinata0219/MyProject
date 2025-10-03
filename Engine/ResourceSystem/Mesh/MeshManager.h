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
#include "MeshResource.h"

/* メッシュリソース管理クラス */
class MeshManager {

private: // シングルトン

    MeshManager() = default;
    ~MeshManager() = default;
    MeshManager(const MeshManager&) = delete;
    const MeshManager& operator=(const MeshManager&) = delete;

public:

    /// <summary>
    /// インスタンスの取得
    /// </summary>
    static MeshManager* GetInstance() {
        static MeshManager instance;
        return &instance;
    }

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Init() {
        // 必要があればここに初期化コードを書く
    }

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() {
        // 必要があればここに解放コードを書く
    }

    /// <summary>
    /// 登録
    /// </summary>
    void Register(const std::string& key, std::unique_ptr<MeshResource> resource) {
        meshMap_.Register(key, std::move(resource));
    }

    /// <summary>
    /// 文字列キーで取得
    /// </summary>
    MeshResource* Get(const std::string& key) {
        return meshMap_.GetByString(key);
    }

    /// <summary>
    /// IDで取得
    /// </summary>
    MeshResource* Get(uint32_t id) {
        return meshMap_.GetByID(id);
    }

    /// <summary>
    /// 存在確認
    /// </summary>
    bool Exists(const std::string& key) const {
        return meshMap_.Exists(key);
    }
    bool Exists(uint32_t id) const {
        return meshMap_.Exists(id);
    }

    /// <summary>
    /// 逆引き
    /// </summary>
    uint32_t GetIDByString(const std::string& key) const {
        return meshMap_.GetIDByString(key);
    }
    std::string GetStringByID(uint32_t id) const {
        return meshMap_.GetStringByID(id);
    }

private:
    IDMap<MeshResource> meshMap_;
};