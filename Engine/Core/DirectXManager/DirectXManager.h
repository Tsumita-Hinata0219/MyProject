#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")

#include <chrono>
#include <thread>

#include "Platform/WinApp/WinApp.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "Math/MyMath.h"
#include "Math/Struct.h"
#include "Resource/CreateResource/CreateResource.h"
#include "Resource/View/RTVManager/RTVManager.h"


// スワップチェーン
struct SwapChains {
	ComPtr<IDXGISwapChain4> swapChain;				// スワップチェーン
	ComPtr<ID3D12Resource> Resources[2];	// スワップチェーンリソース
	DXGI_SWAP_CHAIN_DESC1 Desc{}; // スワップチェーンデスク
};

// RTV
struct RTV {
	ComPtr<ID3D12DescriptorHeap> DescriptorHeap;
	D3D12_RENDER_TARGET_VIEW_DESC Desc{};
	D3D12_CPU_DESCRIPTOR_HANDLE Handles[3];
	D3D12_CPU_DESCRIPTOR_HANDLE StartHandle;
	ComPtr<ID3D12Resource> Resources[3];
	Vector4 color[3];
};



/// <summary>
/// DirectXCommonクラス
/// </summary>
class DirectXManager {

public: // メンバ関数

    /// <summary>
    /// インスタンスの取得
    /// </summary>
    static DirectXManager* GetInstance();

    /// <summary>
    /// 初期化処理
    /// </summary>
    static void Initialize();

    /// <summary>
    /// 描画前処理 PostEffect用
    /// </summary>
    static void PreDrawForPostEffect();

    /// <summary>
    /// 描画後処理 PostEffect用
    /// </summary>
    static void PostDrawForPostEffect();

    /// <summary>
    /// 描画前処理 SwapChain用
    /// </summary>
    static void PreDrawForSwapChain();

    /// <summary>
    /// 描画後処理 SwapChain用
    /// </summary>
    static void PostDrawForSwapChain();

    /// <summary>
    /// 解放処理
    /// </summary>
    static void Release();

#pragma region Accessor アクセッサ

    // デバイスの取得
    ID3D12Device* const GetDevice() { return device_.Get(); }

    // スワップチェーンの取得
    SwapChains const GetSwapChains() { return swapChains_; }

    // SRVディスクリプタヒープの取得
    ID3D12DescriptorHeap* const GetSrvDescriptorHeap() { return srvDescriptorHeap_.Get(); }

#pragma endregion

private:

    /// <summary>
    /// FPS固定初期化処理
    /// </summary>
    void InitFixFPS();

    /// <summary>
    /// FPS固定更新処理
    /// </summary>
    void UpdateFixFPS();

    /// <summary>
    /// DXGIファクトリーの生成
    /// </summary>
    void CreateDxgiFactory();

    /// <summary>
    /// D3D12Deviceの生成
    /// </summary>
    void CreateDevice();

    /// <summary>
    /// エラーと警告の抑制
    /// </summary>
    void DebugErrorInfoQueue();

    /// <summary>
    /// スワップチェーンを生成する
    /// </summary>
    void CreateSwapChain();

    /// <summary>
    /// ディスクリプタヒープを生成する
    /// </summary>
    void SetDescriptorHeap();

    /// <summary>
    /// SwapChainからResourceを引っ張ってくる
    /// </summary>
    void CreateSwapChainResources();

    /// <summary>
    /// RTVを作る
    /// </summary>
    void CreateRTV();

    /// <summary>
    /// 状態を遷移する
    /// </summary>
    void ChanegResourceState();

    /// <summary>
    /// Fenceを生成する
    /// </summary>
    void MakeFence();

    /// <summary>
    /// ViewportとScissorの設定
    /// </summary>
    void SetViewport();
    void SetScissor();

    /// <summary>
    /// ディスクリプタヒープの作成
    /// </summary>
    ID3D12DescriptorHeap* CreateDescriptorHeap(
        ID3D12Device* device,
        D3D12_DESCRIPTOR_HEAP_TYPE heapType,
        UINT numDescriptors,
        bool shaderVisible);

    /// <summary>
    /// 深度ステンシルテクスチャリソースの作成
    /// </summary>
    ComPtr<ID3D12Resource> CreateDepthStencilTexturerResource(int32_t width, int32_t height);

    /// <summary>
    /// 深度ステンシルリソースの作成
    /// </summary>
    void CreateDepthStencilResource();


private:

    // デバッグレイヤー
    ComPtr<ID3D12Debug1> debugController_ = nullptr;

    // DXGIFactory
    ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;

    // 使用するアダプタ
    ComPtr<IDXGIAdapter4> useAdapter_ = nullptr;

    // D3D12デバイス
    ComPtr<ID3D12Device> device_ = nullptr;

    // エラー・警告情報
    ComPtr<ID3D12InfoQueue> infoQueue_ = nullptr;
    D3D12_INFO_QUEUE_FILTER filter_{};

    // バックバッファインデックス
    UINT backBufferIndex_{};

    // リソースバリア
    D3D12_RESOURCE_BARRIER barrier_{};

    // スワップチェーン
    SwapChains swapChains_{};

    // 
    ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;

    // ディスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;

    // ディスクリプターヒープデスク
    D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc_{};

    // フェンス
    ComPtr<ID3D12Fence> fence_ = nullptr;
    uint64_t fenceValue_ = 0;
    HANDLE fenceEvent_{};

    // dxcコンパイラ
    IDxcUtils* dxcUtils_ = nullptr;
    IDxcCompiler3* dxcCompiler_ = nullptr;
    IDxcIncludeHandler* includeHandler_ = nullptr;

    // ビューポート
    D3D12_VIEWPORT viewport_{};

    // シザー矩形
    D3D12_RECT scissorRect_{};

    // 深度ステンシルリソース
    ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;

    // 深度ステンシルステート
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};
    ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;

    // 記録時間(FPS固定用)
    std::chrono::steady_clock::time_point reference_;
};