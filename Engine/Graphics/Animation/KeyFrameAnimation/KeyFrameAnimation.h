#pragma once

#include "../AnimationDataArray/AnimationDataArray.h"
#include "GameObject/GameObject.h"

//#include "Skeleton.h"


/* KeyFrameAnimation */
class KeyFrameAnimation {

private: // シングルトンパターン

	// コンストラクタ、デストラクタ
	KeyFrameAnimation() = default;
	~KeyFrameAnimation() = default;
	KeyFrameAnimation(const KeyFrameAnimation&) = delete;
	const KeyFrameAnimation& operator=(const KeyFrameAnimation&) = delete;

public: // メンバ関数

	// インスタンスの取得
	static KeyFrameAnimation* GetInstance() {
		static KeyFrameAnimation instance;
		return &instance;
	}

	// 初期化処理、更新処理
	void Initialize();
	void Update();

	// Animationを再生する
	Matrix4x4 PlayAnimation(string name, Animation animation, float time);

	// Animationを適用する
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);


private: // メンバ関数

	// 任意の時刻の値を取得する
	Vector3 CalculateValueVector3(const vector<KeyFrameVector3>& keyFrame, float time);
	Quaternion CalculateValueQuaternion(const vector<KeyFrameQuaternion>& keyFrame, float time);


private: // メンバ変数

};

