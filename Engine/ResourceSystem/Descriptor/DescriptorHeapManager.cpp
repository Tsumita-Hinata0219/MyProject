#include "DescriptorHeapManager.h"
#include "Core/DirectXManager/DirectXManager.h"
#include <cassert>

DescriptorHeapManager::DescriptorHeapManager()
{
    dxMgr_ = DirectXManager::GetInstance();
}

void DescriptorHeapManager::Init()
{
    auto device = dxMgr_->GetDevice();

    // ========== CBV / SRV / UAV Heap ==========
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.NumDescriptors = kMaxSRVDescriptors_; // 最大数を設定
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    HRESULT hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&cbvSrvUavHeap_));
    assert(SUCCEEDED(hr));

    cbvSrvUavDescriptorSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    currentIndex_ = 0;
    srvHandles_.resize(kMaxSRVDescriptors_);
}

void DescriptorHeapManager::BeginFrame()
{
    // コマンドリストに DescriptorHeap をセットする処理を入れる予定
    // CommandList->SetDescriptorHeaps(1, cbvSrvUavHeap_.GetAddressOf());
}

void DescriptorHeapManager::EndFrame()
{
    // 今は特にやることなし（必要ならフレーム終わりに整理処理など）
}

void DescriptorHeapManager::Clear()
{
    currentIndex_ = 0;
}

uint32_t DescriptorHeapManager::CreateTextureSRV(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::TexMetadata& metadata)
{
    assert(texture);
    assert(currentIndex_ < kMaxSRVDescriptors_ && "DescriptorHeap overflow!");

    auto device = dxMgr_->GetDevice();

    // CPU/GPUハンドルを計算
    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetCPUHandle(currentIndex_);
    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = GetGPUHandle(currentIndex_);

    // SRV設定
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = metadata.format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);

    device->CreateShaderResourceView(texture.Get(), &srvDesc, cpuHandle);

    // ハンドルを保存
    srvHandles_[currentIndex_] = { cpuHandle, gpuHandle };

    return currentIndex_++;
}

const DescriptorHandle& DescriptorHeapManager::GetSRVHandle(uint32_t index) const
{
    assert(index < currentIndex_ && "Invalid SRV index!");
    return srvHandles_[index];
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetCPUHandle(UINT index) const
{
    D3D12_CPU_DESCRIPTOR_HANDLE handle = cbvSrvUavHeap_->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += static_cast<SIZE_T>(index) * cbvSrvUavDescriptorSize_;
    return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetGPUHandle(UINT index) const
{
    D3D12_GPU_DESCRIPTOR_HANDLE handle = cbvSrvUavHeap_->GetGPUDescriptorHandleForHeapStart();
    handle.ptr += static_cast<SIZE_T>(index) * cbvSrvUavDescriptorSize_;
    return handle;

}
