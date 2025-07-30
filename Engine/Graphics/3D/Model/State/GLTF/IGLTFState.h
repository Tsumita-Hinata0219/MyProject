#pragma once

#include "../IModelState.h"


/* GLTFState */
class IGLTFState : public IModelState {

public:

	// コンストラクタ、デストラクタ
	IGLTFState() {};
	IGLTFState(ModelDatas datas);
	~IGLTFState() {};

	// 描画処理
	void Draw(Transform& transform) override;

	// コマンドコール
	void CommandCall() override;

private:


};
