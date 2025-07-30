#include "Decal.h"
#include "System/Camera/Manager/CameraManager.h"
#include "Graphics/PipeLineManager/PipeLineManager.h"

Decal::Decal()
{
	cameraMgr_ = CameraManager::GetInstance();
	pipeLineMgr_ = PipeLineManager::GetInstance();
	buffer_ = std::make_unique<BufferResource<DecalData>>();
}

void Decal::Init(uint32_t texHandle)
{
	texHandle_ = texHandle;
}

void Decal::Update()
{

}

void Decal::Draw()
{
	// 描画データの更新
	Update_Data();
	// PipeLineのチェック
	Check_PipeLine();
	// 描画データのバインド
	Bind_Data();
	// Draw!
	Execute_Draw();
}

void Decal::Create_Buffer()
{
	buffer_->CreateCBV();
}

void Decal::Update_Data()
{
	buffer_->UpdateData(&data_);
}

void Decal::Check_PipeLine()
{
	pipeLineMgr_->SetPipeLine(PipeLine::Container::Graphic,
		PipeLine::Category::Decal, PipeLine::SubFilter::None);
}

void Decal::Bind_Data()
{
	// Camera VS
	cameraMgr_->Bind_CameraData(0);

	// DecalData VS
	buffer_->BindGraphicsCBV(1);

	// Camera PS
	cameraMgr_->Bind_CameraData(2);

	// DecalData PS
	buffer_->BindGraphicsCBV(3);

	// Texture
	buffer_->BindGraphicsSRV(4, texHandle_);
}

void Decal::Execute_Draw()
{
	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();
	// Draw!!
	commands.List->DrawIndexedInstanced(0, 1, 0, 0, 0);
}
