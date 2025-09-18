#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <cassert>
#include <map>
#include<vector>

#include "Math/MyMath.h"
#include "ResourceSystem/Tex/Resource/TextureResource.h"


/* テクスチャリソース管理クラス */
class TexManager {

private: // シングルトン

	TexManager() = default;
	~TexManager() = default;
	TexManager(const TexManager&) = delete;
	const TexManager& operator=(const TexManager&) = delete;

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static TexManager* GetInstance() {
		static TexManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

private:

	// テクスチャのコンテナマップ

};

