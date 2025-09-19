#pragma once

#include "DirectXTex.h"
#include <wrl.h>
#include <vector>
#include <cassert>
#include "DirectXTex.h"
#include "Math/MyMath.h"

// 前方宣言
class DirectXManager;
class CommandManager;

/* CPU/GPUハンドルをまとめて保持する構造体 */
struct DescriptorHandle {
	D3D12_CPU_DESCRIPTOR_HANDLE cpu{};
	D3D12_GPU_DESCRIPTOR_HANDLE gpu{};
};

/* ディスクリプター管理クラス */
class DescriptorHeapManager {

private: // シングルトン

	DescriptorHeapManager();
	~DescriptorHeapManager() = default;
	DescriptorHeapManager(const DescriptorHeapManager&) = delete;
	const DescriptorHeapManager& operator=(const DescriptorHeapManager&) = delete;

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static DescriptorHeapManager* GetInstance() {
		static DescriptorHeapManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// フレーム開始時処理
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// フレーム終了時処理
	/// </summary>
	void EndFrame();

	/// <summary>
	/// indexをクリア
	/// </summary>
	void Clear();

	/// <summary>
	/// SRVの作成
	/// </summary>
	uint32_t CreateTextureSRV(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::TexMetadata& metadata);

#pragma region accessor

	// Handleを返す
	const DescriptorHandle& GetSRVHandle(uint32_t index) const;

#pragma endregion 

private:

	/// <summary>
	/// ハンドルの取得
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT index) const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(UINT index) const;

private:

	DirectXManager* dxMgr_ = nullptr;
	CommandManager* comdMgr_ = nullptr;

	// CBV / SRV / UAV 用
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cbvSrvUavHeap_;

	// 最大数
	static constexpr UINT kMaxSRVDescriptors_ = 1024;
	// Descriptor サイズやインクリメント値
	UINT cbvSrvUavDescriptorSize_ = 0;
	// 新しいSRVを作るたびに増やす
	UINT currentIndex_ = 0;

	// SRVハンドルを保持
	std::vector<DescriptorHandle> srvHandles_;

};