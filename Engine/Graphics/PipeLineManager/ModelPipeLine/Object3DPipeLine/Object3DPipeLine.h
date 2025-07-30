#pragma once

#include "../../IPipeLineState/IPipeLineState.h"


/* Object3DPipeLineクラス */
class Object3DPipeLine : public IPipeLineState {

public:

	/// <summary>
	/// PSOを構築する
	/// </summary>
	PsoProperty SetUpPso() override;

#pragma region Accessor アクセッサ

	// CullModeの設定
	void SetCullMode(const D3D12_CULL_MODE& setMode) { this->cullMode_ = setMode; }

	void SetFillMode(const D3D12_FILL_MODE& setMode) { this->fillMode_ = setMode; }

	// DepthWriteMask
	void SetUpDepthWriteMask(const D3D12_DEPTH_WRITE_MASK& depthWriteMask) {this->depthWriteMask_ = depthWriteMask; }

#pragma endregion 


protected:

	/// <summary>
	/// RootSignatureのセットアップ
	/// </summary>
	void SetUpRootSignature(D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature) override;


private:

	// Cull_Mode
	D3D12_CULL_MODE cullMode_ = D3D12_CULL_MODE_BACK;

	// Fill_Mode
	D3D12_FILL_MODE fillMode_ = D3D12_FILL_MODE_SOLID;

	// DepthWriteMask
	bool depthWriteMask_ = D3D12_DEPTH_WRITE_MASK_ALL;

};

