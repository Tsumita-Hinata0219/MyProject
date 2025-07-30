#include "EnemyHitEffectCircle.h"
#include "../../../Enemy/IEnemy.h"
#include "System/Camera/Manager/CameraManager.h"


/// <summary>
/// 初期化処理
/// </summary>
void EnemyHitEffectCircle::Init()
{
	// モデルの設定
	modelManager_->LoadModel("Obj/Enemys/Effect/HitCircle", "HitCircle.obj");
	model_ = modelManager_->GetModel("HitCircle");
	model_->SetRenderState(RenderState::DepthWriteMask_ZERO);

	// トランスフォームの初期化
	trans_.Init();
	// 座標は親エネミーと同じ場所
	trans_.srt.translate = pEnemy_->GetTranslate();

	// 死亡フラグは折る
	isDead_ = false;

	// 初期カラーは白
	color_ = Temp::Color::WHITE;

	// 補間に使用するScaleの値
	scale_.first = 2.0f;
	scale_.second = 4.0f;

	// 補間に使用するAlpha値の設定。不透明->透明
	alpha_.first = 1.0f;
	alpha_.second = 0.0f;


	// タイマーの設定 (0.2秒)
	timer_.Init(0.0f, 0.2f * 60.0f);
	timer_.Start();
}


/// <summary>
/// 更新処理
/// </summary>
void EnemyHitEffectCircle::Update()
{
	// タイマー更新
	timer_.Update();
	// タイマー終了
	if (timer_.IsFinish()) {
		
		// 死亡フラグを立てる
		isDead_ = true;
	}

	// 座標は親エネミーと同じ場所
	trans_.srt.translate = pEnemy_->GetTranslate();

	// Scaleの計算
	CalcScale();

	// Alphaの計算
	CalcAlpha();

	// Rotateの計算
	CalcRotate();
}


/// <summary>
/// 描画処理
/// </summary>
void EnemyHitEffectCircle::Draw3D()
{
	model_->SetMaterialColor(color_);
    	model_->Draw(trans_);
}


/// <summary>
/// Scaleの計算
/// </summary>
void EnemyHitEffectCircle::CalcScale()
{
	trans_.srt.scale.x =
		Interpolate(scale_.first, scale_.second, timer_.GetRatio(), Ease::OutSine);
	trans_.srt.scale.y =
		Interpolate(scale_.first, scale_.second, timer_.GetRatio(), Ease::OutSine);
	trans_.srt.scale.z =
		Interpolate(scale_.first, scale_.second, timer_.GetRatio(), Ease::OutSine);
}


/// <summary>
/// Alphaの計算
/// </summary>
void EnemyHitEffectCircle::CalcAlpha()
{
	color_.w =
		Interpolate(alpha_.first, alpha_.second, timer_.GetRatio(), Ease::InSine);
}


/// <summary>
/// Rotateの計算
/// </summary>
void EnemyHitEffectCircle::CalcRotate()
{
	// カメラ座標の取得
	auto camera = CameraManager::GetInstance()->GetCameraDataWeak();
	Vector3 cameraPos = camera.lock()->srt.translate;

	// 正規化
	Vector3 diffRotate = Normalize(trans_.GetWorldPos() - cameraPos);

	// 姿勢の計算
	trans_.srt.rotate.y = atan2(diffRotate.x, diffRotate.z);
	float vecZ = sqrt((diffRotate.x * diffRotate.x) + (diffRotate.z * diffRotate.z));
	float height = -diffRotate.y;
	trans_.srt.rotate.x = atan2(height, vecZ);
}

