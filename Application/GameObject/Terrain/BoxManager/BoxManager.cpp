#include "BoxManager.h"



/// <summary>
/// 初期化処理
/// </summary>
void BoxManager::Init()
{
}


/// <summary>
/// 更新処理
/// </summary>
void BoxManager::Update()
{
	// BoxListの更新処理
	for (std::shared_ptr<IBoxObject> box : boxs_) {
		box->Update();
	}
	// 死亡フラグが立っていたら削除
	boxs_.remove_if([this](std::shared_ptr<IBoxObject> box) {
		if (box->IsDead()) {
			box.reset();
			return true;
		}
		return false;
		}
	);
}


/// <summary>
/// 描画処理
/// </summary>
void BoxManager::Draw3D()
{
	for (std::shared_ptr<IBoxObject> box : boxs_) {
		box->Draw3D();
	}
}


/// <summary>
/// Jsonで読み込んだ情報を受け取る
/// </summary>
void BoxManager::LoadEntityData(const std::vector<std::unique_ptr<EntityData>>& datas)
{
	for (const auto& entityData : datas) {
		if (entityData) {

			if (entityData->entityName == "BarrierBox") {
				CreateNewBox(BoxType::Barrier, entityData->srt);
			}
			else if (entityData->entityName == "DestructibleBox") {
				CreateNewBox(BoxType::Barrier, entityData->srt);
			}
		}
	}
}


/// <summary>
/// 新しいボックスを作る
/// </summary>
void BoxManager::CreateNewBox(BoxType type, const SRTData& setSRT)
{
	type, setSRT;

	// 壊れないボックス
	if (type == BoxType::Barrier) {

		std::shared_ptr<BarrierBox> newBox = std::make_shared<BarrierBox>();

		// 初期化と設定
		newBox->SetManager(this);
		newBox->SetInitSRT(setSRT);
		newBox->Init();

		boxs_.push_back(newBox);
	}
	// 壊れるボックス
	else if (type == BoxType::Destructible) {

		std::shared_ptr<DestructibleBox> newBox = std::make_shared<DestructibleBox>();

		// 初期化と設定
		newBox->SetManager(this);
		newBox->SetInitSRT(setSRT);
		newBox->Init();

		boxs_.push_back(newBox);
	}
}


/// <summary>
/// ImGuiの描画
/// </summary>
void BoxManager::DrawImGui()
{
	if (ImGui::TreeNode("BoxManager")) {

		ImGui::Text("InstanceNum");
		int num = int(boxs_.size());
		ImGui::DragInt("Instance", &num, 0.0f);
		ImGui::Text("");


		ImGui::TreePop();
	}
}
