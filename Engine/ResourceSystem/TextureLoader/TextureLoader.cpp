//#include "TextureLoader.h"
//
//
//void TextureLoader::Init() 
//{
//	CoInitializeEx(0, COINIT_MULTITHREADED);
//
//	// Fenceの作成
//	CreateFence();
//}
//
//void TextureLoader::Finalize() 
//{
//	CoUninitialize();
//
//	CloseHandle(fenceEvent_);
//}
//
//uint32_t TextureLoader::LoadTexture(const std::string& filePath, const std::string& FileName)
//{
//	// インスタンスの取得
//	TextureLoader* instance = TextureLoader::GetInstance();
//
//	// フルファイルパス
//	std::string fullFilePath = "Resources/" + filePath + "/" + FileName;
//
//	// マップコンテナで検索
//	if (!instance->CheckTextureData(FileName)) {
//
//		std::string format = GetExtension(FileName);
//
//		// なければ新しく作る。拡張子で処理を変える
//		if (format == TexFormat::PNG.first) {
//			instance->CreateTextureDataFormatPng(fullFilePath, FileName);
//		}
//		else if (format == TexFormat::JPEG.first) {
//			instance->CreateTextureDataFormatJpeg(fullFilePath, FileName);
//		}
//		else if (format == TexFormat::DSS.first) {
//			instance->CreateTextureDataFormatDDS(fullFilePath, FileName);
//		}
//	}
//
//	// filePahtのマップコンテナのindexを返す
//	return instance->textureMaps_[FileName].index;
//}
//
//void TextureLoader::CreateFence()
//{
//	// Deviceの取得
//	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXManager::GetInstance()->GetDevice();
//
//	// 初期値0でFenceを作る
//	HRESULT result{};
//	result = device->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
//	assert(SUCCEEDED(result));
//
//	// FenceのSignalを待つためのイベントを作成する
//	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
//	assert(fenceEvent_ != nullptr);
//}
//
//bool TextureLoader::CheckTextureData(std::string key) {
//
//	// マップコンテナに検索をかける
//	auto data = textureMaps_.find(key);
//
//	// ヒットしたらtrueを返す
//	if (data != textureMaps_.end()) {
//		return true;
//	}
//	return false;
//}
//
//void TextureLoader::CreateTextureDataFormatPng(std::string filePath, std::string key)
//{
//	// インスタンスの取得
//	TextureLoader* instance = TextureLoader::GetInstance();
//
//	// DescriptorManagerの取得
//	DescriptorManager* descriptor = DescriptorManager::GetInstance();
//
//	// 新しく作成するTextureDataを用意
//	TextureData textureData{};
//
//	// Textureを読んで転送する
//	DirectX::ScratchImage mipImages = CreateMipImage(filePath, TexFormat::PNG.second);
//	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
//	textureData.resource = CreateTextureResource(metadata);
//
//	// 登録
//	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResourece =
//		UploadTextureData(textureData.resource.Get(), mipImages);
//
//	// Commandの実行
//	instance->ExeCommands();
//
//	// SRV作成
//	//textureData.index = SRVManager::CreateTextureSRV(textureData.resource, metadata);
//	textureData.index = descriptor->CreateTextureSRV(textureData.resource, metadata);
//
//	// textureのサイズの取得
//	textureData.size = {
//		static_cast<float>(metadata.width),
//		static_cast<float>(metadata.height) };
//
//	// コンテナに保存
//	instance->textureMaps_[key] = textureData;
//}
//void TextureLoader::CreateTextureDataFormatJpeg(std::string filePath, std::string key)
//{
//	filePath, key;
//}
//void TextureLoader::CreateTextureDataFormatDDS(std::string filePath, std::string key)
//{
//	// インスタンスの取得
//	TextureLoader* instance = TextureLoader::GetInstance();
//
//	// DescriptorManagerの取得
//	DescriptorManager* descriptor = DescriptorManager::GetInstance();
//
//	// 新しく作成するTextureDataを用意
//	TextureData textureData{};
//
//	// Textureを読んで転送する
//	DirectX::ScratchImage mipImages = CreateMipImage(filePath, TexFormat::DSS.second);
//	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
//	textureData.resource = CreateTextureResource(metadata);
//
//	// 登録
//	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResourece =
//		UploadTextureData(textureData.resource.Get(), mipImages);
//
//	// Commandの実行
//	instance->ExeCommands();
//
//	// SRV作成
//	//textureData.index = SRVManager::CreateTextureSRV(textureData.resource, metadata);
//	textureData.index = descriptor->CreateTextureSRV(textureData.resource, metadata);
//
//	// textureのサイズの取得
//	textureData.size = {
//		static_cast<float>(metadata.width),
//		static_cast<float>(metadata.height) };
//
//	// コンテナに保存
//	instance->textureMaps_[key] = textureData;
//}
//
//DirectX::ScratchImage TextureLoader::CreateMipImage(const std::string& filePath, const uint32_t format) {
//
//	DirectX::ScratchImage image{};
//	std::wstring filePathw{};
//	DirectX::ScratchImage mipImages{};
//
//	// formatで処理を変える
//	if (format == TexFormat::PNG.second) { // Format -> PNG
//
//		// テクスチャファイルを読み込んでプログラムで扱えるようにする
//		// Textureデータを読み込む
//		filePathw = ConverString(filePath);
//		HRESULT hr = DirectX::LoadFromWICFile(filePathw.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
//		assert(SUCCEEDED(hr));
//
//		// ミニマップの作成
//		hr = DirectX::GenerateMipMaps(
//			image.GetImages(), image.GetImageCount(), image.GetMetadata(),
//			DirectX::TEX_FILTER_SRGB, 0, mipImages);
//		assert(SUCCEEDED(hr));
//	}
//	else if (format == TexFormat::JPEG.second) { // Format -> JPEG
//
//	}
//	else if (format == TexFormat::DSS.second) { // Format -> DSS
//
//		// テクスチャファイルを読み込んでプログラムで扱えるようにする
//		// Textureデータを読み込む
//		filePathw = ConverString(filePath);
//		HRESULT hr = DirectX::LoadFromDDSFile(filePathw.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
//		assert(SUCCEEDED(hr));
//
//		// ミニマップの作成
//		if (DirectX::IsCompressed(image.GetMetadata().format)) { // 圧縮フォーマットかどうかを調べる
//			mipImages = std::move(image); // 圧縮フォーマットならそのまま使うのでstd::moveする
//		}
//		else {
//			hr = DirectX::GenerateMipMaps(
//				image.GetImages(), image.GetImageCount(), image.GetMetadata(),
//				DirectX::TEX_FILTER_SRGB, 4, mipImages);
//			assert(SUCCEEDED(hr));
//		}
//	}
//
//	// ミニマップ付きのデータを探す	
//	return mipImages;
//}
//
//ComPtr<ID3D12Resource> TextureLoader::CreateTextureResource(const DirectX::TexMetadata& metadata) {
//
//	// 1.metadataを基にResourceの設定
//	// metadataを基にResourceの設定
//	D3D12_RESOURCE_DESC resourceDesc{};
//	resourceDesc = SettingResource(metadata);
//
//	// 2. 利用するHeapの設定
//	// 利用するHeapの設定。非常に特殊な運用 ※02_04exで一般的なケース版がある
//	D3D12_HEAP_PROPERTIES heapProperties{};
//	heapProperties = SettingUseHeap();
//
//	// 3. Resourceを作る
//	// Resourceを生成する
//	ComPtr<ID3D12Resource> resource{};
//	resource = CreateCBV(resourceDesc, heapProperties);
//
//	return resource;
//}
//
//D3D12_RESOURCE_DESC TextureLoader::SettingResource(const DirectX::TexMetadata& metadata) {
//
//	D3D12_RESOURCE_DESC resourceDesc{};
//	resourceDesc.Width = UINT(metadata.width);								// Textureの幅
//	resourceDesc.Height = UINT(metadata.height);							// Textureの高さ
//	resourceDesc.MipLevels = UINT16(metadata.mipLevels);						// mipmapの数
//	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);			    // 奥行き or 配列Textureの配列数
//	resourceDesc.Format = metadata.format;									// TextureのFormat
//	resourceDesc.SampleDesc.Count = 1;										// サンプリングカウント。1固定
//	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);  // Textureの次元数。普段使っているのは2次元
//
//	return resourceDesc;
//}
//
//D3D12_HEAP_PROPERTIES TextureLoader::SettingUseHeap() {
//
//	D3D12_HEAP_PROPERTIES heapProperties{};
//	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;                        // VRAM上に作成する
//	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
//	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;	         // プロセッサの近くに配置
//
//	return heapProperties;
//}
//
//ComPtr<ID3D12Resource> TextureLoader::CreateCBV(D3D12_RESOURCE_DESC resourceDesc, D3D12_HEAP_PROPERTIES heapProperties) {
//
//	ComPtr<ID3D12Resource> resource;
//	HRESULT result;
//	result = DirectXManager::GetInstance()->GetDevice()->CreateCommittedResource(
//		&heapProperties,				   // Heapの設定
//		D3D12_HEAP_FLAG_NONE,			   // Heapの特殊な設定。特になし
//		&resourceDesc,					   // Resourceの設定
//		D3D12_RESOURCE_STATE_COPY_DEST,    // データ転送される設定
//		nullptr,						   // Clear最適地。使わないのでnullptr
//		IID_PPV_ARGS(&resource));		   // 作成するResourceポインタへのポインタ
//
//	assert(SUCCEEDED(result));
//
//	return resource;
//}
//
//void TextureLoader::UpdateTextureData(const DirectX::TexMetadata& metadata, DirectX::ScratchImage& mipImages, TextureData textureData) {
//
//	// Meta情報を取得
//	//const DirectX::TexMetadata& metadata = mipImage.GetMetadata();
//
//	// 全MipMapについて
//	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
//
//		//MipMapLevelを指定して各Imageを取得
//		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
//
//		// Textureに転送
//		HRESULT result;
//		result = textureData.resource->WriteToSubresource(
//			UINT(mipLevel),
//			nullptr,			  // 全領域へコピー
//			img->pixels,		  // 元データアドレス
//			UINT(img->rowPitch),  // 1ラインサイズ
//			UINT(img->slicePitch) // 1枚サイズ
//		);
//		assert(SUCCEEDED(result));
//	}
//}
//[[nodiscard]]
//Microsoft::WRL::ComPtr<ID3D12Resource>  TextureLoader::UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages)
//{
//	// Deviceの取得
//	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXManager::GetInstance()->GetDevice();
//
//	// Commandの取得
//	Commands commands = CommandManager::GetInstance()->GetCommands();
//
//	// 読み込んだデータからDirectX12用のSubResourceの配列を作成する
//	std::vector<D3D12_SUBRESOURCE_DATA> subResources;
//	DirectX::PrepareUpload(device.Get(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subResources);
//
//	// Subresourceの数を基に、コピー元となるResourceに必要なサイズを計算する
//	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subResources.size()));
//
//	// 計算したサイズでIntermediateResourceを作る。CPUとGPUを取り持つためのResourceなので、intermediateResource(中間リソース)と呼ぶ
//	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = CreateResource::CreateBufferResource(intermediateSize);
//
//	// データ転送をコマンドに積む
//	UpdateSubresources(commands.List.Get(), texture.Get(), intermediateResource.Get(), 0, 0, UINT(subResources.size()), subResources.data());
//
//	// ResourceState変更し、IntermediateResourceを返す
//	// Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_ReSOURCE_STATE_GENERIC_READへResourceStateを変更する
//	D3D12_RESOURCE_BARRIER barrier{};
//	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
//	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
//	barrier.Transition.pResource = texture.Get();
//	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
//	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
//	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
//
//	commands.List->ResourceBarrier(1, &barrier);
//
//	return intermediateResource;
//}
//
//void TextureLoader::ExeCommands()
//{
//	// Deviceの取得
//	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXManager::GetInstance()->GetDevice();
//
//	// Commandの取得
//	Commands commands = CommandManager::GetInstance()->GetCommands();
//
//
//	// CommandListをCloseし、CommandQueue->ExecuteComandListsを使いキックする
//	HRESULT result;
//	result = commands.List->Close();
//	assert(SUCCEEDED(result));
//	ID3D12CommandList* commandLists[] = { commands.List.Get() };
//	commands.Queue->ExecuteCommandLists(1, commandLists);
//
//
//	// 実行を待つ
//	fenceValue_++; // fenceの値を更新；
//	// GPUここまでたどり着いたときに、fenceの値を指定した値に代入するようにSignalを送る
//	commands.Queue->Signal(fence_.Get(), fenceValue_);
//	// fenceの値が指定したSignal値にたどり着いているか確認する。GetCompletedValueの初期値はfence作成時に渡した初期値
//	if (fence_->GetCompletedValue() < fenceValue_) {
//
//		// 指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
//		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
//		// イベントを待つ
//		WaitForSingleObject(fenceEvent_, INFINITE);
//	}
//
//
//	// 実行が完了したので、allocatorとcommandListをResetして次のコマンドを積めるようにする
//	result = commands.Allocator->Reset();
//	assert(SUCCEEDED(result));
//	result = commands.List->Reset(commands.Allocator.Get(), nullptr);
//	assert(SUCCEEDED(result));
//
//
//	CommandManager::GetInstance()->SetCommands(commands);
//}
//
