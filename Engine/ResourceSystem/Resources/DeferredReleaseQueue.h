#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <queue>
#include <cstdint>


/// <summary>
/// GPU が使い終わるまで安全にリソースを解放するためのキュー
/// </summary>
class DeferredReleaseQueue {
public:
    struct Item {
        Microsoft::WRL::ComPtr<ID3D12Resource> resource;
        uint64_t fenceValue; // 解放できるタイミング
    };

    /// <summary>
    /// リソースをキューに追加する（GPU が使い終わるまで保持）
    /// </summary>
    void Enqueue(Microsoft::WRL::ComPtr<ID3D12Resource> resource, uint64_t fenceValue) {
        items_.push({ resource, fenceValue });
    }

    /// <summary>
    /// フェンスを監視し、解放可能なリソースを順次削除する
    /// </summary>
    void Process(ID3D12Fence* fence) {
        uint64_t completed = fence->GetCompletedValue();
        while (!items_.empty() && items_.front().fenceValue <= completed) {
            items_.pop(); // ComPtr が参照カウントを下げるので自動解放
        }
    }

private:
    std::queue<Item> items_;
};