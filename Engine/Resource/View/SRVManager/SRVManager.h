#pragma once

#include <map>

#include "DirectXTex.h"
#include "Math/Struct.h"
#include "Core/DirectXManager/DirectXManager.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "Resource/DescriptorManager/DescriptorManager.h"
#include "Resource/CreateResource/CreateResource.h"

#define SRV_DESCRIPTOR_MAX 640


// SRVHandle
struct SRVDataHandle {
	D3D12_CPU_DESCRIPTOR_HANDLE _CPU;
	D3D12_GPU_DESCRIPTOR_HANDLE _GPU;
};


/* SRVManagerクラス */
class SRVManager {

public: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	SRVManager() = default;
	~SRVManager() = default;
	SRVManager(const SRVManager&) = delete;
	const SRVManager& operator=(const SRVManager&) = delete;


public: // メンバ関数

	// インスタンスの取得
	static SRVManager* GetInstance() {
		static SRVManager instance;
		return &instance;
	}

	// 初期化処理
	static void Initialize();

	// フレーム開始時に通る処理
	static void BeginFrame();

	// ImGui描画
	static void DrawImGui();

	// クリア処理
	static void Clear();

	// SRVを作成する
	static uint32_t CreateInstancingSRV(uint32_t instancingNum, Microsoft::WRL::ComPtr<ID3D12Resource>& resource, UINT size);
	static uint32_t CreateTextureSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource, const DirectX::TexMetadata& metadata);
	static uint32_t CreatePostEffectSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource);
	static uint32_t CreateSkinClusterSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource, const Skeleton& skeleton);

	// DescriptorTableの設定
	static void SetGraphicsRootDescriptorTable(UINT rootPatramerterIndex, uint32_t index);

#pragma region Accessor アクセッサ

	// DescriptorHandleの取得
	static D3D12_CPU_DESCRIPTOR_HANDLE Get_CPUDescriptorHandle(
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap, uint32_t index);
	static D3D12_GPU_DESCRIPTOR_HANDLE Get_GPUDescriptorHandle(
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap, uint32_t index);

	// 空きインデックスの取得
	static uint32_t GetEmptyIndex() {
		uint32_t index = SRVManager::GetInstance()->index_;
		return index++; // 空きインデックスなのでインクリメントして返す
	}	

#pragma endregion


private:

	// CPUとGPUの.ptrをずらす
	void ShiftSRVHandlePtr();

	// SRVの生成
	void CreateSRV(Microsoft::WRL::ComPtr<ID3D12Resource> resource, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, uint32_t index);

private: // メンバ変数

	//SRVDataHandle handle_[SRV_DESCRIPTOR_MAX]{};

	// HandleMap
	std::map<uint32_t, SRVDataHandle> handleMap_;

	// DescriptorSize
	uint32_t descriptorSize_ = 0;

	// Index
	uint32_t index_ = 0;
};

