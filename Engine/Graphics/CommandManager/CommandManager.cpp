#include "CommandManager.h"
#include "Core/DirectXManager/DirectXManager.h"


// 初期化処理
void CommandManager::Initialize()
{
	// DirectXCommonのインスタンスの取得
	dxCommon_ = DirectXManager::GetInstance();

	// コマンドキューを生成する
	CreateCommandQueue();

	// コマンドアロケータを生成する
	CreateCommandAllocator();

	// コマンドリストを生成する
	CreateCommandList();
}


// 解放処理
void CommandManager::Finalize()
{

}


// コマンドキューを生成する
void CommandManager::CreateCommandQueue()
{
	// コマンドキューを生成する
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	HRESULT result;
	result = dxCommon_->GetDevice()->CreateCommandQueue(
		&commandQueueDesc,
		IID_PPV_ARGS(&this->commands_.Queue));

	// コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));
}


// コマンドアロケータを生成する
void CommandManager::CreateCommandAllocator()
{
	// コマンドアロケータを生成する
	HRESULT result;
	result = dxCommon_->GetDevice()->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&this->commands_.Allocator));

	// コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));
}


// コマンドリストを生成する
void CommandManager::CreateCommandList()
{
	// コマンドリストを生成する
	HRESULT result;
	result = dxCommon_->GetDevice()->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		this->commands_.Allocator.Get(),
		nullptr,
		IID_PPV_ARGS(&this->commands_.List));

	// コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(result));
}
