#include "StageSelect.h"
#include "System/GameData/GameData.h"


void StageSelect::Init()
{
	/* ----- Input インプット ----- */
	input_ = Input::GetInstance();

	// ステージのセレクト番号
	int stageNum = GameData::GetInstance()->Get_StageSelectNum();
	selectNum_ = stageNum;

	// 選択したかのフラグ
	isSelect_ = false;
}

void StageSelect::Update()
{
	// 決定していれば後の処理は入らない
	if (isSelect_) { return; }

	// LStickの入力を取得
	iLStick_ = input_->GetLStick();

	// フラグが立っているとき
	if (isOperationFunc_) {

		// stickを戻したらフラグを折る
		if (iLStick_.x >= -DZone_ && iLStick_.x <= DZone_) {
			isOperationFunc_ = false;
		}
	}

	// セレクト操作
	OperationSelect();

	// Pad::A or Space ボタンを押したら決定
	if (input_->Trigger(PadData::A) ||
		input_->Trigger(DIK_SPACE)) {
		isSelect_ = true;
	}

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}

void StageSelect::Draw3D()
{
}

void StageSelect::Draw2DFront()
{
}

void StageSelect::Draw2DBack()
{
}

void StageSelect::onCollision([[maybe_unused]] IObject* object)
{
}

void StageSelect::OperationSelect()
{

	// LStickの入力を取得
	iLStick_ = input_->GetLStick();


	// 左入力
	if (iLStick_.x < -DZone_ || input_->Trigger(PadData::LEFT) ||
		input_->Trigger(DIK_LEFT) || input_->Trigger(DIK_A)) {
		if (selectNum_ <= 1) {
			return;
		}
		// デクリメント
		selectNum_--;
	}
	// 右入力
	if (iLStick_.x > DZone_ || input_->Trigger(PadData::RIGHT) ||
		input_->Trigger(DIK_RIGHT) || input_->Trigger(DIK_D)) {
		if (selectNum_ >= 5) {
			return;
		}
		// インクリメント
		selectNum_++;
	}
}


// ImGuiの描画
void StageSelect::DrawImGui()
{
	if (ImGui::TreeNode("StageSelect")) {
		ImGui::Text("NowSelect Num = %d", selectNum_);

		ImGui::TreePop();
	}
}
