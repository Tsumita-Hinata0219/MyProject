#include "BoxFilterEffect.h"



// 初期化処理
void BoxFilterEffect::Init()
{
	Create();

	// MtlData初期値
	mtlData_.color = Vector4::one;
}


// 描画処理
void BoxFilterEffect::Draw()
{
	// MtlBufferにMtlを書き込む
	mtlBuffer_.UpdateData(&mtlData_);

	// コマンドコール
	CommandCall();
}


// ImGuiの描画
void BoxFilterEffect::DrawImGui([[maybe_unused]] std::string name)
{
#ifdef _DEBUG
	if (ImGui::TreeNode((name + "BoxFilter").c_str())) {
		mtlData_.DrawImGui(name);
		ImGui::TreePop();
	}
#endif // _DEBUG
}


// コマンドコール
void BoxFilterEffect::CommandCall()
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineの設定
	PipeLineManager::SetPipeLine(PipeLine::Container::Graphic, PipeLine::Category::BoxFilter);


	// RenderTex
	/*SRVManager::SetGraphicsRootDescriptorTable(3, srv_);*/
	renderTexBuffer_.BindGraphicsSRV(3, srv_);

	// MtlBuffer
	mtlBuffer_.BindGraphicsCBV(4);

	// 描画
	commands.List->DrawInstanced(3, 1, 0, 0);
}
