#include "RetroCRTEffect.h"
#include "System/GameManager/GameManager.h"



// 初期化処理
void RetroCRTEffect::Init()
{
	Create();

	mtlData_.resolution = WinApp::WindowSize();
}


// 描画処理
void RetroCRTEffect::Draw()
{
	mtlData_.time = g_ElapsedTime;

	// MtlBufferにMtlを書き込む
	mtlBuffer_.UpdateData(&mtlData_);

	// コマンドコール
	CommandCall();
}


// ImGuiの描画
void RetroCRTEffect::DrawImGui([[maybe_unused]] std::string name)
{
#ifdef _DEBUG
	if (ImGui::TreeNode((name + "RetroCRT").c_str())) {
		mtlData_.DrawImGui(name);
		ImGui::TreePop();
	}
#endif // _DEBUG
}


// コマンドコール
void RetroCRTEffect::CommandCall()
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineの設定
	PipeLineManager::SetPipeLine(PipeLine::Container::Graphic, PipeLine::Category::RetroCRT);


	// SRVをコマンドに積む
	//SRVManager::SetGraphicsRootDescriptorTable(3, srv_);
	renderTexBuffer_.BindGraphicsSRV(3, srv_);

	// MtlBufferをコマンドに積む
	mtlBuffer_.BindGraphicsCBV(4);

	// 描画
	commands.List->DrawInstanced(3, 1, 0, 0);
}
