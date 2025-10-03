#include "TextureLoader.h"
#include "Core/DirectXManager/DirectXManager.h"
#include "ResourceSystem/Descriptor/DescriptorHeapManager.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "ResourceSystem/Tex/TexManager.h"

TextureLoader::TextureLoader()
{
	texMgr_ = TexManager::GetInstance();
	dxMgr_ = DirectXManager::GetInstance();
	descHeapMgr_ = DescriptorHeapManager::GetInstance();
	comdMgr_ = CommandManager::GetInstance();

	// Deviceの取得
	Microsoft::WRL::ComPtr<ID3D12Device> device = dxMgr_->GetDevice();

	// 初期値0でFenceを作る
	HRESULT result{};
	result = device->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(result));

	// FenceのSignalを待つためのイベントを作成する
	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_ != nullptr);
}

uint32_t TextureLoader::Load(const std::string& rootPath, const std::string& fileName)
{
	// フルファイルパス
	std::string fullPath = "Resources/" + rootPath + "/" + fileName;
	// アクセスキー
	std::uint32_t key = FNV1aHash(fileName);

	// keyが既存ならreturn
	if (texMgr_->Exists(key)) {
		return;
	}

	// 読み込み処理
	// 新しく作るresource
	std::unique_ptr<TexResource> resource = std::make_unique<TexResource>();
	// 拡張子の抽出
	std::string ext = GetExtension(fileName);

	if (ext == TexFileExt::PNG.first || ext == TexFileExt::JPEG.first) {
		LoadPNGorJPEG(fullPath, resource.get());
	}
	else if (ext == TexFileExt::JPEG.first) { 
	}
	else if (ext == TexFileExt::DDS.first) {
		LoadDDS(fullPath, resource.get());
	}

	// managerに登録
	texMgr_->Register(key, std::move(resource));

	// keyを返す
	return key;
}

void TextureLoader::LoadPNGorJPEG(const std::string& path, TexResource* resource)
{
	// ========== 1. ファイル読み込み ==========
	// PNGファイルを読み込み、ScratchImage形式に変換
	auto image = LoadTextureFile(path);

	// ========== 2. MipMap生成 ==========
	// 読み込んだ画像からMipMapを生成
	auto mipImages = GenerateMipMaps(image);

	// ========== 3. メタデータ取得 ==========
	// 画像の幅・高さ・Mip数・フォーマットなどの情報
	const auto& metadata = mipImages.GetMetadata();

	// ========== 4. D3D12リソース設定作成 ==========
	// Texture作成に必要なD3D12_RESOURCE_DESC構造体を作成
	auto desc = CreateResourceDesc(metadata);

	// ========== 5. ヒーププロパティ作成 ==========
	// GPU側メモリにTextureを作成するためのHeap設定
	auto heapProps = CreateHeapProperties();

	// ========== 6. D3D12リソース作成 ==========
	// 実際にID3D12Resourceを生成
	auto d3dResource = CreateD3D12Resource(desc, heapProps);

	// ========== 7. CPU -> GPU へデータ転送 ==========
	UploadTextureData(d3dResource, mipImages);

	// ========== 8. コマンド実行 ==========
	ExeCommand();

	// ========== 9. ディスクリプタ割り当て ==========
	uint32_t descIndex = descHeapMgr_->CreateTextureSRV(d3dResource, metadata);

	// ========== 10. 各種データの設定 ==========
	resource->SetResource(d3dResource.Get());
	resource->SetDescriptorIndex(descIndex);
	resource->SetSize(Vector2{static_cast<float>(metadata.width),  static_cast<float>(metadata.height)});
	resource->SetFullPath(path);
}

void TextureLoader::LoadDDS(const std::string& path, TexResource* resource)
{
	// TODO
	path, resource;
}

DirectX::ScratchImage TextureLoader::LoadTextureFile(const std::string& path)
{
	DirectX::ScratchImage image{};
	std::wstring wPath = ConvertString(path.c_str()); // std::string -> std::wstring

	// WICベースの読み込み（PNG, JPEGなど）
	HRESULT hr = DirectX::LoadFromWICFile(wPath.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	return image;
}

DirectX::ScratchImage TextureLoader::GenerateMipMaps(const DirectX::ScratchImage& srcImage)
{
	DirectX::ScratchImage mipImages{};
	HRESULT hr = DirectX::GenerateMipMaps(
		srcImage.GetImages(),
		srcImage.GetImageCount(),
		srcImage.GetMetadata(),
		DirectX::TEX_FILTER_SRGB,
		0,
		mipImages
	);
	assert(SUCCEEDED(hr));

	return mipImages;
}

D3D12_RESOURCE_DESC TextureLoader::CreateResourceDesc(const DirectX::TexMetadata& metadata)
{
	D3D12_RESOURCE_DESC desc{};
	desc.Width = UINT(metadata.width);
	desc.Height = UINT(metadata.height);
	desc.MipLevels = UINT16(metadata.mipLevels);
	desc.DepthOrArraySize = UINT16(metadata.arraySize);
	desc.Format = metadata.format;
	desc.SampleDesc.Count = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);
	return desc;
}

D3D12_HEAP_PROPERTIES TextureLoader::CreateHeapProperties()
{
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作成
	return heapProps;
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureLoader::CreateD3D12Resource(const D3D12_RESOURCE_DESC& desc, const D3D12_HEAP_PROPERTIES& heapProps)
{
	ComPtr<ID3D12Resource> resource;
	HRESULT hr = dxMgr_->GetDevice()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&resource)
	);
	assert(SUCCEEDED(hr));

	return resource;
}

void  TextureLoader::UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource>& texture, const DirectX::ScratchImage& mipImages)
{
	// Deviceの取得
	Microsoft::WRL::ComPtr<ID3D12Device> device = dxMgr_->GetDevice();
	// Commandの取得
	auto commands = comdMgr_->GetCommands();

	// Subresource 準備
	std::vector<D3D12_SUBRESOURCE_DATA> subResources;
	DirectX::PrepareUpload(device.Get(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subResources);

	// Intermediate Resource（UploadHeap）作成
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subResources.size()));
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;

	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC bufferDesc{};
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = intermediateSize;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc.Count = 1;
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&intermediateResource)
	);
	assert(SUCCEEDED(hr));

	// データ転送
	UpdateSubresources(commands.List.Get(), texture.Get(), intermediateResource.Get(), 0, 0, UINT(subResources.size()), subResources.data());

	// ResourceBarrier で状態変更
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	commands.List->ResourceBarrier(1, &barrier);
}

void TextureLoader::ExeCommand()
{
	// Deviceの取得
	Microsoft::WRL::ComPtr<ID3D12Device> device = dxMgr_->GetDevice();

	// Commandの取得
	Commands commands = comdMgr_->GetCommands();

	// CommandListをCloseし、CommandQueue->ExecuteComandListsを使いキックする
	HRESULT result;
	result = commands.List->Close();
	assert(SUCCEEDED(result));
	ID3D12CommandList* commandLists[] = { commands.List.Get() };
	commands.Queue->ExecuteCommandLists(1, commandLists);

	// 実行を待つ
	fenceValue_++; // fenceの値を更新；
	// GPUここまでたどり着いたときに、fenceの値を指定した値に代入するようにSignalを送る
	commands.Queue->Signal(fence_.Get(), fenceValue_);
	// fenceの値が指定したSignal値にたどり着いているか確認する。GetCompletedValueの初期値はfence作成時に渡した初期値
	if (fence_->GetCompletedValue() < fenceValue_) {

		// 指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		// イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}

	// 実行が完了したので、allocatorとcommandListをResetして次のコマンドを積めるようにする
	result = commands.Allocator->Reset();
	assert(SUCCEEDED(result));
	result = commands.List->Reset(commands.Allocator.Get(), nullptr);
	assert(SUCCEEDED(result));

	comdMgr_->SetCommands(commands);
}
