#pragma once

#include "GameObject/GameObject.h"


// 前方宣言
class PauseManager;


/* ポーズ時の操作クラス */
class PauseController
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PauseController() = default;
	PauseController(PauseManager* pauseManager) : m_pauseManager_(pauseManager) {}

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PauseController() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();


private:



private:

	// ポーズマネージャー
	PauseManager* m_pauseManager_ = nullptr;

	// 入力
	Input* input_ = nullptr;
	// デッドゾーン
	const float DZone_ = 0.4f;
};
