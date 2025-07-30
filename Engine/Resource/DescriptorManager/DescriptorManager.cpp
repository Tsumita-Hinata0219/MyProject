#include "DescriptorManager.h"
#include "Math/Struct.h"


/// <summary>
/// インスタンスの取得
/// </summary>
DescriptorManager* DescriptorManager::GetInstance()
{
	static DescriptorManager instance;
	return &instance;
}


/// <summary>
/// 初期化処理
/// </summary>
void DescriptorManager::Init()
{
	// DescriptorManagerのインスタンス取得
	DescriptorManager* thisClass = DescriptorManager::GetInstance();
	// DirectXCommonのインスタンス取得
	DirectXManager* dxCommon = DirectXManager::GetInstance();

	thisClass->descriptorSize_.SRV =
		dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	thisClass->descriptorSize_.RTV =
		dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	thisClass->descriptorSize_.DSV =
		dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	thisClass->index_ = 0;
}


/// <summary>
/// Descriptorがindex数を超えていないか
/// </summary>
void DescriptorManager::BeginFrame()
{
	if (DescriptorManager::GetInstance()->index_ >= descriptor_Max) {

		Log("The number of DescripterIndexes has exceeded the limit.");
		assert(0);
	}
}


/// <summary>
/// indexをクリアにする
/// </summary>
void DescriptorManager::Clear()
{
	DescriptorManager::GetInstance()->index_ = 0;
}

//
///// <summary>
///// CVBを作成する
///// </summary>
//void DescriptorManager::CreateCBV(Microsoft::WRL::ComPtr<ID3D12Resource>* resource, UINT size, uint32_t count)
//{
//	// Resource用のHeap設定
//	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
//	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う
//
//	// BufferResource。Textureの場合はまた別の設定をする
//	D3D12_RESOURCE_DESC resourceDesc{};
//	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//
//	// Resourceのサイズ
//	resourceDesc.Width = uint64_t(size) * count;
//		
//	// Bufferの場合はこれらは1にする決まり
//	resourceDesc.Height = 1;
//	resourceDesc.DepthOrArraySize = 1;
//	resourceDesc.MipLevels = 1;
//	resourceDesc.SampleDesc.Count = 1;
//
//	// Bufferの場合はこれにする決まり
//	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	// 実際にBufferResourceを作る
//	HRESULT hr_;
//	hr_ = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
//		&uploadHeapProperties_, D3D12_HEAP_FLAG_NONE,
//		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
//		IID_PPV_ARGS(&resource));
//	assert(SUCCEEDED(hr_));
//	if (FAILED(hr_)) {
//		// エラーハンドリング
//		std::cerr << "CreateCommittedResource failed: " << std::hex << hr_ << std::endl;
//	}
//}


/// <summary>
/// SRVを作成する
/// </summary>
uint32_t DescriptorManager::CreateInstancingSRV(uint32_t instancingNum, Microsoft::WRL::ComPtr<ID3D12Resource>& resource, UINT size)
{
	// indexをインクリメント
	index_++;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = instancingNum;
	srvDesc.Buffer.StructureByteStride = size;

	// SRVを作成するDescriptorHeapの場所を決める
	AssignSRVHandles();
	// CPUとGPUの.ptrをずらす
	ShiftSRVHandlePtr();
	// SRVの生成
	CreateSRV(resource, srvDesc, index_);

	return index_;
}
uint32_t DescriptorManager::CreateTextureSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource, const DirectX::TexMetadata& metadata)
{
	// indexをインクリメント
	index_++;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	// CubeMapかどうかで分岐
	if (metadata.IsCubemap()) {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0; // unionがTextureCubeになったが、内部パラメータの意味はTexture2Dと変わらない
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	else {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
		srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	}

	// SRVを作成するDescriptorHeapの場所を決める
	AssignSRVHandles();
	// CPUとGPUの.ptrをずらす
	ShiftSRVHandlePtr();
	// SRVの生成
	CreateSRV(resource, srvDesc, index_);

	return index_;
}
uint32_t DescriptorManager::CreateRenderTextureDepthSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource)
{
	// indexをインクリメント
	index_++;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// SRVを作成するDescriptorHeapの場所を決める
	AssignSRVHandles();
	// CPUとGPUの.ptrをずらす
	ShiftSRVHandlePtr();
	// SRVの生成
	CreateSRV(resource, srvDesc, index_);

	return index_;
}
uint32_t DescriptorManager::CreatePostEffectSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource)
{
	// indexをインクリメント
	index_++;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// SRVを作成するDescriptorHeapの場所を決める
	AssignSRVHandles();
	// CPUとGPUの.ptrをずらす
	ShiftSRVHandlePtr();
	// SRVの生成
	CreateSRV(resource, srvDesc, index_);

	return index_;
}
uint32_t DescriptorManager::CreateSkinClusterSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource, const Skeleton& skeleton)
{
	// indexをインクリメント
	index_++;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
	srvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);

	// SRVを作成するDescriptorHeapの場所を決める
	AssignSRVHandles();
	// CPUとGPUの.ptrをずらす
	ShiftSRVHandlePtr();
	// SRVの生成
	CreateSRV(resource, srvDesc, index_);

	return index_;
}

/// <summary>
/// UAVを作成する
/// </summary>
uint32_t DescriptorManager::CreateUAV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource, UINT size, uint32_t count)
{
	// indexをインクリメント
	index_++;

	// UAVのデスク
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = count;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.StructureByteStride = size;

	// UAVを作成するDescriptorHeapの場所を決める
	AssignSRVHandles();
	// CPUとGPUの.ptrをずらす
	ShiftSRVHandlePtr();
	// UAVの生成
	RegisterUAV(resource, uavDesc, index_);

	return index_;
}


///// <summary>
///// UAVを作る
///// </summary>
//void DescriptorManager::CreateUAV(Microsoft::WRL::ComPtr<ID3D12Resource>* resource, UINT size, uint32_t count)
//{
//	// Resource用のHeap設定
//	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
//	//uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う
//	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_DEFAULT; // DefaultHeapを使う
//
//	// BufferResource。Textureの場合はまた別の設定をする
//	D3D12_RESOURCE_DESC resourceDesc{};
//	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//
//	// 専用のフラグを立てる
//	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
//
//	// Resourceのサイズ
//	resourceDesc.Width = uint64_t(size) * count;
//
//	// Bufferの場合はこれらは1にする決まり
//	resourceDesc.Height = 1;
//	resourceDesc.DepthOrArraySize = 1;
//	resourceDesc.MipLevels = 1;
//	resourceDesc.SampleDesc.Count = 1;
//
//	// Bufferの場合はこれにする決まり
//	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	// 実際にBufferResourceを作る
//	HRESULT hr_;
//	/*hr_ = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
//		&uploadHeapProperties_, D3D12_HEAP_FLAG_NONE,
//		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
//		IID_PPV_ARGS(&resource));*/
//
//	hr_ = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
//		&uploadHeapProperties_, D3D12_HEAP_FLAG_NONE,
//		&resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr,
//		IID_PPV_ARGS(&resource));
//
//	assert(SUCCEEDED(hr_));
//	if (FAILED(hr_)) {
//		// エラーハンドリング
//		std::cerr << "CreateCommittedResource failed: " << std::hex << hr_ << std::endl;
//	}
//}


/// <summary>
/// DescriptorTableを設定する
/// </summary>
void DescriptorManager::SetGraphicsRootDescriptorTable(UINT rootPatramerterIndex, uint32_t texHandle)
{
	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	ID3D12DescriptorHeap* desc[] = { DirectXManager::GetInstance()->GetSrvDescriptorHeap() };
	commands.List->SetDescriptorHeaps(1, desc);
	commands.List->SetGraphicsRootDescriptorTable(
		rootPatramerterIndex,
		srvHandle_[texHandle]._GPU);
}


/// <summary>
/// DescriptorHandleの取得
/// </summary>
D3D12_CPU_DESCRIPTOR_HANDLE DescriptorManager::GetCPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleCPU;
}
D3D12_GPU_DESCRIPTOR_HANDLE DescriptorManager::GetGPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleGPU;
}


/// <summary>
/// SRVを作成するDescriptorHeapの場所を決める
/// </summary>
void DescriptorManager::AssignSRVHandles()
{
	// DirectXCommonのインスタンス取得
	DirectXManager* dxCommon = DirectXManager::GetInstance();

	srvHandle_[index_]._CPU = GetCPUDescriptorHandle(
		dxCommon->GetSrvDescriptorHeap(),
		descriptorSize_.SRV,
		index_);

	srvHandle_[index_]._GPU = GetGPUDescriptorHandle(
		dxCommon->GetSrvDescriptorHeap(),
		descriptorSize_.SRV,
		index_);
}


/// <summary>
/// CPUとGPUの.ptrをずらす
/// </summary>
void DescriptorManager::ShiftSRVHandlePtr()
{
	// DirectXCommonのインスタンス取得
	DirectXManager* dxCommon = DirectXManager::GetInstance();

	srvHandle_[index_]._CPU.ptr +=
		dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	srvHandle_[index_]._GPU.ptr +=
		dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}


/// <summary>
/// indexをインクリメント
/// </summary>
void DescriptorManager::IncrementIndex()
{
	index_++;
}


/// <summary>
/// SRVの生成
/// </summary>
void DescriptorManager::CreateSRV(ComPtr<ID3D12Resource> resource, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, uint32_t index)
{
	// DirectXCommonのインスタンス取得
	DirectXManager* dxCommon = DirectXManager::GetInstance();

	dxCommon->GetDevice()->CreateShaderResourceView(
		resource.Get(),
		&srvDesc,
		srvHandle_[index]._CPU);
}

/// <summary>
/// UAVの生成
/// </summary>
void DescriptorManager::RegisterUAV(ComPtr<ID3D12Resource> resource, D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc, uint32_t index)
{
	// DirectXCommonのインスタンス取得
	DirectXManager* dxCommon = DirectXManager::GetInstance();

	dxCommon->GetDevice()->CreateUnorderedAccessView(
		resource.Get(),
		nullptr,
		&uavDesc,
		srvHandle_[index]._CPU);
}

