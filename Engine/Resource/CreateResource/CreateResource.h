#pragma once
#include "Core/DirectXManager/DirectXManager.h"
#include "Math/MyMath.h"
#include "Math/Struct.h"


struct ResourcePeroperty {
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
	// 頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW IndexBufferView;
	// 頂点リソース用のヒープ設定
	ComPtr<ID3D12Resource> Vertex;
	// 
	ComPtr<ID3D12Resource> Index;
	// 
	ComPtr<ID3D12Resource> TransformationMatrix;
	// 
	ComPtr<ID3D12Resource> Material;
	// 
	ComPtr<ID3D12Resource> Lighting;
	// 
	ComPtr<ID3D12Resource> instancing;
	//
	ComPtr<ID3D12Resource> Well;
};

class CreateResource {

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static CreateResource* GetInstance();

	/// <summary>
	/// 頂点バッファリソースを作成する
	/// </summary>
	static ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
	static void CreateBufferResource(size_t sizeInBytes, ComPtr<ID3D12Resource>& resource);

	/// <summary>
	/// 頂点バッファビューを作成する
	/// </summary>
	static D3D12_VERTEX_BUFFER_VIEW CreateVertexBufferView(size_t sizeInbyte, ID3D12Resource* resource, int size);
	static D3D12_INDEX_BUFFER_VIEW CreateIndexBufferview(size_t sizeInbyte, ID3D12Resource* resource);

	/// <summary>
	/// Rendertexture生成関数
	/// </summary>
	static ComPtr<ID3D12Resource> CreateRenderTextureResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& color);

private:

	void TransfarImage(ComPtr<ID3D12Resource> resource);

};

