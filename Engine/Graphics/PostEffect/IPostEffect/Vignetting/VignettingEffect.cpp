#include "VignettingEffect.h"



// 初期化処理
void VignettingEffect::Init()
{
	Create();
}


// 描画処理
void VignettingEffect::Draw()
{
	// MtlBufferにMtlを書き込む
	mtlBuffer_.UpdateData((&mtlData_));

	// コマンドコール
	CommandCall();
}


// ImGuiの描画
void VignettingEffect::DrawImGui([[maybe_unused]] std::string name)
{
#ifdef _DEBUG
	if (ImGui::TreeNode((name + "Vignetting").c_str())) {
		mtlData_.DrawImGui(name);
		ImGui::TreePop();
	}
#endif // _DEBUG
}


// コマンドコール
void VignettingEffect::CommandCall()
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineの設定
	PipeLineManager::SetPipeLine(PipeLine::Container::Graphic, PipeLine::Category::Vignetting);


	// SRVをコマンドに積む
	//SRVManager::SetGraphicsRootDescriptorTable(3, srv_);
	renderTexBuffer_.BindGraphicsSRV(3, srv_);

	// MtlBufferをコマンドに積む
	mtlBuffer_.BindGraphicsCBV(4);

	// 描画
	commands.List->DrawInstanced(3, 1, 0, 0);
}
