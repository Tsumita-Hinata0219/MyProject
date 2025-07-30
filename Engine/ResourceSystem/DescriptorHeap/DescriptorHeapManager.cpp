#include "DescriptorHeapManager.h"
#include "Core/DirectXManager/DirectXManager.h"


void DescriptorHeapManager::Init(HeapType type, UINT numDescriptors)
{
    heapType_ = type;
    numDescriptors_ = numDescriptors;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = numDescriptors_;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.Type = (type == HeapType::CBV_SRV_UAV) ? D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV : D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;

    auto dxMgr = DirectXManager::GetInstance();
    HRESULT hr = dxMgr->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap_));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create DescriptorHeap.");
    }

    descriptorSize_ = dxMgr->GetDevice()->GetDescriptorHandleIncrementSize(desc.Type);

    // 最初はすべて空き
    freeList_.clear();
    nextFreeIndex_ = 0;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::Allocate()
{
    UINT index = 0;
    if (!freeList_.empty()) {
        index = freeList_.back();
        freeList_.pop_back();
    }
    else {
        if (nextFreeIndex_ >= numDescriptors_) {
            throw std::runtime_error("DescriptorHeap is full.");
        }
        index = nextFreeIndex_++;
    }

    return GetCPUHandle(index);
}

void DescriptorHeapManager::Free(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    UINT start = UINT(descriptorHeap_->GetCPUDescriptorHandleForHeapStart().ptr);
    UINT ptr = static_cast<UINT>(handle.ptr);

    UINT index = (ptr - start) / descriptorSize_;
    freeList_.push_back(index);
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetGPUHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const
{
    UINT offset = static_cast<UINT>((cpuHandle.ptr - descriptorHeap_->GetCPUDescriptorHandleForHeapStart().ptr) / descriptorSize_);
    auto gpuHandle = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
    gpuHandle.ptr += offset * descriptorSize_;
    return gpuHandle;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetCPUHandle(size_t index) const
{
    auto cpuHandle = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
    cpuHandle.ptr += index * descriptorSize_;
    return cpuHandle;
}
