#include "PlayerMovementParticle.h"



/// <summary>
/// 初期化処理
/// </summary>
void PlayerMovementParticle::Init()
{
	isDead_ = false;

	// トランスフォームの初期化
	trans_.Init();

	// 補間に使用するScale値の設定
	scale_.first = 0.8f;
	scale_.second = -0.01f;

	// 補間に使用するAlpha値の設定。不透明->透明
	alpha_.first = 1.0f;
	alpha_.second = 0.0f;

	// 速度
	velocity_ = 0.07f;

	// Timerの初期化&スタート。(1秒)
	timer_.Init(0.0f, 1.0f * 60.0f);
	timer_.Start();
}


/// <summary>
/// 更新処理
/// </summary>
void PlayerMovementParticle::Update()
{
	// タイマーの更新
	timer_.Update();
	// タイマー終了で死亡フラグを立てる
	if (timer_.IsFinish()) {
		isDead_ = true;
	}

	// 軽く上に移動する
	trans_.srt.translate.y += velocity_;

	// Scale値の計算
	CalcScale();

	// Alpha値の計算
	CalcColor();
}


/// <summary>
/// 描画処理
/// </summary>
void PlayerMovementParticle::Draw3D()
{
	model_->SetMaterialColor(color_);
	model_->Draw(trans_);
}


/// <summary>
/// Scale値の計算
/// </summary>
void PlayerMovementParticle::CalcScale()
{
	trans_.srt.scale.x =
		Interpolate(scale_.first, scale_.second, timer_.GetRatio(), Ease::OutSine);
	trans_.srt.scale.y =
		Interpolate(scale_.first, scale_.second, timer_.GetRatio(), Ease::OutSine);
	trans_.srt.scale.z =
		Interpolate(scale_.first, scale_.second, timer_.GetRatio(), Ease::OutSine);
}


/// <summary>
/// Color値の計算
/// </summary>
void PlayerMovementParticle::CalcColor()
{
	color_.w =
		Interpolate(alpha_.first, alpha_.second, timer_.GetRatio(), Ease::InSine);
}

