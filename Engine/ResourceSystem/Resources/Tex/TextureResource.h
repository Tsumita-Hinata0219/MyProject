#pragma once
#include "../IBaseResource.h"


class TextureResource : public IBaseResource {

public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    TextureResource() = default;

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~TextureResource() = default;

    /// <summary>
    /// 生成
    /// </summary>
    bool Create(ID3D12Device* device, const std::wstring& textureFilePath);

    /// <summary>
    /// 
    /// </summary>
    void CreateDescriptors() override;

#pragma region 

    D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle() const override;

#pragma endregion


private:
    D3D12_GPU_DESCRIPTOR_HANDLE gpuDescriptorHandle_{};
};

