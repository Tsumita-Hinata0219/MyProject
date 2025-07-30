#pragma once

#include "GameObject/GameObject.h"
#include "System/IObject/IObject.h"



/* 弾をスイープ(範囲チェック)しながら消す処理 */
class BulletSweepEraser : public IObject {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BulletSweepEraser() { attribute_ = { Attributes::Category::PLAYER, Attributes::Type::SWEERER }; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BulletSweepEraser() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	///  更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw3D() override {};
	void Draw2DFront() override {};
	void Draw2DBack() override {};

	/// <summary>
	/// 衝突判定コールバック関数
	/// </summary>
	void onCollision([[maybe_unused]] IObject* object) override {};

	/// <summary>
	/// スイープ開始
	/// </summary>
	void StartSweep();


#pragma region Accessor アクセッサ

	// ペアレントを結ぶ
	void SetParent(Transform* parent) { this->transform_.SetParent(parent); }

#pragma endregion 


private:

	/// <summary>
	/// 半径の計算
	/// </summary>
	void CalcRadius();


private:
	// コライダー
	std::unique_ptr<SphereCollider> sphere_;

	// 半径
	std::pair<float, float> radius_;

	// スイープ中かどうか
	bool isSweeping_ = false;

	// sweepの時間
	Timer sweepTimer_{};

};
