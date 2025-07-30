#pragma once

#include "../../Interface/IEnemyEffect.h"


/* エネミーのヒットエフェクト */
class EnemyHitEffectCircle : public IEnemyEffect {

public:

	/// <summary>
	/// コンストラク
	/// </summary>
	EnemyHitEffectCircle() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyHitEffectCircle() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw3D() override;


private:

	/// <summary>
	/// Scaleの計算
	/// </summary>
	void CalcScale();

	/// <summary>
	/// Alphaの計算
	/// </summary>
	void CalcAlpha();

	/// <summary>
	/// Rotateの計算
	/// </summary>
	void CalcRotate();


private:

	// モデルにセットするカラー
	Vector4 color_{};

	// 補間に使用する値
	std::pair<float, float> scale_;
	std::pair<float, float> alpha_;

};
