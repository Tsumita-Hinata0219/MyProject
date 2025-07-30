#include "BulletSweepEraser.h"


/// <summary>
/// 初期化処理
/// </summary>
void BulletSweepEraser::Init()
{
	// Transformの初期化
	transform_.Init();

	// 半径。firstが基本、secondが肥大化した半径
	radius_ = std::make_pair(2.0f, 10.0f);

	// Colliderの初期化
	sphere_ = std::make_unique<SphereCollider>(this);
	sphere_->data_.center = transform_.GetWorldPos();
	sphere_->data_.radius = radius_.first; // 基本波形で初期化
	sphere_->SetActive(false);

	// スイープ時間の初期化
	sweepTimer_.Init(0.0f, 1.5f * 60.0f);

	// スイープ中フラグをfalseにする
	isSweeping_ = false;
}


/// <summary>
///  更新処理
/// </summary>
void BulletSweepEraser::Update()
{
	// Transformの更新処理
	transform_.UpdateMatrix();

	// Colliderの座標の設定
	sphere_->data_.center = transform_.GetWorldPos();

	// スイープ中なら
	if (isSweeping_)
	{
		// タイマー更新
		sweepTimer_.Update(true);

		// スイープ終了
		if (sweepTimer_.IsFinish()) {

			// スイープ中フラグをfalseにする
			isSweeping_ = false;
			// コライダーを非アクティブにする
			sphere_->SetActive(false);
			// 半径を元に戻す
			sphere_->data_.radius = radius_.first;
		}

		// 半径の計算
		CalcRadius();
	}
}


/// <summary>
/// スイープ開始
/// </summary>
void BulletSweepEraser::StartSweep()
{
	// 既にスイープ中なら終了
	if (isSweeping_) { return; }

	// スイープ中に設定
	isSweeping_ = true;

	// タイマースタート
	sweepTimer_.Start();

	// colliderをアクティブにする
	sphere_->SetActive(true);
}


/// <summary>
/// 半径の計算
/// </summary>
void BulletSweepEraser::CalcRadius()
{
	sphere_->data_.radius = 
		Interpolate(radius_.first, radius_.second, sweepTimer_.GetRatio(), Ease::OutQuint);
}

