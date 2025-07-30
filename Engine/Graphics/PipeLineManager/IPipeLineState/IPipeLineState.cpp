#include "IPipeLineState.h"
#include "../../ShaderManager/ShaderManager.h"


/// <summary>
/// RootParameterのセットアップ
/// </summary>
D3D12_ROOT_PARAMETER IPipeLineState::SetUpRootParam(RootParam::BuffType buffType, RootParam::ShaderType shaderType, UINT registerNum)
{
	D3D12_ROOT_PARAMETER rootParam{};

	// Resourceのタイプ
	if (buffType == RootParam::BuffType::CBV) {
		SetUpRootParam_CBV(rootParam, shaderType, registerNum);
	}
	else if (buffType == RootParam::BuffType::SRV) {
		SetUpRootParam_SRV(rootParam, shaderType, registerNum);
	}
	else if (buffType == RootParam::BuffType::UAV) {
		SetUpRootParam_UAV(rootParam, shaderType, registerNum);
	}

	return rootParam;
}


/// <summary>
/// RootSignatureのセットアップ
/// </summary>
D3D12_INPUT_ELEMENT_DESC IPipeLineState::SetUpInputElementDescs(LPCSTR SemanticName)
{
	D3D12_INPUT_ELEMENT_DESC inputElementDescs{};

	if (strcmp(SemanticName, "POSITION") == 0)
	{
		inputElementDescs.SemanticName = "POSITION";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "TEXCOORD") == 0) 
	{
		inputElementDescs.SemanticName = "TEXCOORD";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "NORMAL") == 0)
	{
		inputElementDescs.SemanticName = "NORMAL";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "WORLDPOSITION") == 0)
	{
		inputElementDescs.SemanticName = "WORLDPOSITION";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "CAMERAPOSITION") == 0)
	{
		inputElementDescs.SemanticName = "CAMERAPOSITION";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "COLOR") == 0)
	{
		inputElementDescs.SemanticName = "COLOR";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "WEIGHT") == 0)
	{
		inputElementDescs.SemanticName = "WEIGHT";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs.InputSlot = 1;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "INDEX") == 0)
	{
		inputElementDescs.SemanticName = "INDEX";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32A32_SINT;
		inputElementDescs.InputSlot = 1;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "INSTANCEID") == 0)
	{
		inputElementDescs.SemanticName = "INSTANCEID";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32_UINT;
		inputElementDescs.InputSlot = 1;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}

	return inputElementDescs;
}


/// <summary>
/// InputLayoutのセットアップ
/// </summary>
void IPipeLineState::SetUpInputLayout(D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements)
{
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = numInputElements;
}


/// <summary>
/// BlendStateのセットアップ
/// </summary>
void IPipeLineState::SetUpBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode)
{
	blendDesc.RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.BlendEnable = true;


	switch (blendMode)
	{
	case None:
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		break;

	case Add:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case Subtruct:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case Multiply:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case Screen:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;

	default:
		break;
	}
}


/// <summary>
/// RasterizerStateのセットアップ
/// </summary>
void IPipeLineState::SetUpRasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc, D3D12_CULL_MODE Cull, D3D12_FILL_MODE fill)
{
	// 裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = Cull;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = fill;
}


/// <summary>
/// Shadersのコンパイル
/// </summary>
void IPipeLineState::SetUpModelShader(IDxcBlob*& vertexShaderBlob, IDxcBlob*& pixelShaderBlob, const std::string key)
{
	vertexShaderBlob = ShaderManager::GetInstance()->GetModelShader(key).VertexBlob;
	assert(vertexShaderBlob != nullptr);

	pixelShaderBlob = ShaderManager::GetInstance()->GetModelShader(key).PixelBlob;
	assert(pixelShaderBlob != nullptr);
}
void IPipeLineState::SetUpPostEffectShader(IDxcBlob*& vertexShaderBlob, IDxcBlob*& pixelShaderBlob, const std::string key)
{
	vertexShaderBlob = ShaderManager::GetInstance()->GetPostEffectShader(key).VertexBlob;
	assert(vertexShaderBlob != nullptr);

	pixelShaderBlob = ShaderManager::GetInstance()->GetPostEffectShader(key).PixelBlob;
	assert(pixelShaderBlob != nullptr);
}


/// <summary>
/// 各ルートパラメーターの設定
/// </summary>
void IPipeLineState::SetUpRootParam_CBV(D3D12_ROOT_PARAMETER& rootParam, RootParam::ShaderType shaderType, UINT registerNum)
{
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // ResourceType
	rootParam.ShaderVisibility = GetShaderVisibility(shaderType); // Shader使用場所
	rootParam.Descriptor.ShaderRegister = registerNum; // Register番号
}
void IPipeLineState::SetUpRootParam_SRV(D3D12_ROOT_PARAMETER& rootParam, RootParam::ShaderType shaderType, UINT registerNum)
{
	D3D12_DESCRIPTOR_RANGE materiaTexture[1]{};
	materiaTexture[0].BaseShaderRegister = registerNum; // Register番号
	materiaTexture[0].NumDescriptors = 1;
	materiaTexture[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // ResourceType
	materiaTexture[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam.ShaderVisibility = GetShaderVisibility(shaderType); // Shader使用場所
	rootParam.DescriptorTable.pDescriptorRanges = materiaTexture;
	rootParam.DescriptorTable.NumDescriptorRanges = _countof(materiaTexture);
}
void IPipeLineState::SetUpRootParam_UAV(D3D12_ROOT_PARAMETER& rootParam, RootParam::ShaderType shaderType, UINT registerNum)
{
	D3D12_DESCRIPTOR_RANGE materiaTexture[1]{};
	materiaTexture[0].BaseShaderRegister = registerNum; // Register番号
	materiaTexture[0].NumDescriptors = 1;
	materiaTexture[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV; // ResourceType
	materiaTexture[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam.ShaderVisibility = GetShaderVisibility(shaderType); // Shader使用場所
	rootParam.DescriptorTable.pDescriptorRanges = materiaTexture;
	rootParam.DescriptorTable.NumDescriptorRanges = _countof(materiaTexture);
}


/// <summary>
/// ShaderVisibilityの取得
/// </summary>
D3D12_SHADER_VISIBILITY IPipeLineState::GetShaderVisibility(RootParam::ShaderType shaderType)
{
	if (shaderType == RootParam::ShaderType::VERTEX) {
		return D3D12_SHADER_VISIBILITY_VERTEX;
	}
	else if (shaderType == RootParam::ShaderType::PIXEL) {
		return D3D12_SHADER_VISIBILITY_PIXEL;
	}
	else if (shaderType == RootParam::ShaderType::COMPUTE) {
		return D3D12_SHADER_VISIBILITY_ALL;
	}

	return D3D12_SHADER_VISIBILITY_ALL;
}

