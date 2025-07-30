#pragma once


#include "Math/MyMath.h"

// ステート
enum class MenuDirectionState {
	Idle,       // 待機中
	Processing, // 処理中
	Finished    // 処理終了
};

/* ゲーム終了時に表示するメッセージの基底クラス */
class IStageTransitionMenu {

public:

	/// <summary>
	/// 仮想デストラク
	/// </summary>
	virtual ~IStageTransitionMenu() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw2DFront() = 0;

	/// <summary>
	/// 演出開始
	/// </summary>
	virtual void DirectionStart() = 0;

	/// <summary>
	/// 演出更新
	/// </summary>
	virtual void DirectionUpdate() = 0;

	/// <summary>
	/// 演出終了
	/// </summary>
	virtual void DirectionExsit() = 0;

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	virtual void DrawImGui() = 0;

	/// <summary>
	/// ステートの取得
	/// </summary>
	virtual MenuDirectionState GetState() { return this->state_; }

	/// <summary>
	/// テクスチャハンドルの設定
	/// </summary>
	virtual void SetTextureHandle(const uint32_t& setTexHandle) { sprite_->SetTexture(setTexHandle); }

	/// <summary>
	/// 座標の設定
	/// </summary>
	virtual void SetPosition(const Vector3& setPos) { this->trans_.srt.translate = setPos; }

	/// <summary>
	/// カラーの設定
	/// </summary>
	virtual void SetColor(const Vector4& setColor) { this->color_ = setColor; }


protected:

	// ステート
	MenuDirectionState state_ = MenuDirectionState::Idle;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// スプライト
	std::unique_ptr<Sprite> sprite_;

	// トランスフォーム
	Transform trans_{};

	// カラー
	Vector4 color_{};

	// タイマー
	Timer timer_{};
};