#include "TimedCall.h"



// コンストラクタ
TimedCall::TimedCall(std::function<void()> func, uint32_t time)
{
	// 引数として受け取った情報をメンバ変数にしまう
	this->func_ = func;
	this->timer_ = time;

	// フラグは折っておく
	this->isFinished_ = false;
}


// 更新処理
void TimedCall::Update()
{
	// 完了なら早期リターン
	if (isFinished_) {
		return;
	}

	// タイマーを減らす
	timer_--;

	// タイマーが０以下で処理に入る
	if (timer_ <= 0.0f) {

		// 完了フラグを立てる
		isFinished_ = true;

		// コールバック関数の呼び出し
		func_();
	}
}


/// 完了ならtrueを返す
bool TimedCall::IsFinished() { return this->isFinished_; }