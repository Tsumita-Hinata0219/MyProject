#include "MeshResource.h"
#include "Core/DirectXManager/DirectXManager.h"

bool MeshResource::Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    auto dxMgr = DirectXManager::GetInstance();
    // 頂点バッファ作成
    {
        const UINT vertexBufferSize = static_cast<UINT>(sizeof(Vertex) * vertices.size());

        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

        HRESULT hr = dxMgr->GetDevice()->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&vertexBuffer_));

        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create vertex buffer resource.");
        }

        // データコピー
        UINT8* pData = nullptr;
        CD3DX12_RANGE readRange(0, 0);
        hr = vertexBuffer_->Map(0, &readRange, reinterpret_cast<void**>(&pData));
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to map vertex buffer.");
        }
        memcpy(pData, vertices.data(), vertexBufferSize);
        vertexBuffer_->Unmap(0, nullptr);

        // VBV設定
        vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
        vertexBufferView_.SizeInBytes = vertexBufferSize;
        vertexBufferView_.StrideInBytes = sizeof(Vertex);
    }

    // インデックスバッファ作成
    {
        const UINT indexBufferSize = static_cast<UINT>(sizeof(uint32_t) * indices.size());

        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);

        HRESULT hr = dxMgr->GetDevice()->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&indexBuffer_));

        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create index buffer resource.");
        }

        // データコピー
        UINT8* pData = nullptr;
        CD3DX12_RANGE readRange(0, 0);
        hr = indexBuffer_->Map(0, &readRange, reinterpret_cast<void**>(&pData));
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to map index buffer.");
        }
        memcpy(pData, indices.data(), indexBufferSize);
        indexBuffer_->Unmap(0, nullptr);

        // IBV設定
        indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
        indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
        indexBufferView_.SizeInBytes = indexBufferSize;

        indexCount_ = static_cast<UINT>(indices.size());
    }

    return true;
}
