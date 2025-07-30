#include "GaussianFilterEffect.h"



// 初期化処理
void GaussianFilterEffect::Init()
{
	Create();

	// MtlData初期値
	mtlData_.color = Vector4::one;
}


// 描画処理
void GaussianFilterEffect::Draw()
{
	// MtlBufferにMtlを書き込む
	mtlBuffer_.UpdateData((&mtlData_));

	// コマンドコール
	CommandCall();
}


// ImGuiの描画
void GaussianFilterEffect::DrawImGui([[maybe_unused]] std::string name)
{
#ifdef _DEBUG
	if (ImGui::TreeNode((name + "GaussianFilter").c_str())) {
		mtlData_.DrawImGui(name);
		ImGui::TreePop();
	}
#endif // _DEBUG
}


// コマンドコール
void GaussianFilterEffect::CommandCall()
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineの設定
	PipeLineManager::SetPipeLine(PipeLine::Container::Graphic, PipeLine::Category::GaussianFilter);


	// SRVをコマンドに積む
	//SRVManager::SetGraphicsRootDescriptorTable(3, srv_);
	renderTexBuffer_.BindGraphicsSRV(3, srv_);

	// MtlBufferをコマンドに積む
	mtlBuffer_.BindGraphicsCBV(4);

	// 描画
	commands.List->DrawInstanced(3, 1, 0, 0);
}