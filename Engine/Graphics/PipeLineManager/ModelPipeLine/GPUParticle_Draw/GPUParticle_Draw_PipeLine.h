#pragma once

#include "../../IPipeLineState/IPipeLineState.h"


/* GPUParticle_Draw_PipeLineクラス */
class GPUParticle_Draw_PipeLine : public IPipeLineState {

public:

	/// <summary>
	/// PSOを構築する
	/// </summary>
	PsoProperty SetUpPso() override;

#pragma region Accessor アクセッサ

	// CullModeの設定
	void SetCUllMode(const D3D12_CULL_MODE& setMode) { this->cullMode_ = setMode; }

	// DepthWriteMask
	void SetUpDepthWriteMask(const D3D12_DEPTH_WRITE_MASK& depthWriteMask) { this->depthWriteMask_ = depthWriteMask; }

#pragma endregion 


protected:

	/// <summary>
	/// RootSignatureのセットアップ
	/// </summary>
	void SetUpRootSignature(D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature) override;


private:

	// Cull_Mode
	D3D12_CULL_MODE cullMode_ = D3D12_CULL_MODE_BACK;

	// DepthWriteMask
	bool depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;

};