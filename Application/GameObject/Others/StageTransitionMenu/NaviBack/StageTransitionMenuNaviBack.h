#pragma once

#include <list>
#include <memory>

#include "System/IObject/IObject.h"
#include "../../../GameObject.h"
#include "../IStageTransitionMenu.h"
#include "Graphics/PostEffect/IPostEffect/RadialBlur/RadialBlurEffect.h"


enum class MenuSelect {
	Back,
	Next,
	Other,
};


/* ゲーム終了メニューのナビゲーションバック */
class StageTransitionMenuNaviBack {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageTransitionMenuNaviBack() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageTransitionMenuNaviBack() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw2DFront();

	/// <summary>
	/// 終了処理
	/// </summary>
	void FuncEndDirection();

	/// <summary>
	/// 終了処理開始
	/// </summary>
	void EndDirectionStart();


#pragma region Accessor アクセッサ

	// ターゲット座標
	void SetTargetPos(Vector3 setA, Vector3 setB) { targetPos_ = { setA, setB }; }

	// セレクト
	MenuSelect GetNowSelect() const { return this->nowSelect_; }

#pragma endregion 


private:

	/// <summary>
	/// セレクト操作
	/// </summary>
	void SelectOperation();

	/// <summary>
	/// セレクト変更
	/// </summary>
	void ChangeSelect(MenuSelect select);

	/// <summary>
	/// Y軸サイズの補間処理
	/// </summary>
	void InterpolateSizeY();

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui();


private:

	// 入力
	Input* input_ = nullptr;
	//Stickの入力を取得
	Vector2 iLStick_{};
	// デッドゾーン
	const float DZone_ = 0.4f;

	// 配列サイズ
	static const uint32_t arraySize_ = 2;

	// テクスチャハンドル
	std::array<uint32_t, arraySize_> texHDArr_;

	// スプライト
	std::array<std::unique_ptr<Sprite>, arraySize_> spriteArr_;

	// スプライトサイズ
	Vector2 initSize_{};
	Vector2 targetSize_{};

	// トランスフォーム
	std::array<Transform, arraySize_> transArr_;

	// ターゲット座標
	std::array<Vector3, arraySize_> targetPos_;

	// セレクトステート
	MenuSelect nowSelect_ = MenuSelect::Other;

	// タイマー
	Timer timer_{};

	// 終了処理に入るかのフラグ
	bool IsEndDirection_ = false;

};

