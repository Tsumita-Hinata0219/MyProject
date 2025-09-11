#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <cstdint>
#include <stdexcept>

/// <summary>
/// テクスチャリソースをラップするクラス
/// SRV / RTV / DSV をまとめて管理
/// </summary>
class TextureResource {

public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    TextureResource() = default;

    /// <summary>
    /// 2Dテクスチャを作成
    /// </summary>
    void Create(uint32_t width, uint32_t height,DXGI_FORMAT format,
        D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
        D3D12_RESOURCE_STATES initState = D3D12_RESOURCE_STATE_COMMON,
        D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT);

    /// <summary>
    /// SRV を作成
    /// </summary>
    void CreateSRV(D3D12_CPU_DESCRIPTOR_HANDLE handle);

    /// <summary>
    /// RTV を作成
    /// </summary>
    void CreateRTV(D3D12_CPU_DESCRIPTOR_HANDLE handle);

    /// <summary>
    /// DSV を作成
    /// </summary>
    void CreateDSV(D3D12_CPU_DESCRIPTOR_HANDLE handle);

    /// <summary>
    /// 取得
    /// </summary>
    ID3D12Resource* GetResource() const { return resource_.Get(); }
    uint32_t GetWidth() const { return width_; }
    uint32_t GetHeight() const { return height_; }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    DXGI_FORMAT format_ = DXGI_FORMAT_UNKNOWN;
};
