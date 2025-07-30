#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")

#include <wrl.h>
using Microsoft::WRL::ComPtr;
using namespace std;


class DirectXManager;


// コマンド
struct Commands {
	ComPtr<ID3D12CommandQueue> Queue;		   // コマンドキュー
	ComPtr<ID3D12CommandAllocator> Allocator; // コマンドアロケータ
	ComPtr<ID3D12GraphicsCommandList> List;   // コマンドリスト
};


/* CommandManagerクラス */
class CommandManager {

private: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	CommandManager() = default;
	~CommandManager() = default;
	CommandManager(const CommandManager&) = delete;
	const CommandManager& operator=(const CommandManager&) = delete;


public: // メンバ関数

	// インスタンスの取得
	static CommandManager* GetInstance() {
		static CommandManager instance;
		return &instance;
	}

	// 初期化処理
	void Initialize();

	// 解放処理
	void Finalize();


#pragma region Accessor アクセスメソッド

	// コマンド
	Commands const GetCommands() { return this->commands_; }
	void SetCommands(Commands setCommands) { this->commands_ = setCommands; }

	// コマンドキュー
	ID3D12CommandQueue* const GetQueue() { return this->commands_.Queue.Get(); }
	void SetQueue(ID3D12CommandQueue* const setQueue) { this->commands_.Queue = setQueue; }

	// アロケータ
	ID3D12CommandAllocator* const GetAllocator() { return this->commands_.Allocator.Get(); }
	void SetAllocator(ID3D12CommandAllocator* const setAllocator) { this->commands_.Allocator = setAllocator; }

	// リスト
	ID3D12GraphicsCommandList* const GetList() { return this->commands_.List.Get(); }
	void SetList(ID3D12GraphicsCommandList* const setList) { this->commands_.List = setList; }

#pragma endregion 


private: // メンバ関数

	// コマンドキューを生成する
	void CreateCommandQueue();

	// コマンドアロケータを生成する
	void CreateCommandAllocator();

	// コマンドリストを生成する
	void CreateCommandList();


private: // メンバ変数

	// DirectXCommon
	DirectXManager* dxCommon_ = nullptr;

	// コマンド
	Commands commands_{};

};

