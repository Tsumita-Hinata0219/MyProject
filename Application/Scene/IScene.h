#pragma once

class GameManager;

/* ISceneクラス */
class IScene {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IScene() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IScene() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 背景スプライトの描画処理
	/// </summary>
	virtual void BackSpriteDraw() = 0;

	/// <summary>
	/// ３Dオブジェクトの描画処理
	/// </summary>
	virtual void ModelDraw() = 0;

	/// <summary>
	/// 前景スプライトの描画処理
	/// </summary>
	virtual void FrontSpriteDraw() = 0;

	/// <summary>
	/// 親マネージャーのポインタ
	/// </summary>
	/// <param name="setManager"></param>
	void SetManager(GameManager* setManager) { this->Manager_ = setManager; }


protected:

	// ゲームマネージャー
	GameManager* Manager_ = nullptr;

};