#include "ShaderManager.h"



/// <summary>
/// 初期化処理
/// </summary>
void ShaderManager::Initialize() {

	// DXCの初期化
	ShaderManager::GetInstance()->InitializeDXC();
	// includeに対応するための設定 
	ShaderManager::GetInstance()->SettingIncludeHandler();
	// シェーダーをコンパイルする
	ShaderManager::GetInstance()->ModelShadersCompiles();
	ShaderManager::GetInstance()->PostEffectShadersCompiles();
	ShaderManager::GetInstance()->ComputeShadersCompiles();
}



/// <summary>
/// CompileShader関数
/// </summary>
IDxcBlob* ShaderManager::CompileShader(const std::wstring& filePath, const wchar_t* profile) {

	IDxcUtils* dxcUtils = ShaderManager::GetInstance()->dxc_.Utils;
	IDxcCompiler3* dxcCompiler = ShaderManager::GetInstance()->dxc_.Compiler;
	IDxcIncludeHandler* includeHandler = ShaderManager::GetInstance()->dxc_.includeHandler;

	/* --- 1. hlslファイルを読む --- */

	// これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	IDxcBlobEncoding* shaderSource = nullptr;
	// hlslを読み込む
	HRESULT hr_ = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読み込めなかったら止める
	assert(SUCCEEDED(hr_));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer{};
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTF8の文字コードであることを通知


	/* --- 2.Compilerする --- */

	LPCWSTR arguments[] = {
		filePath.c_str(),  // コンパイル対象のhlslファイル名
		L"-E", L"main",  // エントリーポイントの指定。基本的にmain以外にはしない
		L"-T", profile,  // ShaderProfileの設定
		L"-Zi", L"-Qembed_debug",  // デバッグ用の情報を埋め込む
		L"-Od",  // 最適化を外しておく
		L"-Zpr",  // メモリレイアウトは行優先
	};
	// 実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr_ = dxcCompiler->Compile(
		&shaderSourceBuffer,        // 読み込んだファイル
		arguments,					// コンパイルオプション
		_countof(arguments),		// コンパイルオプションの数
		includeHandler,				// includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)	// コンパイル結果
	);
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr_));


	/* --- 3. 警告・エラーが出てないか確認する --- */

	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		// 警告・エラーゼッタイ
		Log(shaderError->GetStringPointer());
		assert(false);
	}


	/* --- 4. Compiler結果を受け取って返す --- */

	// コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr_ = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr_));
	// 成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeded, path{}, profile:{}\n", filePath, profile)));
	// もう使わないリソースを解放
	shaderResult->Release();
	shaderSource->Release();
	// 実行用のバイナリを返却
	return shaderBlob;
}



/// <summary>
/// DXCの初期化
/// </summary>
void ShaderManager::InitializeDXC() {

	HRESULT hr{};

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&ShaderManager::GetInstance()->dxc_.Utils));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&ShaderManager::GetInstance()->dxc_.Compiler));
	assert(SUCCEEDED(hr));
}



/// <summary>
/// includeに対応するための設定
/// </summary>
void ShaderManager::SettingIncludeHandler() {

	ShaderManager::GetInstance()->dxc_.Utils->CreateDefaultIncludeHandler(&ShaderManager::GetInstance()->dxc_.includeHandler);
}



/// <summary>
/// シェーダーをコンパイルする
/// </summary>
void ShaderManager::ModelShadersCompiles()
{
	Object2DShader();
	Object3DShader();
	SkinningObject3dShader();
	SkyboxShader();
	CPUParticleShader();
	GPUParticle_Draw();
	Decal();
}
void ShaderManager::PostEffectShadersCompiles()
{
	AbsentShader();
	BoxFilterShader();
	DissolveShader();
	GaussianFilterShader();
	GlitchShader();
	GrainShader();
	GrayScaleShader();
	HSVShader();
	OutLineShader();
	RadialBlurShader();
	RandomShader();
	RetroCRTShader();
	SepiaToneShader();
	VignettingShader();
}
void ShaderManager::ComputeShadersCompiles()
{
	SphereEmitter();
	ConstantField();
	GPUParticle_Init();
	GPUParticle_Update();
	GPUParticle_FadeOut();
}



/// <summary>
/// シェーダーをセットする
/// </summary>
IDxcBlob* ShaderManager::SetShader(ShaderType type, const std::wstring& path)
{
	std::wstring targetProfile{};

	if (type == VS) {
		targetProfile = L"vs_6_0";
	}
	else if (type == PS) {
		targetProfile = L"ps_6_0";
	}
	else if (type == CS) {
		targetProfile = L"cs_6_0";
	}

	return ShaderManager::CompileShader(path.c_str(), targetProfile.c_str());
}



void ShaderManager::Object2DShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/Object2d/Object2d.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/Object2d/Object2d.PS.hlsl"),
	};
	modelShadersMap_["Object2D"] = shader;
}

void ShaderManager::Object3DShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/Object3d/Object3d.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/Object3d/Object3d.PS.hlsl"),
	};
	modelShadersMap_["Object3D"] = shader;
}

void ShaderManager::SkinningObject3dShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/Skinning/SkinningObject3d.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/Skinning/SkinningObject3d.PS.hlsl"),
	};
	modelShadersMap_["SkinningObject3d"] = shader;
}

void ShaderManager::SkyboxShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/Skybox/Skybox.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/Skybox/Skybox.PS.hlsl"),
	};
	modelShadersMap_["Skybox"] = shader;
}

void ShaderManager::CPUParticleShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/CPUParticle/CPUParticle.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/CPUParticle/CPUParticle.PS.hlsl"),
	};
	modelShadersMap_["CPUParticle"] = shader;
}

void ShaderManager::GPUParticle_Draw()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/GPUParticle/Draw/GPUParticle_Draw.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/GPUParticle/Draw/GPUParticle_Draw.PS.hlsl"),
	};
	modelShadersMap_["GPUParticle_Draw"] = shader;
}

void ShaderManager::Decal()
{
	ShadersMode shader{
		   .VertexBlob = SetShader(VS, L"Resources/Shaders/Decal/Decal.VS.hlsl"),
		   .PixelBlob = SetShader(PS, L"Resources/Shaders/Decal/Decal.PS.hlsl"),
	};
	modelShadersMap_["Decal"] = shader;
}



void ShaderManager::AbsentShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/Absent/Absent.PS.hlsl"),
	};
	postEffectShadersMap_["Absent"] = shader;
}

void ShaderManager::BoxFilterShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/BoxFilter/BoxFilter.PS.hlsl"),
	};
	postEffectShadersMap_["BoxFilter"] = shader;
}

void ShaderManager::ColorGrading()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/ColorGrading/ColorGrading.PS.hlsl"),
	};
	postEffectShadersMap_["ColorGrading"] = shader;
}

void ShaderManager::DissolveShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/Dissolve/Dissolve.PS.hlsl"),
	};
	postEffectShadersMap_["Dissolve"] = shader;
}

void ShaderManager::GaussianFilterShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/GaussianFilter/GaussianFilter.PS.hlsl"),
	};
	postEffectShadersMap_["GaussianFilter"] = shader;
}

void ShaderManager::GlitchShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/Glitch/Glitch.PS.hlsl"),
	};
	postEffectShadersMap_["Glitch"] = shader;
}

void ShaderManager::GrainShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/Grain/Grain.PS.hlsl"),
	};
	postEffectShadersMap_["Grain"] = shader;
}

void ShaderManager::GrayScaleShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/GrayScale/GrayScale.PS.hlsl"),
	};
	postEffectShadersMap_["GrayScale"] = shader;
}

void ShaderManager::HSVShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/HSV/HSV.PS.hlsl"),
	};
	postEffectShadersMap_["HSV"] = shader;
}

void ShaderManager::OutLineShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/OutLine/OutLine.PS.hlsl"),
	};
	postEffectShadersMap_["OutLine"] = shader;
}

void ShaderManager::RadialBlurShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/RadialBlur/RadialBlur.PS.hlsl"),
	};
	postEffectShadersMap_["RadialBlur"] = shader;
}

void ShaderManager::RandomShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/Random/Random.PS.hlsl"),
	};
	postEffectShadersMap_["Random"] = shader;
}

void ShaderManager::RetroCRTShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/RetroCRT/RetroCRT.PS.hlsl"),
	};
	postEffectShadersMap_["RetroCRT"] = shader;
}

void ShaderManager::SepiaToneShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/SepiaTone/SepiaTone.PS.hlsl"),
	};
	postEffectShadersMap_["SepiaTone"] = shader;
}

void ShaderManager::VignettingShader()
{
	ShadersMode shader{
		.VertexBlob = SetShader(VS, L"Resources/Shaders/PostEffect/PostEffect.VS.hlsl"),
		.PixelBlob = SetShader(PS, L"Resources/Shaders/PostEffect/Vignetting/Vignetting.PS.hlsl"),
	};
	postEffectShadersMap_["Vignetting"] = shader;
}



void ShaderManager::SphereEmitter()
{
	ShadersMode shader = {
		.ComputeBlob = SetShader(CS, L"Resources/Shaders/Emitter/Sphere/SphereEmitter.CS.hlsl"),
	};
	computeShadersMap_["SphereEmitter"] = shader;
}

void ShaderManager::ConstantField()
{
	ShadersMode shader = {
		.ComputeBlob = SetShader(CS, L"Resources/Shaders/Field/Constant/ConstantField.CS.hlsl"),
	};
	computeShadersMap_["ConstantField"] = shader;
}


void ShaderManager::GPUParticle_Init()
{
	ShadersMode shader = {
		.ComputeBlob = SetShader(CS, L"Resources/Shaders/GPUParticle/Init/GPUParticle_Init.CS.hlsl"),
	};
	computeShadersMap_["GPUParticle_Init"] = shader;
}

void ShaderManager::GPUParticle_Update()
{
	ShadersMode shader = {
		.ComputeBlob = SetShader(CS, L"Resources/Shaders/GPUParticle/Update/GPUParticle_Update.CS.hlsl"),
	};
	computeShadersMap_["GPUParticle_Update"] = shader;
}

void ShaderManager::GPUParticle_FadeOut()
{
	ShadersMode shader = {
		.ComputeBlob = SetShader(CS, L"Resources/Shaders/GPUParticle/Material/FadeOut/GPUParticle_FadeOut.CS.hlsl"),
	};
	computeShadersMap_["GPUParticle_FadeOut"] = shader;
}
