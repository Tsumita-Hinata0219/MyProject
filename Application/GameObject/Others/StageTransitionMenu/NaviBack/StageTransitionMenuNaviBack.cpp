#include "StageTransitionMenuNaviBack.h"
#include "System/GameData/GameData.h"



/// <summary>
/// 初期化処理
/// </summary>
void StageTransitionMenuNaviBack::Init()
{
	// 入力
	input_ = Input::GetInstance();

	// テクスチャ
	uint32_t backTexHD = TextureManager::LoadTexture(
		"Texture/Game/StageTransitionMenu", "StageTransitionMenu_BackScreen.png");
	uint32_t frameTexHD = TextureManager::LoadTexture(
		"Texture/Game/StageTransitionMenu",	"StageTransitionMenu_Frame.png");
	texHDArr_ = { backTexHD, frameTexHD };

	// 補間に使用するスプライトサイズ
	initSize_ = { 192.0f, 24.0f };
	targetSize_ = { 192.0f, 52.0f };

	// 初期化で使用するサイズ
	Vector2 size_ = { 192.0f, 36.0f };

	for (int i = 0; i < arraySize_; ++i) {

		// スプライト
		spriteArr_[i] = std::make_unique<Sprite>();
		spriteArr_[i]->Init(size_);
		spriteArr_[i]->SetAnchor(SpriteAnchor::Center);
		spriteArr_[i]->SetTexture(texHDArr_[i]);
		spriteArr_[i]->SetColor(Temp::Color::WHITE);

		// トランスフォーム
		transArr_[i].Init();
		transArr_[i].srt.translate = { -100.0f, -100.0f, 0.0f }; // 画面外に出しておく
	}

	// タイマー
	timer_.Init(0.0f, 1.0f * 60.0f);

	// 選択しているものはNoneにしておく
	nowSelect_ = MenuSelect::Other;

	// 終了処理に入るフラグは折っておく
	IsEndDirection_ = false;
}


/// <summary>
/// 更新処理
/// </summary>
void StageTransitionMenuNaviBack::Update()
{
	// セレクト操作
	SelectOperation();

	// Y軸サイズの補間処理
	InterpolateSizeY();

#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG
}


/// <summary>
/// 描画処理
/// </summary>
void StageTransitionMenuNaviBack::Draw2DFront()
{
	spriteArr_[1]->Draw(transArr_[1]);
	spriteArr_[0]->Draw(transArr_[0]);
}


/// <summary>
/// 終了処理
/// </summary>
void StageTransitionMenuNaviBack::FuncEndDirection()
{
	if (!IsEndDirection_) { return; }

	if (timer_.IsActive()) {

		// タイマー更新
		timer_.Update();

		Vector2 setSize = targetSize_;
		setSize.y =
			targetSize_.y + (initSize_.y - targetSize_.y) * Ease::InBack(timer_.GetRatio(), 8.0f);

		// 初期値から目標値へY軸サイズを補完する
		spriteArr_[1]->SetSize(setSize);

		if (timer_.IsFinish()) {
			timer_.Clear();
		}
	}
}


/// <summary>
/// 終了処理開始
/// </summary>
void StageTransitionMenuNaviBack::EndDirectionStart()
{
	// 終了処理に入る
	IsEndDirection_ = true;

	// タイマー設定
	timer_.Init(0.0f, 1.0f * 60.0f);
	timer_.Start();

	// サイズを初期値にする
	spriteArr_[1]->SetSize(targetSize_);
}


/// <summary>
/// セレクト操作
/// </summary>
void StageTransitionMenuNaviBack::SelectOperation()
{
	// LStickの入力を取得
	iLStick_ = input_->GetLStick();

	// 左入力
	if (iLStick_.x < -DZone_ || input_->Trigger(PadData::LEFT) || 
		input_->Trigger(DIK_LEFT) || input_->Trigger(DIK_A)) {
		ChangeSelect(MenuSelect::Back);
	}
	// 右入力
	if (iLStick_.x > DZone_ || input_->Trigger(PadData::RIGHT) || 
		input_->Trigger(DIK_RIGHT) || input_->Trigger(DIK_D)) {
		int num = GameData::GetInstance()->Get_StageSelectNum();
		if (num >= 5) { return; }
		ChangeSelect(MenuSelect::Next);	
	}
}


/// <summary>
/// セレクト変更
/// </summary>
void StageTransitionMenuNaviBack::ChangeSelect(MenuSelect select)
{
	// ステートが異なるなら処理に入る
	if (nowSelect_ != select) {

		// 選択中を更新
		nowSelect_ = select;

		// 座標を更新
		for (auto& element : transArr_) {
			element.srt.translate = targetPos_[int(nowSelect_)];
		}

		// タイマー設定
		timer_.Init(0.0f, 1.0f * 60.0f);
		timer_.Start();
		
		// サイズを初期値にする
		spriteArr_[1]->SetSize(initSize_);
	}
}


/// <summary>
/// Y軸サイズの補間処理
/// </summary>
void StageTransitionMenuNaviBack::InterpolateSizeY()
{
	if (timer_.IsActive()) {

		// タイマー更新
		timer_.Update();

		Vector2 setSize = initSize_;
		setSize.y = Interpolate(initSize_.y, targetSize_.y, timer_.GetRatio(), Ease::OutCubic);

		// 初期値から目標値へY軸サイズを補完する
		spriteArr_[1]->SetSize(setSize);

		if (timer_.IsFinish()) {
			timer_.Clear();
		}
	}
}


/// <summary>
/// ImGuiの描画
/// </summary>
void StageTransitionMenuNaviBack::DrawImGui()
{
	if (ImGui::TreeNode("StageTransitionMenu_NaviBack")) {

		if (nowSelect_ == MenuSelect::Other) {
			ImGui::Text("NowSelect : Other");
		}
		else if (nowSelect_ == MenuSelect::Back) {
			ImGui::Text("NowSelect : Back");
		}
		else if (nowSelect_ == MenuSelect::Next) {
			ImGui::Text("NowSelect : Next");
		}
		ImGui::Text("");

		ImGui::Text("TargetSize");
		ImGui::DragFloat2("size", &targetSize_.x, 0.1f);
		ImGui::Text("");

		ImGui::Text("Input");
		ImGui::DragFloat2("L_Stick", &iLStick_.x, 0.0f);
		ImGui::Text("");

		ImGui::TreePop();
	}
}
