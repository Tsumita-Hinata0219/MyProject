#include "GameCameraControl.h"
#include "../GameCamera.h"
#include "GameObject/Character/Player/Player.h"


/// <summary>
/// 初期化処理
/// </summary>
void GameCameraControl::Init(GameCamera* pCamera, Player* pPlayer)
{
	// 各ポインターの設定
	pGameCamera_ = pCamera;
	pPlayer_ = pPlayer;
	pCameraData_ = CameraManager::GetInstance()->GetCameraDataWeak();

	// Inputクラス
	input_ = Input::GetInstance();

	// 俯瞰とFollowCameraで姿勢と相対位置を変える
	if (pGameCamera_->GetCameraType() == GameCameraType::ORBITAL) {

		initRotate_ = { 0.3f, 0.0f, 0.0f };
		constOffset_ = { 0.0f, 15.0f, -45.0f };
	}
	else if (pGameCamera_->GetCameraType() == GameCameraType::TOPDOWN) {

		initRotate_ = { 0.7f, 0.0f, 0.0f };
		constOffset_ = { 0.0f, 60.0f, -60.0f };
	}

	// 姿勢
	pCameraData_.lock()->srt.rotate = initRotate_;
	// オフセットの設定
	playerOffset_ = constOffset_;
}


/// <summary>
/// 更新処理
/// </summary>
void GameCameraControl::Update()
{
	// カメラのタイプがFollowCameraなら回転の処理を通す
	if (pGameCamera_->GetCameraType() == GameCameraType::ORBITAL) {

		// stickの入力を受け取る
		iRStick_ = input_->GetRStick();
		// 回転処理
		FuncOrientation();
	}

	// フォロー処理
	FuncFollow();
}


/// <summary>
/// 回転処理
/// </summary>
void GameCameraControl::FuncOrientation()
{
	if (std::abs(iRStick_.x) > DZone_ || std::abs(iRStick_.y) > DZone_) {

		// 目標回転角度
		float addAngle = iRStick_.x * addOrientationSpeed_;

		// 現在の角度と目標角度の計算
		float targetAngle = pCameraData_.lock()->srt.rotate.y + addAngle;

		// 現在の角度を目標角度の間を補間
		pCameraData_.lock()->srt.rotate.y =
			Lerp(pCameraData_.lock()->srt.rotate.y, targetAngle, orientationLerpSpeed_);
	}
}


/// <summary>
/// フォロー処理
/// </summary>
void GameCameraControl::FuncFollow()
{
	// オフセットの設定
	playerOffset_ = constOffset_;

	// カメラの前方方向に基づいてカメラのオフセットを回転させる
	Matrix4x4 rotateMat = Matrix4x4::identity;
	rotateMat = MakeRotateYMatrix(pCameraData_.lock()->srt.rotate.y);

	// カメラのオフセットの回転
	playerOffset_ = TransformWithPerspective(playerOffset_, rotateMat);

	// プレイヤーの位置にオフセットを加えてカメラの位置を更新
	pCameraData_.lock()->srt.translate = pPlayer_->GetWorldPos() + playerOffset_;

	// Yの位置だけ固定
	pCameraData_.lock()->srt.translate.y = constOffset_.y;
}
