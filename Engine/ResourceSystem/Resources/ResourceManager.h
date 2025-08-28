#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <cassert>
#include <map>
#include<vector>


/* リソース管理クラス */
class ResourceManager {

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

	void Register();




private:

	 
};