#pragma once

#include "Math/MyMath.h"
#include <functional>

/* 時限発動クラス */
class TimedCall {

public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TimedCall(std::function<void()> func, uint32_t time);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TimedCall() {};

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 完了ならtrueを返す
	/// </summary>
	bool IsFinished();


private:

	// コールバック
	std::function<void()> func_;

	// 残り時間
	uint32_t timer_;

	// 完了フラグ
	bool isFinished_;
};

