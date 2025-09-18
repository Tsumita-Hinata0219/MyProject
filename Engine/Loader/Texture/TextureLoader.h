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



namespace TexFileFormat {
const std::pair<std::string, uint32_t> PNG = { ".png", 0 };
const std::pair<std::string, uint32_t> JPEG = { ".jpeg", 1 };
const std::pair<std::string, uint32_t> DSS = { ".dds", 2 };
};

/* テクスチャのローダー */
class TextureLoader {

private: // シングルトン

	// コンストラクタ、デストラクタ
	TextureLoader() = default;
	~TextureLoader() = default;
	TextureLoader(const TextureLoader&) = delete;
	const TextureLoader& operator=(const TextureLoader&) =  delete;

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static TextureLoader* GetInstance() {
		static TextureLoader instance;
		return &instance;
	}

	/// <summary>
	/// テクスチャのロード
	/// </summary>
	void LoadTexture(const std::string& rootPath, const std::string& fileName);
	
};


