#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")

#include <assert.h>

#include "Core/DirectXManager/DirectXManager.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "Resource/DescriptorManager/DescriptorManager.h"
#include "Math/MyMath.h"
#include "Math/Struct.h"


/* BufferResourceクラス */
template<typename T>
class BufferResource {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BufferResource() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BufferResource() = default;

	/// <summary>
	/// Resource作成
	/// </summary>
	void CreateCBV(UINT itemCount = 1);

	/// <summary>
	/// VertexBufferViewの作成
	/// </summary>
	void CreateVertexBufferView();

	/// <summary>
	/// IndexBufferViewの作成
	/// </summary>
	void CreateIndexBufferView();

	/// <summary>
	/// InstancingResourceの作成
	/// </summary>
	void CreateInstancingResource(uint32_t instancingNum);

	/// <summary>
	/// UAVの作成
	/// </summary>
	void CreateUAV(UINT itemCount = 1);

	/// <summary>
	/// データを書き込む
	/// </summary>
	void UpdateData(const T* data);
	void UpdateData(const std::vector<T>& datas, uint32_t num);

	/// <summary>
	/// コマンドを積む
	/// </summary>
	void BindGraphicsCBV(UINT number);
	void BindGraphicsSRV(UINT number, uint32_t index);
	void BindGraphicsSRV_Instanced(UINT number);
	void BindComputeCBV(UINT number);
	void BindComputeSRV_Instanced(UINT number);
	void IASetVertexBuffers(UINT number);
	void IASetIndexBuffer();

	/// <summary>
	/// UAVのBarrierを張る
	/// </summary>
	void SetUAVBarrier();

#pragma region Accessor アクセッサ

	// Resource
	ID3D12Resource* GetResource() { return this->buffer_.Get(); }

	// VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW GetVBV() { return this->vertexBufferView_; }

	// IndexBufferView
	D3D12_INDEX_BUFFER_VIEW GetIBV() { return this->indexBufferView_; }

#pragma endregion 


private:

	/// <summary>
	/// CBVの生成
	/// </summary>
	void CreateCBVResource();

	/// <summary>
	/// UAVの作成
	/// </summary>
	void CreateUAVResource();


private: 

	// Resource
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;

	// VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	// IndexBufferView
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// 作成するResourceの数
	UINT itemCount_ = 1;

	// mappedData
	T* mappedData_{};

	// Index
	uint32_t srvIndex_ = 0;
	//uint32_t uavIndex_ = 0;

	// Size
	uint32_t resourceSize_ = 0;
};



/// <summary>
/// Resource作成
/// </summary>
template<typename T>
inline void BufferResource<T>::CreateCBV(UINT itemCount)
{
	// 作成するResourceの要素数
	this->itemCount_ = itemCount;

	// CBVの作成
	CreateCBVResource();
}


/// <summary>
/// VertexBufferViewの作成
/// </summary>
template<typename T>
inline void BufferResource<T>::CreateVertexBufferView()
{
	vertexBufferView_.BufferLocation = buffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(T) * itemCount_);
	vertexBufferView_.StrideInBytes = UINT(sizeof(T));
}


/// <summary>
/// IndexBufferViewの作成
/// </summary>
template<typename T>
inline void BufferResource<T>::CreateIndexBufferView()
{
	indexBufferView_.BufferLocation = buffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(T) * itemCount_);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
}


/// <summary>
/// InstancingResourceの作成
/// </summary>
template<typename T>
inline void BufferResource<T>::CreateInstancingResource(uint32_t instancingNum)
{
	DescriptorManager* descriptor = DescriptorManager::GetInstance();
	srvIndex_ = descriptor->CreateInstancingSRV(instancingNum, this->buffer_, UINT(sizeof(T)));
}


/// <summary>
/// UAVの作成
/// </summary>
template<typename T>
inline void BufferResource<T>::CreateUAV(UINT itemCount)
{
	// 作成するResourceの要素数
	this->itemCount_ = itemCount;

	// UAVの作成
	CreateUAVResource();
}


/// <summary>
/// データを書き込む
/// </summary>
template<typename T>
inline void BufferResource<T>::UpdateData(const T* data)
{
	// buffer_ が nullptr の場合はエラー
	if (!buffer_) {
		// ログを出力し、アサーションでプログラムを停止させる
		Log("constBuff_ is nullptr. Make sure to create constBuffer before calling UpdateData.");
		assert(false);
		return;
	}

	// Map
	HRESULT result;
	result = buffer_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));
	assert(SUCCEEDED(result));

	// WriteData
	assert(data != nullptr); // ポインタが有効か確認
	assert(mappedData_ != nullptr); // mappedData_ が初期化されているか確認
	std::memcpy(mappedData_, data, sizeof(T) * itemCount_);

	// UnMap
	if (buffer_ && mappedData_) {
		buffer_->Unmap(0, nullptr);
		mappedData_ = nullptr;
	}
}

template<typename T>
inline void BufferResource<T>::UpdateData(const std::vector<T>& datas, uint32_t num)
{
	// buffer_ が nullptr の場合はエラー
	if (!buffer_) {
		// ログを出力し、アサーションでプログラムを停止させる
		Log("constBuff_ is nullptr. Make sure to create constBuffer before calling UpdateData.");
		assert(false);
		return;
	}

	// Map
	HRESULT result;
	result = buffer_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));
	assert(SUCCEEDED(result));

	// WriteData
	// ベクトルが空でないことを確認
	assert(!datas.empty());
	// データ数が一致しているか確認する
	assert(num == itemCount_);
	// 実際のデータを書き込む処理
	std::memcpy(mappedData_, datas.data(), sizeof(T) * num);

	// UnMap
	if (buffer_ && mappedData_) {
		buffer_->Unmap(0, nullptr);
		mappedData_ = nullptr;
	}
}


/// <summary>
/// コマンドを積む
/// </summary>
template<typename T>
inline void BufferResource<T>::BindGraphicsCBV(UINT number)
{
	Commands commands = CommandManager::GetInstance()->GetCommands();
	commands.List->SetGraphicsRootConstantBufferView(number, buffer_->GetGPUVirtualAddress());
}

template<typename T>
inline void BufferResource<T>::BindGraphicsSRV(UINT number, uint32_t index)
{
	DirectXManager* dxCommon = DirectXManager::GetInstance();
	Commands commands = CommandManager::GetInstance()->GetCommands();
	DescriptorManager* descriptor = DescriptorManager::GetInstance();
	ID3D12DescriptorHeap* desc[] = { dxCommon->GetSrvDescriptorHeap() };
	commands.List->SetDescriptorHeaps(1, desc);
	commands.List->SetGraphicsRootDescriptorTable(number, descriptor->GetSRVHandle(index)._GPU);
}

template<typename T>
inline void BufferResource<T>::BindGraphicsSRV_Instanced(UINT number)
{
	DirectXManager* dxCommon = DirectXManager::GetInstance();
	Commands commands = CommandManager::GetInstance()->GetCommands();
	DescriptorManager* descriptor = DescriptorManager::GetInstance();
	ID3D12DescriptorHeap* desc[] = { dxCommon->GetSrvDescriptorHeap() };
	commands.List->SetDescriptorHeaps(1, desc);
	commands.List->SetGraphicsRootDescriptorTable(number, descriptor->GetSRVHandle(srvIndex_)._GPU);
}

template<typename T>
inline void BufferResource<T>::BindComputeCBV(UINT number)
{
	Commands commands = CommandManager::GetInstance()->GetCommands();
	commands.List->SetComputeRootConstantBufferView(number, buffer_->GetGPUVirtualAddress());
}

template<typename T>
inline void BufferResource<T>::BindComputeSRV_Instanced(UINT number)
{
	DirectXManager* dxCommon = DirectXManager::GetInstance();
	Commands commands = CommandManager::GetInstance()->GetCommands();
	DescriptorManager* descriptor = DescriptorManager::GetInstance();
	ID3D12DescriptorHeap* desc[] = { dxCommon->GetSrvDescriptorHeap() };
	commands.List->SetDescriptorHeaps(1, desc);
	commands.List->SetComputeRootDescriptorTable(number, descriptor->GetSRVHandle(srvIndex_)._GPU);
}

template<typename T>
inline void BufferResource<T>::IASetVertexBuffers(UINT number)
{
	Commands commands = CommandManager::GetInstance()->GetCommands();
	commands.List->IASetVertexBuffers(0, number, &vertexBufferView_);
}

template<typename T>
inline void BufferResource<T>::IASetIndexBuffer()
{
	Commands commands = CommandManager::GetInstance()->GetCommands();
	commands.List->IASetIndexBuffer(&indexBufferView_);
}


/// <summary>
/// UAVのBarrierを張る
/// </summary>
template<typename T>
inline void BufferResource<T>::SetUAVBarrier()
{
	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();
	// UAVBarrier
	D3D12_RESOURCE_BARRIER uavBarrier{};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	uavBarrier.UAV.pResource = buffer_.Get();
	commands.List->ResourceBarrier(1, &uavBarrier);
}


/// <summary>
/// CBVの生成
/// </summary>
template<typename T>
inline void BufferResource<T>::CreateCBVResource()
{
	// Resource用のHeap設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う

	// BufferResource。Textureの場合はまた別の設定をする
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	// Resourceのサイズ
	this->resourceSize_ = sizeof(T) * itemCount_;
	resourceDesc.Width = resourceSize_;

	// Bufferの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;

	// Bufferの場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 実際にBufferResourceを作る
	HRESULT hr_;
	hr_ = DirectXManager::GetInstance()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&buffer_));
	assert(SUCCEEDED(hr_));
	if (FAILED(hr_)) {
		// エラーハンドリング
		std::cerr << "CreateCommittedResource failed: " << std::hex << hr_ << std::endl;
	}
}


/// <summary>
/// UAVの作成
/// </summary>
template<typename T>
inline void BufferResource<T>::CreateUAVResource()
{
	// Resource用のHeap設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	//uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_DEFAULT; // DefaultHeapを使う

	// BufferResource。Textureの場合はまた別の設定をする
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	// 専用のフラグを立てる
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	// Resourceのサイズ
	this->resourceSize_ = sizeof(T) * itemCount_;
	resourceDesc.Width = resourceSize_;

	// Bufferの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;

	// Bufferの場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 実際にBufferResourceを作る
	HRESULT hr_;
	hr_ = DirectXManager::GetInstance()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr,
		IID_PPV_ARGS(&buffer_));

	// DescriptorManagerでUAVを作る
	DescriptorManager* descriptor = DescriptorManager::GetInstance();
	this->srvIndex_ = descriptor->CreateUAV(this->buffer_, sizeof(T), itemCount_);

	assert(SUCCEEDED(hr_));
	if (FAILED(hr_)) {
		// エラーハンドリング
		std::cerr << "CreateCommittedResource failed: " << std::hex << hr_ << std::endl;
	}
}
