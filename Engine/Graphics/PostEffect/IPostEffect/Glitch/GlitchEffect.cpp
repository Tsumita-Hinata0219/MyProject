#include "GlitchEffect.h"
#include "System/GameManager/GameManager.h"


// 初期化処理
void GlitchEffect::Init()
{
	Create();

	mtlData_.color = Vector4::one;
	mtlData_.maskTexture = TextureManager::LoadTexture("Texture", "noise4.png");
	mtlData_.time = 0.0f;
}


// 描画処理
void GlitchEffect::Draw()
{
	mtlData_.time = g_ElapsedTime;

	// MtlBufferにMtlを書き込む
	mtlBuffer_.UpdateData((&mtlData_));

	// コマンドコール
	CommandCall();
}


// ImGuiの描画
void GlitchEffect::DrawImGui([[maybe_unused]] std::string name)
{
#ifdef _DEBUG
	if (ImGui::TreeNode((name + "Glitch").c_str())) {
		mtlData_.DrawImGui(name);
		ImGui::TreePop();
	}
#endif // _DEBUG
}


// コマンドコール
void GlitchEffect::CommandCall()
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineの設定
	PipeLineManager::SetPipeLine(PipeLine::Container::Graphic, PipeLine::Category::Glitch);


	// SRVをコマンドに積む
	//SRVManager::SetGraphicsRootDescriptorTable(3, srv_);
	renderTexBuffer_.BindGraphicsSRV(3, srv_);

	// MtlBufferをコマンドに積む
	mtlBuffer_.BindGraphicsCBV(4);

	// MaksTexture
	//SRVManager::SetGraphicsRootDescriptorTable(5, mtlData_.maskTexture);
	mtlBuffer_.BindGraphicsSRV(5, mtlData_.maskTexture);

	// 描画
	commands.List->DrawInstanced(3, 1, 0, 0);
}