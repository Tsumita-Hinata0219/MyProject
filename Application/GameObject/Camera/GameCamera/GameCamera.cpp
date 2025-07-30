#include "GameCamera.h"
#include "../../Character/Player/Player.h"


/// <summary>
/// 初期化
/// </summary>
void GameCamera::Init()
{
	// Camera カメラ 
	cameraManager_ = CameraManager::GetInstance();
	cameraManager_->ReSet();
	camera_ = cameraManager_->GetCameraDataWeak();
	camera_.lock()->Init({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -30.0f });

	// 操作処理クラス
	control_ = std::make_unique<GameCameraControl>();
	control_->Init(this, player_);

	// シェイクの範囲
	shakeScope_.X = { -1.0f, 1.0f };
	shakeScope_.Y = { -1.0f, 1.0f };
}


/// <summary>
/// 更新処理
/// </summary>
void GameCamera::Update()
{
	// カメラデータの更新
	camera_.lock()->Update();

	// 操作処理
	control_->Update();

	// 前方ベクトルと右方ベクトルを求める
	CalcForwardVec();
	CalcRightVec();

	// シェイク中
	if (isShake_) {
		ExecuteShake();
	}

#ifdef _DEBUG
	// ImGuiの描画
	DrawImGui();
#endif // _DEBUG
}


/// <summary>
/// 描画処理
/// </summary>
void GameCamera::Draw3D()
{
}
void GameCamera::Draw2DFront()
{
}
void GameCamera::Draw2DBack()
{
}

/// <summary>
/// 衝突判定コールバック関数
/// </summary>
void GameCamera::onCollision([[maybe_unused]] IObject * object)
{
}


/// <summary>
/// カメラのシェイク開始
/// </summary>
void GameCamera::ActivateShake(float intensity, float duration)
{
	// フラグを立て
	isShake_ = true;
	// シェイクの強さを設定
	shakeIntensity_ = intensity;
	// シェイクの時間を設定
	shakeTimer_.Init(0.0f, duration);
	shakeTimer_.Start();
}


/// <summary>
/// 前方ベクトルを求める
/// </summary>
void GameCamera::CalcForwardVec()
{
	// デフォルトの前方ベクトル
	Vector3 deffForwardVec = Vector3::oneZ;

	// Y軸の回転行列
	Matrix4x4 rotateYMat =
		MakeRotateYMatrix(camera_.lock()->srt.rotate.y);

	// 前方ベクトルを求める
	forwardVec_ =
		TransformWithPerspective(deffForwardVec, rotateYMat);
}


/// <summary>
/// 右方ベクトルを求める
/// </summary>
void GameCamera::CalcRightVec()
{
	// デフォルトの前方ベクトル
	Vector3 deffRightVec = Vector3::oneX;

	// Y軸の回転行列
	Matrix4x4 rotateYMat =
		MakeRotateYMatrix(camera_.lock()->srt.rotate.y);

	// 前方ベクトルを求める
	rightVec_ =
		TransformWithPerspective(deffRightVec, rotateYMat);
}


/// <summary>
/// シェイク処理
/// </summary>
void GameCamera::ExecuteShake()
{
	// タイマー更新
	shakeTimer_.Update();

	// 現在のシェイク強度を取得（減衰計算）
	float intensity = CalculateShakeIntensity();

	// ランダムなオフセットを生成
	Vector2 shakeOff = RandomGenerator::getRandom(shakeScope_);
	shakeOff *= intensity;

	// カメラ座標の加算
	camera_.lock()->srt.translate.x += shakeOff.x;
	camera_.lock()->srt.translate.y += shakeOff.y;

	// タイマー終了
	if (shakeTimer_.IsFinish()) {
		// フラグを下げる
		isShake_ = false;
		// シェイクの時間をリセット
		shakeTimer_.Reset();
		// 強度をリセット（必要に応じて）
		shakeIntensity_ = 0.0f;
	}
}


/// <summary>
/// シェイクの強度を計算する（減衰）
/// </summary>
float GameCamera::CalculateShakeIntensity()
{
	// 経過時間と総時間を取得
	float elapsedTime = shakeTimer_.GetNowFrame();
	float totalTime = shakeTimer_.GetEndFrame();

	// 減衰係数（外部設定可能にすることも推奨）
	const float decayRate = 3.0f; // 減衰の速さを調整

	// 減衰計算（指数関数的）
	float decayFactor = exp(-decayRate * elapsedTime / totalTime);

	// 現在の強度を返す
	return shakeIntensity_ * decayFactor;
}


/// <summary>
/// ImGuiの描画
/// </summary>
void GameCamera::DrawImGui()
{
	if (ImGui::TreeNode("GameCamera")) {

		camera_.lock()->DrawImGui();

		if (ImGui::Button("Shake")) {
			ActivateShake();
		}

		ImGui::TreePop();
	}
}
