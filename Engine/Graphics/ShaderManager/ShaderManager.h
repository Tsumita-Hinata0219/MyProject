#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <string>
#include <wrl.h>
#include <format>
#include <iostream>
#include <map>

#include "Math/MyMath.h"
#include "Utilities/EnumUtilities/EnumUtilities.h"


struct Shaders {
	IDxcUtils* Utils;
	IDxcCompiler3* Compiler;
	IDxcIncludeHandler* includeHandler;
};

struct ShadersMode {
	IDxcBlob* VertexBlob;
	IDxcBlob* PixelBlob;
	IDxcBlob* ComputeBlob;
};

enum ShaderType {
	VS,
	PS,
	CS,
};


class ShaderManager {

private: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	ShaderManager() = default;
	~ShaderManager() = default;
	ShaderManager(const ShaderManager&) = delete;
	const ShaderManager& operator=(const ShaderManager&) = delete;


public: // メンバ関数

	/// <summary>
	/// ShaderManagerのインスタンス取得
	/// </summary>
	static ShaderManager* GetInstance() {
		static ShaderManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

#pragma region Get 取得

	/// <summary>
	/// 
	/// </summary>
	ShadersMode GetModelShader(const std::string key) { return this->modelShadersMap_[key]; }
	ShadersMode GetPostEffectShader(const std::string key) { return this->postEffectShadersMap_[key]; }
	ShadersMode GetComputeShader(const std::string key) { return this->computeShadersMap_[key]; }

#pragma endregion


private: // メンバ関数

	/// <summary>
	/// CompileShader関数
	/// </summary>
	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile);

	/// <summary>
	/// DXCの初期化
	/// </summary>
	void InitializeDXC();

	/// <summary>
	/// includeに対応するための設定
	/// </summary>
	void SettingIncludeHandler();

	/// <summary>
	/// シェーダーをコンパイルする
	/// </summary>
	void ModelShadersCompiles();
	void PostEffectShadersCompiles();
	void ComputeShadersCompiles();

	/// <summary>
	/// シェーダーをセットする
	/// </summary>
	IDxcBlob* SetShader(ShaderType type, const std::wstring& path);


#pragma region ModelShaders

	void Object2DShader();
	void Object3DShader();
	void SkinningObject3dShader();
	void SkyboxShader();
	void CPUParticleShader();
	void GPUParticle_Draw();
	void Decal();

#pragma endregion

#pragma region PostEffectShaders

	void AbsentShader();
	void BoxFilterShader();
	void ColorGrading();
	void DissolveShader();
	void GaussianFilterShader();
	void GlitchShader();
	void GrainShader();
	void GrayScaleShader();
	void HSVShader();
	void OutLineShader();
	void RadialBlurShader();
	void RandomShader();
	void RetroCRTShader();
	void SepiaToneShader();
	void VignettingShader();

#pragma endregion

#pragma region ComputeShaders

	void SphereEmitter();
	void ConstantField();
	void GPUParticle_Init();
	void GPUParticle_Update();
	void GPUParticle_FadeOut();

#pragma endregion


private: // メンバ変数

	Shaders dxc_{};
	std::map<std::string, ShadersMode> modelShadersMap_;
	std::map<std::string, ShadersMode> postEffectShadersMap_;
	std::map<std::string, ShadersMode> computeShadersMap_;
};