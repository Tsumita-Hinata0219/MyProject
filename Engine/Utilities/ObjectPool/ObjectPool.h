#pragma once

#include "Math/MyMath.h"


/* ObjectPoolクラス */
template <typename T>
class ObjectPool {

public:

	/// <summary>
	/// プール作成
	/// </summary>
	void Create(size_t initSize = 10, size_t initExpansionSize = 5) {

		// 初期サイズ分インスタンスを作る
		Init(initSize);
		// 拡張時のサイズを設定
		expansionSize_ = initExpansionSize;
	}

	/// <summary>
	/// オブジェクトを取得
	/// </summary>
	T* Get() {
		for (size_t i = 0; i < pools_.size(); ++i) {
			if (!inUse_[i]) {
				inUse_[i] = true;
				return pools_[i].get();
			}
		}
		ExpandPool(expansionSize_);
		return Get(); // インスタンスが足りなければ拡張してreturn
	}

	/// <summary>
	/// オブジェクトをプールに返却
	/// </summary>
	void Return(T* obj) {
		for (size_t i = 0; i < pools_.size(); ++i) {
			if (pools_[i].get() == obj) {
				inUse_[i] = false;
				return;
			}
		}
	}


private:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(size_t size) {
		for (size_t i = 0; i < size; ++i) {
			pools_.push_back(std::make_unique<T>());
			inUse_.push_back(false);
		}
	}

	/// <summary>
	/// プールの拡張処理
	/// </summary>
	void ExpandPool(size_t size) {
		for (size_t i = 0; i < size; ++i) {
			pools_.push_back(std::make_unique<T>());
			inUse_.push_back(false);
		}
	}


private:

	// オブジェクト配列
	std::vector<std::unique_ptr<T>> pools_;
	// オブジェクトの使用状況
	std::vector<bool> inUse_;
	// 拡張時の生成数
	size_t expansionSize_ = 0;

};