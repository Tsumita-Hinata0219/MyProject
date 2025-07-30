#include "SelectBar.h"



// 初期化処理
void SelectBar::Init()
{
	/* ----- Input インプット ----- */
	input_ = Input::GetInstance();

	// Modelの設定
	modelManager_ = ModelManager::GetInstance();
	modelManager_->LoadModel("Obj/SelectBar", "SelectBar.obj");
	model_ = modelManager_->GetModel("SelectBar");

	uint32_t dds = TextureManager::LoadTexture("Obj/SelectBar", "SelectBar.dds");
	model_->SetMaterialTexture(dds);

	// Transformの初期化
	trans_.Init();
	trans_.srt.scale.x = 2.0f;
	trans_.srt.scale.y = 0.5f;
	trans_.srt.translate.z = 18.0f;

	// 選択中
	nowOption_ = SelectOption::Start;
}


// 更新処理
void SelectBar::Update()
{
	// UI操作
	Operation();

	// 座標の変更
	ChangeSelectBarPos();

#ifdef _DEBUG
	if (ImGui::TreeNode("SelectBar")) {

		trans_.DrawImGui();
		ImGui::Text("");
		
		if (nowOption_ == SelectOption::Start) {
			ImGui::Text("Now_Select : Start");
		}
		else if (nowOption_ == SelectOption::Exit) {
			ImGui::Text("Now_Select : Exit");
		}

		ImGui::TreePop();
	}
#endif // _DEBUG
}


// 描画処理
void SelectBar::Draw3D()
{
	model_->Draw(trans_);
}
void SelectBar::Draw2DFront() {}
void SelectBar::Draw2DBack() {}


// 衝突判定コールバック関数
void SelectBar::onCollision([[maybe_unused]] IObject* object)
{
}


// 操作
void SelectBar::Operation()
{
	if (input_->Trigger(PadData::UP)) {
		PreOption();
	}
	if (input_->Trigger(PadData::DOWN)) {
		NextOption();
	}
}


// 次のオプションへ
void SelectBar::NextOption()
{
	nowOption_ =
		static_cast<SelectOption>((static_cast<int>(nowOption_) + 1) % 
			static_cast<int>(SelectOption::Count));
}


// 前のオプションへ
void SelectBar::PreOption()
{
	nowOption_ =
		static_cast<SelectOption>((static_cast<int>(nowOption_) - 1 +
			static_cast<int>(SelectOption::Count)) % static_cast<int>(SelectOption::Count));
}


// 座標の変更
void SelectBar::ChangeSelectBarPos()
{
	trans_.srt.translate = labelPos_[enum_val(nowOption_)];
}
