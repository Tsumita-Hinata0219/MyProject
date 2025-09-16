#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <cassert>
#include <map>
#include <unordered_map>
#include<vector>


/* リソース管理クラス */
class ResourceManager {

public:

	enum class ResourceType {
		DYNAMIC,
		STATIC,
		MODEL,
	};
	struct ResourceInfo {
		std::shared_ptr<ID3D12Resource> resource;
		ResourceType type;
		std::string filePath;
	};

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResourceManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResourceManager() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();


private:
	std::unordered_map<uint32_t, ResourceInfo> resources_;
};