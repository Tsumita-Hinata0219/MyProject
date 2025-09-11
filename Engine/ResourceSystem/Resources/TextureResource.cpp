#include "TextureResource.h"
#include "Core/DirectXManager/DirectXManager.h"


void TextureResource::Create(uint32_t width, uint32_t height, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags, D3D12_RESOURCE_STATES initState, D3D12_HEAP_TYPE heapType)
{
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = heapType;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Alignment = 0;
    desc.Width = width;
    desc.Height = height;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = format;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = flags;

    auto device = DirectXManager::GetInstance()->GetDevice();
    if (FAILED(device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        initState,
        nullptr,
        IID_PPV_ARGS(&resource_))))
    {
        throw std::runtime_error("Failed to create texture resource");
    }

    width_ = width;
    height_ = height;
    format_ = format;
}

void TextureResource::CreateSRV(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = format_;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    auto device = DirectXManager::GetInstance()->GetDevice();
    device->CreateShaderResourceView(resource_.Get(), &srvDesc, handle);
}

void TextureResource::CreateRTV(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = format_;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

    auto device = DirectXManager::GetInstance()->GetDevice();
    device->CreateRenderTargetView(resource_.Get(), &rtvDesc, handle);
}

void TextureResource::CreateDSV(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = format_;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

    auto device = DirectXManager::GetInstance()->GetDevice();
    device->CreateDepthStencilView(resource_.Get(), &dsvDesc, handle);
}
