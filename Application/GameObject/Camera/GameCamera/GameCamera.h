#pragma once

#include <list>
#include <memory>

#include "System/IObject/IObject.h"
#include "../../GameObject.h"

#include "Control/GameCameraControl.h"


// GameCameraのタイプ
enum class GameCameraType {
	ORBITAL,
	TOPDOWN,
};


// 前方宣言
class Player;


/* Gameに使うカメラクラス */
class GameCamera : public IObject {

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameCamera() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameCamera() = default;

	/// <summary>
	/// 初期化
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
	void Draw2DFront() override;
	void Draw2DBack() override;

	/// <summary>
	/// 衝突判定コールバック関数
	/// </summary>
	/// <param name="object"></param>
	void onCollision([[maybe_unused]] IObject* object) override;

	/// <summary>
	/// カメラのシェイク開始
	/// </summary>
	void ActivateShake(float intensity = 0.8f, float duration = 30.0f);


#pragma region Accessor アクセッサ

	// Type
	GameCameraType GetCameraType() const { return this->cametaType_; }
	void SetCameraType(GameCameraType setType) { this->cametaType_ = setType; }

	// Player
	void SetPlayer(Player* player) { this->player_ = player; }

	// ForwardVec
	Vector3& GetForwardVec() { return this->forwardVec_; }

	// RightVec
	Vector3& GetRightVec() { return this->rightVec_; }

#pragma endregion 


private:

	/// <summary>
	/// 前方ベクトルを求める
	/// </summary>
	void CalcForwardVec();

	/// <summary>
	/// 右方ベクトルを求める
	/// </summary>
	void CalcRightVec();

	/// <summary>
	/// シェイク処理
	/// </summary>
	void ExecuteShake();

	/// <summary>
	/// シェイクの強度を計算する（減衰）
	/// </summary>
	float CalculateShakeIntensity();

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui();


private:

	// プレイヤー
	Player* player_ = nullptr;

	// カメラ
	CameraManager* cameraManager_ = nullptr;
	std::weak_ptr<CameraData> camera_;

	// カメラのタイプ
	GameCameraType cametaType_ = GameCameraType::ORBITAL;

	// 操作処理クラス
	std::unique_ptr<GameCameraControl> control_;

	// 前方ベクトル
	Vector3 forwardVec_{};
	// 右方ベクトル
	Vector3 rightVec_{};

	// シェイク中
	bool isShake_ = false;
	// シェイクの強さ
	float shakeIntensity_ = 0.0f;
	// シェイクの時間
	Timer shakeTimer_{};
	// シェイク範囲
	ScopeVec2 shakeScope_{};

};

