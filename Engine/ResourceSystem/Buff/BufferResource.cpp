#include "BufferResource.h"
#include "Core/DirectXManager/DirectXManager.h"

void BufferResource::Create(size_t size, D3D12_HEAP_TYPE heapType)
{
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = heapType;
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    auto device = DirectXManager::GetInstance()->GetDevice();
    if (FAILED(device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        heapType == D3D12_HEAP_TYPE_UPLOAD ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(&resource_))))
    {
        throw std::runtime_error("Failed to create buffer resource");
    }

    size_ = size;
}

void BufferResource::Update(const void* data, size_t size)
{
    if (!resource_) return;
    void* mapped = nullptr;
    D3D12_RANGE range = { 0, 0 }; // 読み取りはしない
    resource_->Map(0, &range, &mapped);
    memcpy(mapped, data, size);
    resource_->Unmap(0, nullptr);
}
