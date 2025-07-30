#pragma once

#include "DirectXTex.h"
#include "Math/MyMath.h"
#include "Core/DirectXManager/DirectXManager.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "Resource/CreateResource/CreateResource.h"


struct DescriptorSize {
	uint32_t SRV;
	uint32_t RTV;
	uint32_t DSV;
};
struct DescriptorSRVHandle {
	D3D12_CPU_DESCRIPTOR_HANDLE _CPU;
	D3D12_GPU_DESCRIPTOR_HANDLE _GPU;
};

struct Skeleton;

/* DescriptorManagerクラス */
class DescriptorManager {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DescriptorManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DescriptorManager() = default;

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static DescriptorManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Init();

	/// <summary>
	/// Descriptorがindex数を超えていないか
	/// </summary>
	static void BeginFrame();

	/// <summary>
	/// indexをクリアにする
	/// </summary>
	static void Clear();

	///// <summary>
	///// CVBを作成する
	///// </summary>
	//void CreateCBV(Microsoft::WRL::ComPtr<ID3D12Resource>* resource, UINT size, uint32_t count);

	/// <summary>
	/// SRVを作成する
	/// </summary>
	uint32_t CreateInstancingSRV(uint32_t instancingNum, Microsoft::WRL::ComPtr<ID3D12Resource>& resource, UINT size);
	uint32_t CreateTextureSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource, const DirectX::TexMetadata& metadata);
	uint32_t CreateRenderTextureDepthSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource);
	uint32_t CreatePostEffectSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource);
	uint32_t CreateSkinClusterSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource, const Skeleton& skeleton);

	/// <summary>
	/// UAVを作成する
	/// </summary>
	uint32_t CreateUAV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource, UINT size, uint32_t count);

	/// <summary>
	/// DescriptorTableを設定する
	/// </summary>
	void SetGraphicsRootDescriptorTable(UINT rootPatramerterIndex, uint32_t texHandle);


#pragma region Get

	/// <summary>
	/// DescriptorHandleの取得
	/// </summary>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(
		ComPtr<ID3D12DescriptorHeap> descriptorHeap,
		uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(
		ComPtr<ID3D12DescriptorHeap> descriptorHeap,
		uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// indexの取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetIndex() { return DescriptorManager::GetInstance()->index_; }

	/// <summary>
	/// descriptorSizeの取得
	/// </summary>
	DescriptorSize GetDescriptorSize() { return DescriptorManager::GetInstance()->descriptorSize_; }

	/// <summary>
	/// GPUハンドルの取得
	/// </summary>
	DescriptorSRVHandle GetSRVHandle(uint32_t handle) { return this->srvHandle_[handle]; }

#pragma endregion 


#pragma region Set

	/// <summary>
	/// srvHandle_._CPUの設定
	/// </summary>
	void SetDescriptorHandle_CPU(D3D12_CPU_DESCRIPTOR_HANDLE CPU, uint32_t index) {
		DescriptorManager::GetInstance()->srvHandle_[index]._CPU = CPU;
	}

	/// <summary>
	/// srvHandle_._GPUの設定
	/// </summary>
	void SetDescriptorHandle_GPU(D3D12_GPU_DESCRIPTOR_HANDLE GPU, uint32_t index) {
		DescriptorManager::GetInstance()->srvHandle_[index]._GPU = GPU;
	}

#pragma endregion 


private:

	/// <summary>
	/// SRVを作成するDescriptorHeapの場所を決める
	/// </summary>
	void AssignSRVHandles();

	/// <summary>
	/// CPUとGPUの.ptrをずらす
	/// </summary>
	void ShiftSRVHandlePtr();

	/// <summary>
	/// indexをインクリメント
	/// </summary>
	void IncrementIndex();

	/// <summary>
	/// SRVの生成
	/// </summary>
	void CreateSRV(ComPtr<ID3D12Resource> resource, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, uint32_t index);

	/// <summary>
	/// UAVの生成
	/// </summary>
	void RegisterUAV(ComPtr<ID3D12Resource> resource, D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, uint32_t index);

private:

	// 最大数
	static const uint32_t descriptor_Max = 64000;
	// サイズ
	DescriptorSize descriptorSize_{};
	// SRV
	DescriptorSRVHandle srvHandle_[descriptor_Max]{};
	// 配列のIndex
	uint32_t index_;
};

