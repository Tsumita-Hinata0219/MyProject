#include "DescriptorAllocator.h"
#include "Core/DirectXManager/DirectXManager.h"


void DescriptorAllocator::Init(uint32_t numDescriptors) 
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = numDescriptors;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    auto device = DirectXManager::GetInstance()->GetDevice();
    device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap_));
    descriptorSize_ = device->GetDescriptorHandleIncrementSize(desc.Type);
    capacity_ = numDescriptors;
    offset_ = 0;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::AllocateCPU()
{
    if (offset_ >= capacity_) {
        throw std::runtime_error("Descriptor heap exhausted!");
    }
    D3D12_CPU_DESCRIPTOR_HANDLE handle = heap_->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += static_cast<SIZE_T>(offset_) * descriptorSize_;
    offset_++;
    return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetGPUHandle(uint32_t index) const
{
    D3D12_GPU_DESCRIPTOR_HANDLE handle = heap_->GetGPUDescriptorHandleForHeapStart();
    handle.ptr += static_cast<SIZE_T>(index) * descriptorSize_;
    return handle;
}

