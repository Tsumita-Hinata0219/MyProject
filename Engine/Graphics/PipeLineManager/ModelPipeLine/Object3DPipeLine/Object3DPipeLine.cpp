#include "Object3DPipeLine.h"



// Psoを構築する
PsoProperty Object3DPipeLine::SetUpPso()
{
	/* --- RootSignatureを作成 --- */
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	SetUpRootSignature(descriptionRootSignature);


	/* --- InputLayoutを設定する --- */
	std::array<D3D12_INPUT_ELEMENT_DESC, 4> inputElementDesc = {
		SetUpInputElementDescs("POSITION"),
		SetUpInputElementDescs("TEXCOORD"),
		SetUpInputElementDescs("NORMAL"),
		SetUpInputElementDescs("WORLDPOSITION"),
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	SetUpInputLayout(inputLayoutDesc, inputElementDesc.data(), static_cast<UINT>(inputElementDesc.size()));


	/* --- BlendStateを設定する --- */
	// 全ての色要素を書き込む
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RENDER_TARGET_BLEND_DESC& pBlendDesc = blendDesc.RenderTarget[0];
	SetUpBlendState(pBlendDesc, None);


	/* --- RasiterzerStateを設定する --- */
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	SetUpRasterizerState(rasterizerDesc, cullMode_, fillMode_);


	/* --- Shaderを設定する --- */
	// Shaderをコンパイルする
	IDxcBlob* vertexShaderBlob = nullptr;
	IDxcBlob* pixelShaderBlob = nullptr;
	SetUpModelShader(vertexShaderBlob, pixelShaderBlob, "Object3D");


	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = pso_.rootSignature; // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // InputLayout
	graphicsPipelineStateDesc.BlendState = blendDesc; // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // RasterizeState

	graphicsPipelineStateDesc.VS = {
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize() }; // VertexShader

	graphicsPipelineStateDesc.PS = {
		pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize() }; // PixelShader


	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;


	// 利用するトポロジ(形状)もタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pso_.primitiveTopologyType =
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	// どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = depthWriteMask_;
	// 書き込む
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


	// 実際に生成
	HRESULT hr{};
	hr = DirectXManager::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
		&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&pso_.graphicsPipelineState));
	assert(SUCCEEDED(hr));

	return pso_;
}


// RootSignatureのセットアップ
void Object3DPipeLine::SetUpRootSignature(D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature)
{
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


	D3D12_ROOT_PARAMETER rootParameters[12]{};

	// ─── VS
	// b0 : 頂点位置
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;// VertexShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;// レジスタ番号

	// ─── PS
	// b0 : マテリアル
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 0; // レジスタ番号

	// b1 : カメラ
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[2].Descriptor.ShaderRegister = 1; // レジスタ番号

	// b2 : ライト
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[3].Descriptor.ShaderRegister = 2; // レジスタ番号

	// b3 : 環境マップ
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[4].Descriptor.ShaderRegister = 3; // レジスタ番号

	// b4 : 色加算
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[5].Descriptor.ShaderRegister = 4; // レジスタ番号

	// b5 : 法線マップ
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[6].Descriptor.ShaderRegister = 5; // レジスタ番号

	// b6 : デカール
	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // CBVを使う
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[7].Descriptor.ShaderRegister = 6; // レジスタ番号

	// t0 : モデルテクスチャ
	D3D12_DESCRIPTOR_RANGE materiaTexture[1]{};
	materiaTexture[0].BaseShaderRegister = 0; // レジスタ番号
	materiaTexture[0].NumDescriptors = 1; // 数は1つ
	materiaTexture[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	materiaTexture[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // offsetを自動計算
	rootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixShaderで使う
	rootParameters[8].DescriptorTable.pDescriptorRanges = materiaTexture; // Tableの中身の配列を指定
	rootParameters[8].DescriptorTable.NumDescriptorRanges = _countof(materiaTexture); // Tableで利用する

	// t1 : 環境マップテクスチャ
	D3D12_DESCRIPTOR_RANGE environmentTexture[1]{};
	environmentTexture[0].BaseShaderRegister = 1; // レジスタ番号
	environmentTexture[0].NumDescriptors = 1; // 数は1つ
	environmentTexture[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	environmentTexture[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // offsetを自動計算
	rootParameters[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixShaderで使う
	rootParameters[9].DescriptorTable.pDescriptorRanges = environmentTexture; // Tableの中身の配列を指定
	rootParameters[9].DescriptorTable.NumDescriptorRanges = _countof(environmentTexture); // Tableで利用する

	// t2 : 法線マップテクスチャ
	D3D12_DESCRIPTOR_RANGE normalMapTexture[1]{};
	normalMapTexture[0].BaseShaderRegister = 2; // レジスタ番号
	normalMapTexture[0].NumDescriptors = 1; // 数は1つ
	normalMapTexture[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	normalMapTexture[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // offsetを自動計算
	rootParameters[10].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[10].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixShaderで使う
	rootParameters[10].DescriptorTable.pDescriptorRanges = normalMapTexture; // Tableの中身の配列を指定
	rootParameters[10].DescriptorTable.NumDescriptorRanges = _countof(normalMapTexture); // Tableで利用する

	// t3 : デカールテクスチャ
	D3D12_DESCRIPTOR_RANGE decalTexture[1]{};
	decalTexture[0].BaseShaderRegister = 3; // レジスタ番号
	decalTexture[0].NumDescriptors = 1; // 数は1つ
	decalTexture[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	decalTexture[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // offsetを自動計算
	rootParameters[11].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[11].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixShaderで使う
	rootParameters[11].DescriptorTable.pDescriptorRanges = decalTexture; // Tableの中身の配列を指定
	rootParameters[11].DescriptorTable.NumDescriptorRanges = _countof(decalTexture); // Tableで利用する

	// s0 : サンプラー
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1]{};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0～1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う


	descriptionRootSignature.pParameters = rootParameters; // ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters); // 配列の長さ
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);


	// シリアライズしてバイナリにする
	HRESULT hr_ = D3D12SerializeRootSignature(
		&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&pso_.signatureBlob,
		&pso_.errorBlob);
	if (FAILED(hr_)) {
		Log(reinterpret_cast<char*>(pso_.errorBlob->GetBufferPointer()));
		assert(false);
	}


	// バイナリを元に生成
	hr_ = DirectXManager::GetInstance()->GetDevice()->CreateRootSignature(
		0,
		pso_.signatureBlob->GetBufferPointer(),
		pso_.signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&pso_.rootSignature));
	assert(SUCCEEDED(hr_));
}
