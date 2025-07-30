#pragma once
#include "../IBaseResource.h"
#include <vector>
#include <DirectXMath.h>
#include <stdexcept>


struct Vertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 uv;
};

class MeshResource : public IBaseResource {

public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    MeshResource() = default;

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~MeshResource() = default;

    /// <summary>
    /// 生成
    /// </summary>
    bool Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

    /// <summary>
    /// 
    /// </summary>
    void CreateDescriptors() override{}; // 空実装

#pragma region 

    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle() const override { return {}; }

    const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const { return vertexBufferView_; }
    const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const { return indexBufferView_; }

    UINT GetIndexCount() const { return indexCount_; }

#pragma endregion

private:

    // 頂点バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

    // インデックスバッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_;
    D3D12_INDEX_BUFFER_VIEW indexBufferView_;
    UINT indexCount_ = 0;
};