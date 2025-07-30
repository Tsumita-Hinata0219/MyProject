#include "IRenderComponent.h"
#include "Graphics/3D/Model/Manager/ModelManager.h"


/// <summary>
/// 生成
/// </summary>
void IRenderComponent::Create(const std::string& renderDataName)
{
	renderState_ = std::make_unique<RenderSystem::RenderState>(owner_);
	renderState_->SetRenderData(renderDataName);
	renderState_->Create_RenderBuffer();
}


/// <summary>
/// 描画処理
/// </summary>
void IRenderComponent::Draw()
{
	renderState_->Draw3D();
}
