#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>


/// <summary>
/// ディスクリプタヒープから SRV/CBV/UAV を割り当てるクラス
/// </summary>
class DescriptorAllocator {

public:
    DescriptorAllocator() = default;

    /// <summary>
    /// ヒープを初期化
    /// </summary>
    void Init(uint32_t numDescriptors);

    /// <summary>
    /// ディスクリプタを1つ確保する（連番で確保）
    /// </summary>
    D3D12_CPU_DESCRIPTOR_HANDLE AllocateCPU();
    
    /// <summary>
    /// GPU ハンドルを取得する
    /// </summary>
    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t index) const;

    /// <summary>
    /// ヒープ本体を返す
    /// </summary>
    ID3D12DescriptorHeap* GetHeap() const { return heap_.Get(); }

    /// <summary>
    /// 確保済み数を返す
    /// </summary>
    uint32_t GetAllocatedCount() const { return offset_; }

private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;
    uint32_t descriptorSize_ = 0;
    uint32_t capacity_ = 0;
    uint32_t offset_ = 0; // 現在の確保位置
};