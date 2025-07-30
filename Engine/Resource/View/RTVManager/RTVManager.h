#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")
#include <map>

#include "Math/MyMath.h"
#include "Math/Struct.h"
#include "Resource/DescriptorManager/DescriptorManager.h"
#include "Core/DirectXManager/DirectXManager.h"

#define RTV_Index_Max 16


// RTVProperty
struct RTVProperty {
	D3D12_CPU_DESCRIPTOR_HANDLE Handles;
	ComPtr<ID3D12Resource> Resources;
	Vector4 color;
};


class RTVData;

/* RTVManagerクラス */
class RTVManager {

private: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	RTVManager() = default;
	~RTVManager() = default;
	RTVManager(const RTVManager&) = delete;
	const RTVManager& operator=(const RTVManager&) = delete;


public: // メンバ関数

	// インスタンスの取得
	static RTVManager* GetInstance() {
		static RTVManager instance;
		return &instance;
	}

	// 初期化処理
	static void Initialize();

	// 開始処理
	static void BeginFrame();

	// ImGui描画
	static void DrawImGui();

	// RTVDataの追加
	static void AddRTV(const std::string name, RTVProperty prope);

	// RTVDataの検索
	static RTVData* GetRTV(std::string name);

	// 指定RTVDataの破棄
	static void RemoveRTVData(std::string name);

	// 全RTVDataの破棄
	static void AllRemoveRTVData();

#pragma region Heap

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const {
		return this->descriptorHeap_;
	}

	void SetDescriptorHeap(ComPtr<ID3D12DescriptorHeap> heap) {
		this->descriptorHeap_ = heap;
	}
#pragma endregion 

#pragma region Desc

	D3D12_RENDER_TARGET_VIEW_DESC GetDesc() const {
		return this->desc_;
	}

	void SetDesc(D3D12_RENDER_TARGET_VIEW_DESC Desc) {
		this->desc_ = Desc;
	}
#pragma endregion 

#pragma region Handle

	/*D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(const string name) {
		return this->rtvMap_[name]->GetRTVPrope().Handles;
	}*/

	D3D12_CPU_DESCRIPTOR_HANDLE GetStartHandle() const {
		return this->startHandle_;
	}

	void SetStarthandle(D3D12_CPU_DESCRIPTOR_HANDLE handle) {
		this->startHandle_ = handle;
	}

#pragma endregion 


private: // メンバ関数



private: // メンバ変数

	// 使用サイズ
	uint32_t index_ = 0;

	// RTVデータ
	std::map<std::string, unique_ptr<RTVData>> rtvMap_;

	// ヒープ
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	// デスク
	D3D12_RENDER_TARGET_VIEW_DESC desc_{};

	// ディスクリプタの先頭
	D3D12_CPU_DESCRIPTOR_HANDLE startHandle_;
};



/* RTVDataクラス */
class RTVData {

public: // メンバ関数

	// コンストラクタ、デストラクタ
	RTVData(RTVProperty rtv) {
		index_ = 0;
		rtvPrope_ = rtv;
	};
	~RTVData() {};

#pragma region Get

	// RTVの取得
	RTVProperty GetRTVPrope() { return this->rtvPrope_; }

#pragma endregion 

#pragma region Set

	// RTVの取得
	void SetRTVPrope(RTVProperty rtv) { this->rtvPrope_ = rtv; }

#pragma endregion 

private: // メンバ変数

	// インデックス
	uint32_t index_ = 0;

	// RTV
	RTVProperty rtvPrope_{};
};
