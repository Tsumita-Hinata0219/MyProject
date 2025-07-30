#pragma once

#include "Math/MyMath.h"

/* Timerクラス */
class Timer final {

public: // メンバ関数

	// タイマーの設定
	void Init(float setStart, float setEnd);

	// スタート時に呼び出す処理
	void Start();

	// タイマーの更新処理
	void Update(bool roop = false);

	// リセット
	void Reset();

	// クリア
	void Clear();


#pragma region Get 取得

	float GetNowFrame() const { return this->nowFrame_; }
	float GetEndFrame() const { return this->endFrame_; }
	float GetRatio() const { return this->frameRatio_; }
	bool IsActive() const { return this->isActive_; }
	bool IsFinish() const { return this->isFinish_; }

#pragma endregion 

#pragma region Set 設定

	void SetNowFrame(float setFrame) { this->nowFrame_ = setFrame; }
	void SetEndFrame(float setFrame) { this->endFrame_ = setFrame; }
	void SetRatio(float setFrame) { this->frameRatio_ = setFrame; }
	/*void SetIsActive(bool setFlag) { this->isActive_ = setFlag; }
	void SetIsFinish(bool setFlag) { this->isFinish_ = setFlag; }*/

#pragma endregion 


private:

	// リスタート
	void ReStart();


private: // メンバ変数

	float nowFrame_ = 1.0f;
	float endFrame_ = 1.0f;
	float frameRatio_ = 0.0f;
	bool isActive_ = false;
	bool isFinish_ = true;
	std::pair<float, float> saveTimer_;
};

