#pragma once

#include "Math/MyMath.h"
#include <map>
using namespace std;

template <typename tValue>
struct KeyFrame {
	tValue value; // !< キーフレームの値
	float time;   // !< キーフレームの時刻(単位は秒)
};
using KeyFrameVector3 = KeyFrame<Vector3>;
using KeyFrameQuaternion = KeyFrame<Quaternion>;

template <typename tValue>
struct AnimationCurve {
	vector<KeyFrame<tValue>> keyFrames;
};

struct NodeAnimation {
	AnimationCurve<Vector3> scale;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> translate;
};

struct Animation {
	float duration; // アニメーション全体の尺(単位は秒)
	// NodeAnimationの集合。Node名で開けるようにしておく
	map<string, NodeAnimation> nodeAnimations;
};

/* AnimationDataArrasyクラス */
class AnimationDataArray {

public: // メンバ関数

	// コンストラクタ、デストラクタ
	AnimationDataArray(Animation animation);
	~AnimationDataArray() {};

#pragma region Get

	/// <summary>
	/// アニメーションの取得
	/// </summary>
	Animation GetAnimation() { return this->animation_; }

#pragma endregion 

private: // メンバ変数

	Animation animation_{};
};

