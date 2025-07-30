#include "StageTransitionMenuManager.h"
#include "System/GameData/GameData.h"


/// <summary>
/// 初期化処理
/// </summary>
void StageTransitionMenuManager::Init()
{
	input_ = Input::GetInstance();

	/* ----- Menu メニュー ----- */
	menus_.resize(EnumSize<STMenuType>::value);
	menus_[int(STMenuType::Blur)] = std::make_unique<StageTransitionMenuBlur>();
	menus_[int(STMenuType::BackScreen)] = std::make_unique<StageTransitionMenuBackScreen>();
	menus_[int(STMenuType::Navigation_Back)] = std::make_unique<StageTransitionMenuNavigation>();
	menus_[int(STMenuType::Navigation_Next)] = std::make_unique<StageTransitionMenuNavigation>();
	menus_[int(STMenuType::ResultUI)] = std::make_unique<StageTransitionMenuResultUI>();
	menus_[int(STMenuType::TextLine)] = std::make_unique<StageTransitionMenuTextLine>();
	// 各演出の初期化
	for (auto& element : menus_) {
		element->Init();
	}
	// Navigationだけテクスチャと座標を変える
	uint32_t backTexHD = TextureManager::LoadTexture("Texture/Game/StageTransitionMenu", "StageTransitionMenu_Back.png");
	uint32_t nextTexHD = TextureManager::LoadTexture("Texture/Game/StageTransitionMenu", "StageTransitionMenu_Next.png");
	menus_[int(STMenuType::Navigation_Back)]->SetTextureHandle(backTexHD);
	menus_[int(STMenuType::Navigation_Next)]->SetTextureHandle(nextTexHD);
	Vector3 backInitPos = { 520.0f, 450.0f, 0.0f };
	Vector3 nextInitPos = { 760.0f, 450.0f, 0.0f };
	menus_[int(STMenuType::Navigation_Back)]->SetPosition(backInitPos);
	menus_[int(STMenuType::Navigation_Next)]->SetPosition(nextInitPos);

	/* ----- NavigatiuonTextBack ナビゲーションバック ----- */
	naviBack_ = std::make_unique<StageTransitionMenuNaviBack>();
	naviBack_->Init();
	naviBack_->SetTargetPos(backInitPos, nextInitPos);

	// フラグは折っておく
	isActive_ = false;
	isFinish_ = false;

	// 初期ステートは待機
	state_ = MenuDirectionState::Idle;
}


/// <summary>
/// 更新処理
/// </summary>
void StageTransitionMenuManager::Update()
{
#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG

	// ステートが処理中以外なら早期return
	if (state_ != MenuDirectionState::Processing) { return; }

	// 各演出の更新
	for (auto& element : menus_) {
		element->Update();
	}

	// バックスクリーンの演出が終わったらその他の演出を始める
	if (menus_[int(STMenuType::BackScreen)]->GetState() == MenuDirectionState::Finished) {
		menus_[int(STMenuType::ResultUI)]->DirectionStart();
		menus_[int(STMenuType::TextLine)]->DirectionStart();
	}
	// リザルトUIの演出が終わったらNaviGationの演出を始める
	if (menus_[int(STMenuType::ResultUI)]->GetState() == MenuDirectionState::Finished) {
		menus_[int(STMenuType::Navigation_Back)]->DirectionStart();
		menus_[int(STMenuType::Navigation_Next)]->DirectionStart();
	}

	// ナビゲーションの演出が終わったらセレクトバーの更新を入れる
	if (menus_[int(STMenuType::Navigation_Back)]->GetState() == MenuDirectionState::Finished) {
		isFinish_ = true;
		naviBack_->Update();

		if (naviBack_->GetNowSelect() == MenuSelect::Back) {
			menus_[int(STMenuType::Navigation_Back)]->SetColor(Temp::Color::BLACK);
			menus_[int(STMenuType::Navigation_Next)]->SetColor(Temp::Color::WHITE);
		}
		else if (naviBack_->GetNowSelect() == MenuSelect::Next) {
			menus_[int(STMenuType::Navigation_Back)]->SetColor(Temp::Color::WHITE);
			menus_[int(STMenuType::Navigation_Next)]->SetColor(Temp::Color::BLACK);
		}
	}
}


/// <summary>
/// 描画処理
/// </summary>
void StageTransitionMenuManager::Draw2DFront()
{
	if (!isActive_) { return; }

	// 各演出の描画
	menus_[int(STMenuType::Blur)]->Draw2DFront();
	menus_[int(STMenuType::BackScreen)]->Draw2DFront();
	// NavigationTextBackはNavigationの後ろに入れる
	naviBack_->Draw2DFront();
	menus_[int(STMenuType::Navigation_Back)]->Draw2DFront();
	int num = GameData::GetInstance()->Get_StageSelectNum();
	if (num < 5) { 
		menus_[int(STMenuType::Navigation_Next)]->Draw2DFront();
	}
	menus_[int(STMenuType::ResultUI)]->Draw2DFront();
	menus_[int(STMenuType::TextLine)]->Draw2DFront();
}


/// <summary>
/// 演出開始
/// </summary>
void StageTransitionMenuManager::DirectionStart()
{
	if (state_ == MenuDirectionState::Idle) {

		// ステートを処理中へ
		state_ = MenuDirectionState::Processing;

		// フラグを立て
		isActive_ = true;

		// ブラーとバックスクリーンの演出開始
		menus_[int(STMenuType::Blur)]->DirectionStart();
		menus_[int(STMenuType::BackScreen)]->DirectionStart();
	}
}


/// <summary>
/// 終了処理
/// </summary>
void StageTransitionMenuManager::FuncEndDirection()
{
	naviBack_->FuncEndDirection();
}


/// <summary>
/// ImGuiの描画
/// </summary>
void StageTransitionMenuManager::DrawImGui()
{
	if (ImGui::TreeNode("StageTransitionMenu_Manager")) {

		ImGui::Text("Directions_State");
		ShowState("	BackScreen", menus_[int(STMenuType::BackScreen)]->GetState());
		ShowState("	Blur", menus_[int(STMenuType::Blur)]->GetState());
		ShowState("	Navigation_Back", menus_[int(STMenuType::Navigation_Back)]->GetState());
		ShowState("	Navigation_Next", menus_[int(STMenuType::Navigation_Next)]->GetState());
		ShowState("	ResultUI", menus_[int(STMenuType::ResultUI)]->GetState());
		ShowState("	TextLine", menus_[int(STMenuType::TextLine)]->GetState());

		ImGui::Text("");

		ImGui::TreePop();
	}
}


/// <summary>
/// Stateの描画
/// </summary>
void StageTransitionMenuManager::ShowState(const char* label, MenuDirectionState state)
{
	ImGui::Text("%s_State : ", label); ImGui::SameLine();
	switch (state) {
	case MenuDirectionState::Idle:
		ImGui::Text("Idle");
		break;
	case MenuDirectionState::Processing:
		ImGui::Text("Processing");
		break;
	case MenuDirectionState::Finished:
		ImGui::Text("Finished");
		break;
	}
}
