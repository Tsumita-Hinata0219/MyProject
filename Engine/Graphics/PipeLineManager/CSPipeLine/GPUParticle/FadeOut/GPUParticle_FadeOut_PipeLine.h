#pragma once

#include "../../../IPipeLineState/IPipeLineState.h"

/* GPUParticle_FadeOut_PipeLineクラス */
class GPUParticle_FadeOut_PipeLine : public IPipeLineState {

public:

	// Psoを構築する
	PsoProperty SetUpPso() override;


protected:

	// RootSignatureのセットアップ
	void SetUpRootSignature(D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature) override;

};

