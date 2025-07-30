#pragma once

#include "../../IPipeLineState/IPipeLineState.h"


/* SepiaTonePipeLineクラス */
class SepiaTonePipeLine : public IPipeLineState {

public:

	// Psoを構築する
	PsoProperty SetUpPso() override;


protected:

	// RootSignatureのセットアップ
	void SetUpRootSignature(D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature) override;

};

