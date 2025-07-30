#pragma once
#include <wrl/client.h>
#include <d3d12.h>
#include <vector>
#include <mutex>


/* DescriptorHeapを管理するクラス */
class DescriptorHeapManager {

private: // シングルトン

	DescriptorHeapManager() = default;
	~DescriptorHeapManager() = default;
	DescriptorHeapManager(const DescriptorHeapManager&) = delete;
	DescriptorHeapManager& operator=(const DescriptorHeapManager&) = delete;

public:

	// Heapの種類
	enum class HeapType {
		CBV_SRV_UAV,
		Sampler,
	};

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static DescriptorHeapManager* GetInstance() {
		static DescriptorHeapManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(HeapType type, UINT numDescriptors);

	/// <summary>
	/// Descriptorを1つ確保しハンドルを返す
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE Allocate();

	/// <summary>
	/// Descriptorを解放
	/// </summary>
	void Free(D3D12_CPU_DESCRIPTOR_HANDLE handle);

	/// <summary>
	/// GPU用のヒープハンドルを取得
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) const;

	/// <summary>
	/// CPU用のヒープハンドルを取得
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(size_t index) const;

private:

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	HeapType heapType_;
	UINT numDescriptors_;
	UINT descriptorSize_;

	// 空きスロット管理用の単純なフリーリスト
	std::vector<UINT> freeList_;
	UINT nextFreeIndex_;

	// 排他制御用
	std::mutex mutex_;
};