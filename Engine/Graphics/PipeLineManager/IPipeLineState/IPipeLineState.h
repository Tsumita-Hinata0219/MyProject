#pragma once

#include <d3dx12.h>
#include <dxcapi.h>

#include <map>

#include "Math/MyMath.h"
#include "Math/Struct.h"
#include "Core/DirectXManager/DirectXManager.h"
#include "PipeLineStructure.h"
#include "Graphics/ShaderManager/ShaderManager.h"


/* IPipeLineStateクラス */
class IPipeLineState {

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~IPipeLineState() = default;

	/// <summary>
	/// Psoを構築する
	/// </summary>
	/// <returns></returns>
	virtual PsoProperty SetUpPso() = 0;

#pragma region Accessor アクセッサ

	// Psoの取得
	PsoProperty GetPsoProperty() const { return pso_; }

#pragma endregion


protected: // メンバ関数

	/// <summary>
	/// RootParameterの設定
	/// </summary>
	D3D12_ROOT_PARAMETER SetUpRootParam(RootParam::BuffType buffType, RootParam::ShaderType shaderType, UINT registerNum);

	/// <summary>
	/// RootSignatureの設定
	/// </summary
	virtual void SetUpRootSignature(D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature) = 0;

	/// <summary>
	/// InputLayoutの設定
	/// </summary>
	D3D12_INPUT_ELEMENT_DESC SetUpInputElementDescs(LPCSTR SemanticName);
	void SetUpInputLayout(D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements);

	/// <summary>
	/// BlendStateの設定
	/// </summary>
	void SetUpBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode);

	/// <summary>
	/// RasterizerStateの設定
	/// </summary>
	void SetUpRasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc, D3D12_CULL_MODE Cull, D3D12_FILL_MODE fill);

	/// <summary>
	/// Shadersのコンパイル
	/// </summary>
	void SetUpModelShader(IDxcBlob*& vertexShaderBlob, IDxcBlob*& pixelShaderBlob, const std::string key);
	void SetUpPostEffectShader(IDxcBlob*& vertexShaderBlob, IDxcBlob*& pixelShaderBlob, const std::string key);


private:

	/// <summary>
	/// 各ルートパラメーターの設定
	/// </summary>
	void SetUpRootParam_CBV(D3D12_ROOT_PARAMETER& rootParam, RootParam::ShaderType shaderType, UINT registerNum);
	void SetUpRootParam_SRV(D3D12_ROOT_PARAMETER& rootParam, RootParam::ShaderType shaderType, UINT registerNum);
	void SetUpRootParam_UAV(D3D12_ROOT_PARAMETER& rootParam, RootParam::ShaderType shaderType, UINT registerNum);

	/// <summary>
	/// ShaderVisibilityの取得
	/// </summary>
	D3D12_SHADER_VISIBILITY GetShaderVisibility(RootParam::ShaderType shaderType);


protected: // メンバ変数

	// PipeLineのデータ
	PsoProperty pso_{};

};

