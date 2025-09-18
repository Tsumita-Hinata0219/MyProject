#pragma once

#include <d3dx12.h>

#include "../Interface/IResource.h"
#include "Math/MyMath.h"

/* テクスチャ用の実体クラス */
class TexResource : public IResource {
	
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TexResource() override { Release(); }

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release() override {
		gpuResource_.Reset();
		descriptorIndex_ = 0;
		size_  = {};
	}

#pragma region accessor

	// GPUリソースの参照を取得
	ID3D12Resource* GetResource() const { return gpuResource_.Get(); }
	Microsoft::WRL::ComPtr<ID3D12Resource> GetResourceComPtr() const { return gpuResource_; }
	// GPUリソースを設定
	void SetResource(Microsoft::WRL::ComPtr<ID3D12Resource> resource) { gpuResource_ = resource; }

	// ディスクリプタインデックスを取得/設定
	uint32_t GetDescriptorIndex() const { return descriptorIndex_; }
	void SetDescriptorIndex(uint32_t index) { descriptorIndex_ = index; }

	// サイズを取得/設定
	const Vector2& GetSize() const { return size_; }
	void SetSize(const Vector2& size) { size_ = size; }

#pragma endregion

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> gpuResource_;
	uint32_t descriptorIndex_ = 0;
	Vector2 size_{};
};