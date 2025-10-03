#pragma once

#include <d3dx12.h>
#include <wrl.h>
#include <vector>
#include <string>

#include "../Interface/IResource.h"
#include "Math/MyMath.h"

struct MeshVertex {
    Vector4 position;
    Vector2 texCoord;
    Vector3 normal;
};
struct MeshNode {
    QuaternionTransform transform;
    Matrix4x4 localMatrix;
    std::string name;
    std::vector<MeshNode> Children;
};
struct MeshInfo {
    std::vector<MeshVertex> vertices;
    std::vector<uint32_t> indices;
    MeshNode rootNode;
};

/* メッシュ用の実体クラス */
class MeshResource : public IResource {

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MeshResource() override { Release(); }

    /// <summary>
    /// 解放処理
    /// </summary>
    void Release() override {
        vertexBuffer_.Reset();
        indexBuffer_.Reset();
        meshInfo_ = {};
        vertexCount_ = 0;
        indexCount_ = 0;
        vbView_ = {};
        ibView_ = {};
        fullPath_.clear();
    }

#pragma region Accessors

    // 頂点バッファ取得
    ID3D12Resource* GetVertexBuffer() const { return vertexBuffer_.Get(); }
    Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBufferComPtr() const { return vertexBuffer_; }
    void SetVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> vb, UINT stride, UINT count) {
        vertexBuffer_ = vb;
        vertexStride_ = stride;
        vertexCount_ = count;
        vbView_.BufferLocation = vb->GetGPUVirtualAddress();
        vbView_.StrideInBytes = stride;
        vbView_.SizeInBytes = stride * count;
    }

    // インデックスバッファ取得
    ID3D12Resource* GetIndexBuffer() const { return indexBuffer_.Get(); }
    Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBufferComPtr() const { return indexBuffer_; }
    void SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> ib, DXGI_FORMAT format, UINT count) {
        indexBuffer_ = ib;
        indexFormat_ = format;
        indexCount_ = count;
        ibView_.BufferLocation = ib->GetGPUVirtualAddress();
        ibView_.Format = format;
        ibView_.SizeInBytes = (format == DXGI_FORMAT_R16_UINT ? 2 : 4) * count;
    }

    // Meshデータ
    const MeshInfo GetMeshInfo() const { return meshInfo_; }
    void SetMeshInfo(const MeshInfo& info) { meshInfo_ = info;}

    // VBV/IBV 取得
    const D3D12_VERTEX_BUFFER_VIEW& GetVBView() const { return vbView_; }
    const D3D12_INDEX_BUFFER_VIEW& GetIBView() const { return ibView_; }

    // 頂点数/インデックス数
    UINT GetVertexCount() const { return vertexCount_; }
    UINT GetIndexCount() const { return indexCount_; }

    // フルパス
    const std::string& GetFullPath() const { return fullPath_; }
    void SetFullPath(const std::string& path) { fullPath_ = path; }

#pragma endregion

private:

    // GPU リソース
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
    Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_;

    // VBV / IBV
    D3D12_VERTEX_BUFFER_VIEW vbView_{};
    D3D12_INDEX_BUFFER_VIEW ibView_{};

    // 情報
    MeshInfo meshInfo_;
    UINT vertexCount_ = 0;
    UINT vertexStride_ = 0;
    UINT indexCount_ = 0;
    DXGI_FORMAT indexFormat_ = DXGI_FORMAT_R32_UINT; // デフォルトは32bit

    // ロード元のフルパス（管理用）
    std::string fullPath_;

};