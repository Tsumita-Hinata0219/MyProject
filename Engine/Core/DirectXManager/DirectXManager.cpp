#include "DirectXManager.h"


/// <summary>
/// DirectXCommonのインスタンス取得
/// </summary>
DirectXManager* DirectXManager::GetInstance() 
{
	static DirectXManager instance;
	return &instance;
}


/// <summary>
/// 初期化処理
/// </summary>
void DirectXManager::Initialize() 
{
	DirectXManager* directX = DirectXManager::GetInstance();

	/* ----- デバッグレイヤー -----*/
#ifdef _DEBUG

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&directX->debugController_)))) {
		// デバッグレイヤーを有効にする
		directX->debugController_->EnableDebugLayer();
		// さらにGPU側でもチェックを行うようにする
		directX->debugController_->SetEnableGPUBasedValidation(TRUE);
	}

#endif // _DEBUG

	// FPS固定初期化処理
	directX->InitFixFPS();

	// DxgiFactoryを生成する
	directX->CreateDxgiFactory();

	// D3D12Deviceを生成する
	directX->CreateDevice();

	// エラーと警告の抑制
	directX->DebugErrorInfoQueue();

	// コマンド関連の生成
	CommandManager::GetInstance()->Initialize();

	// スワップチェーンを生成する
	directX->CreateSwapChain();

	// ディスクリプタヒープの生成
	directX->SetDescriptorHeap();

	// フェンスを作る
	directX->MakeFence();

	// ViewportとScissor
	directX->SetViewport();
	directX->SetScissor();
}


/// <summary>
/// 描画前処理 PostEffect用
/// </summary>
void DirectXManager::PreDrawForPostEffect() 
{
	auto commandManager = CommandManager::GetInstance();
	Commands commands = commandManager->GetCommands();
	D3D12_RESOURCE_BARRIER barrier{};

	// RTV を取得
	RTVProperty rtv = RTVManager::GetRTV("PostEffect")->GetRTVPrope();
	if (rtv.Resources.Get() == nullptr) {
		// RTV が無効な場合はエラー処理
		assert(false && "Invalid RTV resource.");
		return;
	}

	// Barrierを設定する
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = rtv.Resources.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	// TransitionBarrierを張る
	commands.List->ResourceBarrier(1, &barrier);

	// 描画先のRTVとDSVを設定
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = DirectXManager::GetInstance()->dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	commands.List->OMSetRenderTargets(1, &rtv.Handles, false, &dsvHandle);

	// 色で画面全体をクリア
	Vector4 color = rtv.color;
	float clearColor[] = { color.x, color.y, color.z, color.w };
	commands.List->ClearRenderTargetView(rtv.Handles, clearColor, 0, nullptr);

	// 深度で画面全体をクリア
	commands.List->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポートとシザーを設定
	commands.List->RSSetViewports(1, &DirectXManager::GetInstance()->viewport_);
	commands.List->RSSetScissorRects(1, &DirectXManager::GetInstance()->scissorRect_);

	// コマンドを更新
	commandManager->SetCommands(commands);
}


/// <summary>
/// 描画後処理 PostEffect用
/// </summary>
void DirectXManager::PostDrawForPostEffect() 
{
	Commands commands = CommandManager::GetInstance()->GetCommands();
	RTVProperty rtv = RTVManager::GetRTV("PostEffect")->GetRTVPrope();

	// リソースが無効でないか確認
	if (rtv.Resources.Get() == nullptr) {
		// エラーハンドリング（例：ログ出力）
		assert(false && "Resource is null.");
		return;
	}

	D3D12_RESOURCE_BARRIER barrier{};
	// Barrierを設定する
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = rtv.Resources.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	// TransitionBarrierを張る
	commands.List->ResourceBarrier(1, &barrier);
}


/// <summary>
/// 描画前処理 SwapChain用
/// </summary>
void DirectXManager::PreDrawForSwapChain() 
{
	// DirectXCommon インスタンス取得
	auto directXCommon = DirectXManager::GetInstance();
	if (!directXCommon) {
		// エラー処理: DirectXCommon インスタンスが取得できない場合
		return;
	}

	// スワップチェインおよび関連リソース取得
	SwapChains swapChains{};
	swapChains.swapChain = directXCommon->swapChains_.swapChain.Get();
	swapChains.Resources[0] = directXCommon->swapChains_.Resources[0].Get();
	swapChains.Resources[1] = directXCommon->swapChains_.Resources[1].Get();

	// スワップチェインが無効な場合は処理を中断
	if (!swapChains.swapChain || !swapChains.Resources[0] || !swapChains.Resources[1]) {
		// エラー処理: スワップチェインまたはリソースが無効
		return;
	}

	// コマンド取得
	Commands commands = CommandManager::GetInstance()->GetCommands();
	if (!commands.List) {
		// エラー処理: コマンドリストが無効
		return;
	}

	// RTVプロパティ取得
	RTVProperty rtv[2];
	for (int i = 0; i < 2; ++i) {
		auto rtvManager = RTVManager::GetRTV("SwapChain" + std::to_string(i));
		if (rtvManager) {
			rtv[i] = rtvManager->GetRTVPrope();
		}
		else {
			// エラー処理: RTVManager から取得できなかった場合
			return;
		}
	}

	// バックバッファのインデックス取得
	UINT backBufferIndex = swapChains.swapChain->GetCurrentBackBufferIndex();

	// バリア設定
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = swapChains.Resources[backBufferIndex].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	// リソースバリアをコマンドリストに設定
	commands.List->ResourceBarrier(1, &barrier);

	// 現在のバリアを保存（後で使用する場合）
	directXCommon->barrier_ = barrier;

	// 描画先のRTVを設定
	commands.List->OMSetRenderTargets(
		1, &rtv[backBufferIndex].Handles,
		false,
		nullptr);

	// 指定した色で画面全体をクリア
	float clearColor[] = {
		rtv[backBufferIndex].color.x,
		rtv[backBufferIndex].color.y,
		rtv[backBufferIndex].color.z,
		rtv[backBufferIndex].color.w
	};

	commands.List->ClearRenderTargetView(
		rtv[backBufferIndex].Handles,
		clearColor,
		0, nullptr);

	// ビューポートとシザー矩形を設定
	commands.List->RSSetViewports(1, &directXCommon->viewport_);
	commands.List->RSSetScissorRects(1, &directXCommon->scissorRect_);

	// コマンドをCommandManagerにセット
	CommandManager::GetInstance()->SetCommands(commands);
}


/// <summary>
/// 描画後処理 SwapChain用
/// </summary>
void DirectXManager::PostDrawForSwapChain() 
{
	auto directXCommon = DirectXManager::GetInstance();
	auto commandManager = CommandManager::GetInstance();

	// 必要な変数を取得
	SwapChains& swapChains = directXCommon->swapChains_;
	Commands commands = commandManager->GetCommands();
	D3D12_RESOURCE_BARRIER& barrier = directXCommon->barrier_;

	// 状態遷移：RenderTarget -> Present
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commands.List->ResourceBarrier(1, &barrier);

	// コマンドリストを閉じる
	HRESULT result = commands.List->Close();
	if (FAILED(result)) {
		// エラーハンドリング（例：ログ出力）
		assert(false && "Failed to close command list.");
	}

	// コマンドリストを実行
	ID3D12CommandList* commandLists[] = { commands.List.Get() };
	commands.Queue->ExecuteCommandLists(1, commandLists);

	// フェンスの値を更新してSignal
	directXCommon->fenceValue_++;
	commands.Queue->Signal(directXCommon->fence_.Get(), directXCommon->fenceValue_);

	// GPUが指定した値に達していない場合は待機
	if (directXCommon->fence_->GetCompletedValue() < directXCommon->fenceValue_) {
		directXCommon->fence_->SetEventOnCompletion(directXCommon->fenceValue_, directXCommon->fenceEvent_);
		WaitForSingleObject(directXCommon->fenceEvent_, INFINITE);
	}

	// FPS固定更新処理
	directXCommon->UpdateFixFPS();

	// 画面に描画
	swapChains.swapChain->Present(1, 0);

	// 次のフレーム用にコマンドリストをリセット
	result = commands.Allocator->Reset();
	if (FAILED(result)) {
		assert(false && "Failed to reset command allocator.");
	}

	result = commands.List->Reset(commands.Allocator.Get(), nullptr);
	if (FAILED(result)) {
		assert(false && "Failed to reset command list.");
	}

	// コマンド情報を更新
	commandManager->SetCommands(commands);
}


/// <summary>
/// 解放処理
/// </summary>
void DirectXManager::Release() 
{
	CloseHandle(DirectXManager::GetInstance()->fenceEvent_);
}


/// <summary>
/// FPS固定初期化処理
/// </summary>
void DirectXManager::InitFixFPS()
{
	// 現在時間を記録する
	reference_ = std::chrono::steady_clock::now();
}


/// <summary>
/// FPS固定更新処理
/// </summary>
void DirectXManager::UpdateFixFPS()
{
	// 1/60秒ぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60秒よりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	// 現在時刻を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// 前回記録からの経過時間を記録する
	std::chrono::microseconds elapased =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒(よりわずかに短い時間)立ってない場合
	if (elapased < kMinCheckTime) {
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			// 1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}


/* ---DirectX初期化--- */
/// ここから ↓ 
//////////////////////////////

/// <summary>
/// DirectX初期化
/// </summary>
void DirectXManager::CreateDxgiFactory() 
{
	IDXGIFactory7* dxgiFactory = DirectXManager::GetInstance()->dxgiFactory_.Get();
	IDXGIAdapter4* useAdapter = DirectXManager::GetInstance()->useAdapter_.Get();

	// DXGIファクトリーの生成
	// HRESULTはWindows系のエラーコードであり、
	// 関数が成功したかどうかをSUCCEEDEDマクロで判定できる
	dxgiFactory = nullptr;
	HRESULT result;
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	// 初期化の根本的な部分でエラーが出た場合はプログラムが間違ってるかどうか、
	// どうにもできない場合が多いのでassertにしておく
	assert(SUCCEEDED(result));


	// いい順にアダプタを頼む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; i++)
	{
		// アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		result = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(result)); // 取得できないのは一大事

		// ソフトウェアアダプタ出なければ採用！
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// 採用したアダプタの情報をログに出力。wstringのほうなので注意
			Log(ConvertString(std::format(L"USE Adapater:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr; // ソフトウェアアダプタの場合は見なかったことにする
	}
	// 適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);

	this->dxgiFactory_ = dxgiFactory;
	this->useAdapter_ = useAdapter;
}


/// <summary>
/// D3D12Deviceを生成する
/// </summary>
void DirectXManager::CreateDevice() 
{
	// 昨日レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0
	};

	const char* featureLevelStrings[] = { "12.2", "12.1", "12.0" };


	// 高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		// 採用したアダプターでデバイスを生成
		HRESULT result;
		result = D3D12CreateDevice(this->useAdapter_.Get(), featureLevels[i], IID_PPV_ARGS(&DirectXManager::GetInstance()->device_));

		// 指定した昨日レベルでデバイスが生成できたかを確認
		if (SUCCEEDED(result)) {
			// 生成できたのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}

	// デバイスの生成がうまく行かなかったので起動できない
	assert(this->device_ != nullptr);
	Log("Complete create D3D12Device!!!\n"); // 初期化完了のログを出す

}

//////////////////////////////
/// ここまで↑
/* ---DirectX初期化--- */


/// <summary>
/// エラーと警告の抑制
/// </summary>
void DirectXManager::DebugErrorInfoQueue() 
{
#ifdef _DEBUG

	ID3D12InfoQueue* infoQueue = this->infoQueue_.Get();
	D3D12_INFO_QUEUE_FILTER filter = this->filter_;

	if (SUCCEEDED(this->device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {

		// ヤバいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);

		// エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		// 警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);


		// 抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds_[] = {
			// Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			// https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities_[] = { D3D12_MESSAGE_SEVERITY_INFO };
		filter.DenyList.NumIDs = _countof(denyIds_);
		filter.DenyList.pIDList = denyIds_;
		filter.DenyList.NumSeverities = _countof(severities_);
		filter.DenyList.pSeverityList = severities_;
		// 指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		this->infoQueue_ = infoQueue;
		this->filter_ = filter;


		// 解放
		this->infoQueue_->Release();
	}

#endif // _DEBUG
}


/// <summary>
/// スワップチェーンを生成する
/// </summary>
void DirectXManager::CreateSwapChain() 
{
	HWND hwnd_ = WinApp::GetInstance()->GetHwnd();
	SwapChains swapChains = this->swapChains_;

	swapChains.Desc.Width = WinApp::kWindowWidth;	  // 画面の幅、ウィンドウのクライアント領域を同じものにしてく
	swapChains.Desc.Height = WinApp::kWindowHeight; // 画面の高さ、ウィンドウのクライアント領域を同じものにしておく
	swapChains.Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色の形式
	swapChains.Desc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChains.Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 描画のターゲットとして利用する
	swapChains.Desc.BufferCount = 2; // ダブルバッファ
	swapChains.Desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // モニタにうつしたら、中身を破棄

	// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	HRESULT result;
	result = this->dxgiFactory_->CreateSwapChainForHwnd(
		CommandManager::GetInstance()->GetQueue(),
		hwnd_,
		&swapChains.Desc,
		nullptr,
		nullptr,
		reinterpret_cast<IDXGISwapChain1**>(swapChains.swapChain.GetAddressOf()));

	assert(SUCCEEDED(result));

	this->swapChains_ = swapChains;
}


/// <summary>
/// ディスクリプタヒープの生成
/// </summary>
void DirectXManager::SetDescriptorHeap() 
{
	RTVManager::GetInstance()->SetDescriptorHeap(
		CreateDescriptorHeap(this->device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 5, false)
	);

	this->srvDescriptorHeap_ = CreateDescriptorHeap(
		this->device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1280, true);


	// SwapChainからResourceを引っ張ってくる
	CreateSwapChainResources();


	// RTVを作る
	CreateRTV();


	// depthStencilResourceを作る
	CreateDepthStencilResource();
}


/// <summary>
/// SwapChainからResourceを引っ張ってくる
/// </summary>
void DirectXManager::CreateSwapChainResources() 
{
	HRESULT result;
	result = this->swapChains_.swapChain->GetBuffer(0, IID_PPV_ARGS(&this->swapChains_.Resources[0]));

	// うまく取得できなければ起動できない
	assert(SUCCEEDED(result));


	result = this->swapChains_.swapChain->GetBuffer(1, IID_PPV_ARGS(&this->swapChains_.Resources[1]));

	// うまく取得できなければ起動できない
	assert(SUCCEEDED(result));
}


/// <summary>
/// RTVを作る
/// </summary>
void DirectXManager::CreateRTV()
{
	ComPtr<ID3D12Device> device = this->device_;
	SwapChains swapChains = this->GetSwapChains();

	// Heapの設定
	RTVManager::GetInstance()->SetDescriptorHeap(CreateDescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_Index_Max, false));

	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 出力結果をSRGBに変換して書き込む
	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2sテクスチャとして書き込む
	RTVManager::GetInstance()->SetDesc(desc); // デスクの設定

	// クリアカラー
	Vector4 clearColor = { 0.1f, 0.25f, 0.5f, 1.0f }; // 青っぽい色。RGBAの順

	// ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE startHandle;
	startHandle = RTVManager::GetInstance()->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();


	RTVProperty rtvPrope[4]{};
	// 1つ目
	rtvPrope[0].Handles = startHandle;
	device->CreateRenderTargetView(swapChains.Resources[0].Get(), &desc, rtvPrope[0].Handles);
	rtvPrope[0].Resources = swapChains.Resources[0].Get();
	rtvPrope[0].color = clearColor;
	
	// 2つ目
	rtvPrope[1].Handles.ptr = rtvPrope[0].Handles.ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	device->CreateRenderTargetView(swapChains.Resources[1].Get(), &desc, rtvPrope[1].Handles);
	rtvPrope[1].Resources = swapChains.Resources[1].Get();
	rtvPrope[1].color = clearColor;
	
	// 3つ目
	rtvPrope[2].Handles.ptr = rtvPrope[1].Handles.ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	auto renderTextureResource = CreateResource::CreateRenderTextureResource(
		WinApp::kWindowWidth,
		WinApp::kWindowHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		clearColor);
	device->CreateRenderTargetView(renderTextureResource.Get(), &desc, rtvPrope[2].Handles);
	rtvPrope[2].Resources = renderTextureResource.Get();
	rtvPrope[2].color = clearColor;


	// DescriptorHandleとDescriptorHeap
	typedef struct D3D12_CPU_DESCRIPTOR_HANDLE {
		SIZE_T ptr;
	}D3D12_CPU_DESCRIPTOR_HANDLE;

	
	// RTVManagerに設定する
	RTVManager::AddRTV("SwapChain0", rtvPrope[0]);
	RTVManager::AddRTV("SwapChain1", rtvPrope[1]);
	RTVManager::AddRTV("PostEffect", rtvPrope[2]);
}


/// <summary>
/// 状態を遷移する
/// </summary>
void DirectXManager::ChanegResourceState() 
{
	// 画面に描く処理はすべて終わり、画面に映すので、状態を遷移
	// 今回はRenderTargetからPresentにする
	this->barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	this->barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	// TransitionBarrierを張る
	CommandManager::GetInstance()->GetList()->ResourceBarrier(1, &this->barrier_);
}


/// <summary>
/// Fenceを生成する
/// </summary>
void DirectXManager::MakeFence() 
{
	// 初期値0でFenceを作る
	HRESULT result;
	result = this->device_->CreateFence(this->fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->fence_));
	assert(SUCCEEDED(result));
	// FenceのSignalを待つためのイベントを作成する
	HANDLE fenceEvent;
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}


/// <summary>
/// ViewportとScissor
/// </summary>
void DirectXManager::SetViewport() 
{
	// ビューポート
	// クライアント領域のサイズと一緒にして画面全体に表示
	this->viewport_.Width = float(WinApp::kWindowWidth);
	this->viewport_.Height = float(WinApp::kWindowHeight);
	this->viewport_.TopLeftX = 0;
	this->viewport_.TopLeftY = 0;
	this->viewport_.MinDepth = 0.0f;
	this->viewport_.MaxDepth = 1.0f;
}
void DirectXManager::SetScissor() 
{
	// シザー矩形
	// 基本的にビューポートと同じ矩形が構成されるようにする
	this->scissorRect_.left = 0;
	this->scissorRect_.right = WinApp::kWindowWidth;
	this->scissorRect_.top = 0;
	this->scissorRect_.bottom = WinApp::kWindowHeight;
}



ID3D12DescriptorHeap* DirectXManager::CreateDescriptorHeap(
	ID3D12Device* device,
	D3D12_DESCRIPTOR_HEAP_TYPE heapType,
	UINT numDescriptors,
	bool shaderVisible) {

	this->DescriptorHeapDesc_.Type = heapType;
	this->DescriptorHeapDesc_.NumDescriptors = numDescriptors;
	this->DescriptorHeapDesc_.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT result;
	result = device->CreateDescriptorHeap(&this->DescriptorHeapDesc_, IID_PPV_ARGS(&this->descriptorHeap_));
	//ディスクリプタヒープの生成ができないので起動できない
	assert(SUCCEEDED(result));
	return this->descriptorHeap_.Get();
}



/// <summary>
/// Textureの深度の設定をしていく
/// </summary>
ComPtr<ID3D12Resource> DirectXManager::CreateDepthStencilTexturerResource(int32_t width, int32_t height) 
{
	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width; // Textureの幅
	resourceDesc.Height = height; // Textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.DepthOrArraySize = 1; // 奥行き or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。Resourceと合わせる

	// Resourceの生成
	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT result;
	result = this->device_->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Hepaの特殊な設定。特になし
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue, // Clear最適値
		IID_PPV_ARGS(&resource)); // 作成するResourceポインタへのポインタ

	assert(SUCCEEDED(result));

	return resource;
}



/// <summary>
/// depthStencilResourceを作る
/// </summary>
void DirectXManager::CreateDepthStencilResource() 
{
	// DepthStencilTextureをウィンドウサイズで作成
	this->depthStencilResource_ = CreateDepthStencilTexturerResource(
		WinApp::kWindowWidth, 
		WinApp::kWindowHeight);

	// DSV用のヒープでディスクリプタのの数は1。DSVはShader内で触るものではないので、ShaderVisibleはfalse
	this->dsvDescriptorHeap_ =
		CreateDescriptorHeap(this->device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

	// DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format。基本的はResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture

	// DSVHeapの先頭にDSVを作る
	this->device_->CreateDepthStencilView(
		this->depthStencilResource_.Get(),
		&dsvDesc, 
		this->dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());
}
