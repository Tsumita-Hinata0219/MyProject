#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <cstdint>
#include <stdexcept>

/// <summary>
/// バッファリソースをラップするクラス
/// CBV / VBV / IBV / UAV をまとめて管理できる
/// </summary>
class BufferResource {

public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    BufferResource() = default;

    /// <summary>
    /// バッファを作成
    /// </summary>
    void Create(size_t size, D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_UPLOAD);

    /// <summary>
    /// データを書き込む（Upload Heap 前提）
    /// </summary>
    void Update(const void* data, size_t size);

    /// <summary>
    /// GPU リソースを取得
    /// </summary>
    ID3D12Resource* GetResource() const { return resource_.Get(); }

    /// <summary>
    /// オブジェクトのサイズを取得
    /// </summary>
    size_t GetSize() const { return size_; }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    size_t size_ = 0;
};