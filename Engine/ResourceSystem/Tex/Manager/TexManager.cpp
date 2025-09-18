#include "TexManager.h"
#include "Core/DirectXManager/DirectXManager.h"
//
//void TexManager::Init()
//{
//	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
//	assert(SUCCEEDED(hr));
//
//	// Fenceの作成
//	CreateFence();
//}
//
//void TexManager::Finalize() const
//{
//	CoUninitialize();
//	CloseHandle(fenceEvent_);
//}
//
//void TexManager::Load(const std::string& rootPath, const std::string& fileName)
//{
//	// フルファイルパス
//	std::string fullPath = "Resources/" + rootPath + "/" + fileName;
//	// アクセスキー
//	std::uint32_t key = FNV1aHash(fullPath);
//
//	// 検索し引っかかればそのままreturn
//	auto it = texMap_.find(key);
//	if (it != texMap_.end()) {
//		return;
//	}
//
//	// 新しくロード
//	// 新しく作るTexInfo
//	std::unique_ptr<TexInfo> texInfo = std::make_unique<TexInfo>();
//	// 拡張子の抽出
//	std::string ext = GetExtension(fileName);
//
//	// なければ新しく作る。拡張子で処理を変える
//	if (ext == TexFileFormat::PNG.first) {
//
//	}
//	else if (ext == TexFileFormat::JPEG.first) {
//
//	}
//	else if (ext == TexFileFormat::DSS.first) {
//
//	}
//
//	// mapに保存
//	texMap_[key] = std::move(texInfo);
//}
//
//void TexManager::CreateFence()
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