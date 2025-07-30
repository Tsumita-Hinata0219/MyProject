#include "SRVManager.h"



// 初期化処理
void SRVManager::Initialize()
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXManager::GetInstance()->GetDevice();
	SRVManager* instance = SRVManager::GetInstance();

	// DescriptorSizeの作成
	instance->descriptorSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}


// フレーム開始時に通る処理
void SRVManager::BeginFrame()
{
	if (SRVManager::GetInstance()->index_ >= SRV_DESCRIPTOR_MAX) {
		// ディスクリプタインデックスが最大値を超えた場合エラーログを出力
		Log("SRVManager index Overflow\n");
		// アサートでプログラムを停止
		assert(0);
	}
}


// ImGui描画
void SRVManager::DrawImGui()
{
	ImGui::Text("SRV_Index : %d", SRVManager::GetInstance()->index_ - 1); // index_は常に空きindexを指しているのでデクリメント
}


// クリア処理
void SRVManager::Clear()
{
	SRVManager::GetInstance()->index_ = 0;
}


// SRVを作成する
uint32_t SRVManager::CreateInstancingSRV(uint32_t instancingNum, Microsoft::WRL::ComPtr<ID3D12Resource>& resource, UINT size)
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXManager::GetInstance()->GetDevice();
	SRVManager* instance = SRVManager::GetInstance();

	// Indexをインクリメント
	instance->index_++;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = instancingNum;
	srvDesc.Buffer.StructureByteStride = size;

	// SRVを作成するHeapの場所を決める
	instance->handleMap_[instance->index_]._CPU = instance->Get_CPUDescriptorHandle(
		DirectXManager::GetInstance()->GetSrvDescriptorHeap(), instance->index_);
	instance->handleMap_[instance->index_]._GPU = instance->Get_GPUDescriptorHandle(
		DirectXManager::GetInstance()->GetSrvDescriptorHeap(), instance->index_);

	// CPUとGPUの.ptrをずらす
	instance->ShiftSRVHandlePtr();

	// SRVの作成
	instance->CreateSRV(resource, srvDesc, instance->index_);

	return instance->index_;
}
uint32_t SRVManager::CreateTextureSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource, const DirectX::TexMetadata& metadata)
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXManager::GetInstance()->GetDevice();
	SRVManager* instance = SRVManager::GetInstance();

	// Indexをインクリメント
	instance->index_++;

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

	// SRVを作成するHeapの場所を決める
	instance->handleMap_[instance->index_]._CPU = instance->Get_CPUDescriptorHandle(
		DirectXManager::GetInstance()->GetSrvDescriptorHeap(), instance->index_);
	instance->handleMap_[instance->index_]._GPU = instance->Get_GPUDescriptorHandle(
		DirectXManager::GetInstance()->GetSrvDescriptorHeap(), instance->index_);

	// CPUとGPUの.ptrをずらす
	instance->ShiftSRVHandlePtr();

	// SRVの作成
	instance->CreateSRV(resource, srvDesc, instance->index_);

	return instance->index_;
}
uint32_t SRVManager::CreatePostEffectSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource)
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXManager::GetInstance()->GetDevice();
	SRVManager* instance = SRVManager::GetInstance();

	// Indexをインクリメント
	instance->index_++;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// SRVを作成するHeapの場所を決める
	instance->handleMap_[instance->index_]._CPU = instance->Get_CPUDescriptorHandle(
		DirectXManager::GetInstance()->GetSrvDescriptorHeap(), instance->index_);
	instance->handleMap_[instance->index_]._GPU = instance->Get_GPUDescriptorHandle(
		DirectXManager::GetInstance()->GetSrvDescriptorHeap(), instance->index_);

	// CPUとGPUの.ptrをずらす
	instance->ShiftSRVHandlePtr();

	// SRVの作成
	instance->CreateSRV(resource, srvDesc, instance->index_);

	return instance->index_;
}
uint32_t SRVManager::CreateSkinClusterSRV(Microsoft::WRL::ComPtr<ID3D12Resource>& resource, const Skeleton& skeleton)
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXManager::GetInstance()->GetDevice();
	SRVManager* instance = SRVManager::GetInstance();

	// Indexをインクリメント
	instance->index_++;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
	srvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);

	// SRVを作成するHeapの場所を決める
	instance->handleMap_[instance->index_]._CPU = instance->Get_CPUDescriptorHandle(
		DirectXManager::GetInstance()->GetSrvDescriptorHeap(), instance->index_);
	instance->handleMap_[instance->index_]._GPU = instance->Get_GPUDescriptorHandle(
		DirectXManager::GetInstance()->GetSrvDescriptorHeap(), instance->index_);

	// CPUとGPUの.ptrをずらす
	instance->ShiftSRVHandlePtr();

	// SRVの作成
	instance->CreateSRV(resource, srvDesc, instance->index_);

	return instance->index_;
}


// DescriptorTableの設定
void SRVManager::SetGraphicsRootDescriptorTable(UINT rootPatramerterIndex, uint32_t index)
{
	Commands commands = CommandManager::GetInstance()->GetCommands();
	SRVManager* instance = SRVManager::GetInstance();

	ID3D12DescriptorHeap* desc[] = { DirectXManager::GetInstance()->GetSrvDescriptorHeap()};
	commands.List->SetDescriptorHeaps(1, desc);
	commands.List->SetGraphicsRootDescriptorTable(rootPatramerterIndex, instance->handleMap_[index]._GPU);
}


// DescriptorHandleの取得
D3D12_CPU_DESCRIPTOR_HANDLE SRVManager::Get_CPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (static_cast<unsigned long long>(SRVManager::GetInstance()->descriptorSize_ * index));
	return handle;
}
D3D12_GPU_DESCRIPTOR_HANDLE SRVManager::Get_GPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (static_cast<unsigned long long>(SRVManager::GetInstance()->descriptorSize_ * index));
	return handle;
}


// CPUとGPUの.ptrをずらす
void SRVManager::ShiftSRVHandlePtr()
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXManager::GetInstance()->GetDevice();
	SRVManager* instance = SRVManager::GetInstance();

	instance->handleMap_[instance->index_]._CPU.ptr +=
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	instance->handleMap_[instance->index_]._GPU.ptr +=
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}


// SRVの生成
void SRVManager::CreateSRV(Microsoft::WRL::ComPtr<ID3D12Resource> resource, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, uint32_t index)
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXManager::GetInstance()->GetDevice();
	SRVManager* instance = SRVManager::GetInstance();

	device->CreateShaderResourceView(resource.Get(), &srvDesc, instance->handleMap_[index]._CPU);
}

