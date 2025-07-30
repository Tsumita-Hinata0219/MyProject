#include "KeyFrameAnimation.h"



// 初期化処理
void KeyFrameAnimation::Initialize() {}


// 更新処理
void KeyFrameAnimation::Update() {}


// Animationを再生する
Matrix4x4 KeyFrameAnimation::PlayAnimation(string name, Animation animation, float time)
{
	// 時刻を進める
	//time += 1.0f / 60.0f; // 多分この処理いらない

	// 最後まで行ったら最初からリピート再生。リピートしなくてもいい
	time = fmod(time, animation.duration);

	// rootNodeのAnimation取得
	NodeAnimation& rootNodeAnimation = animation.nodeAnimations[name];

	// 指定時刻の値を取得
	Vector3 translate = this->CalculateValueVector3(rootNodeAnimation.translate.keyFrames, time);
	Quaternion rotate = this->CalculateValueQuaternion(rootNodeAnimation.rotate.keyFrames, time);
	Vector3 scale = this->CalculateValueVector3(rootNodeAnimation.scale.keyFrames, time);
	Matrix4x4 resultMat = MakeAffineMatrix(scale, rotate, translate);

	return resultMat;
}


// Animationを適用する
void KeyFrameAnimation::ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime)
{
	for (Joint& joint : skeleton.joints) {

		// 対象のJointのAnimationがあれば、値の適用を行う。下記のif文はC++17から可能になった初期化付きif文
		if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {

			const NodeAnimation& rootNodeAniamtion = (*it).second;
			joint.transform.translate = this->CalculateValueVector3(rootNodeAniamtion.translate.keyFrames, animationTime);
			joint.transform.rotate = this->CalculateValueQuaternion(rootNodeAniamtion.rotate.keyFrames, animationTime);
			joint.transform.scale = this->CalculateValueVector3(rootNodeAniamtion.scale.keyFrames, animationTime);
		}
	}
}


// 任意の時刻の値を取得する
Vector3 KeyFrameAnimation::CalculateValueVector3(const vector<KeyFrameVector3>& keyFrame, float time)
{
	// キーがないものは返す値がわからないのでassert
	assert(!keyFrame.empty());

	// キーが1つか、時刻がキーフレーム前なら最初の値とする
	if (keyFrame.size() == 1 || time <= keyFrame[0].time) {
		return keyFrame[0].value;
	}

	/* ========================================================================
	      先頭から順番に時刻を調べ、指定した時刻が範囲内でであれば、線形補間を行い値を返す
	   ======================================================================== */
	for (size_t index = 0; index < keyFrame.size() - 1; ++index) {

		size_t nextIndex = index + 1;

		// indexとnextIndexの2つのkeyFrameを取得して範囲内に時刻があるかを判定
		if (keyFrame[index].time <= time && time <= keyFrame[nextIndex].time) {
			
			// 範囲内を補間する
			float t = (time - keyFrame[index].time) / (keyFrame[nextIndex].time - keyFrame[index].time);
			return Lerp(keyFrame[index].value, keyFrame[nextIndex].value, t);
		}
	}

	// ここまできた場合は一番後の時刻よりも後ろなので最後の値を返すことにする
	return(*keyFrame.rbegin()).value;
}

Quaternion KeyFrameAnimation::CalculateValueQuaternion(const vector<KeyFrameQuaternion>& keyFrame, float time)
{
	// キーがないものは返す値がわからないのでassert
	assert(!keyFrame.empty());

	// キーが1つか、時刻がキーフレーム前なら最初の値とする
	if (keyFrame.size() == 1 || time <= keyFrame[0].time) {
		return keyFrame[0].value;
	}

	/* ========================================================================
		  先頭から順番に時刻を調べ、指定した時刻が範囲内でであれば、線形補間を行い値を返す
	   ======================================================================== */
	for (size_t index = 0; index < keyFrame.size() - 1; ++index) {

		size_t nextIndex = index + 1;

		// indexとnextIndexの2つのkeyFrameを取得して範囲内に時刻があるかを判定
		if (keyFrame[index].time <= time && time <= keyFrame[nextIndex].time) {

			// 範囲内を補間する
			float t = (time - keyFrame[index].time) / (keyFrame[nextIndex].time - keyFrame[index].time);
			return Slerp(keyFrame[index].value, keyFrame[nextIndex].value, t);
		}
	}

	// ここまできた場合は一番後の時刻よりも後ろなので最後の値を返すことにする
	return(*keyFrame.rbegin()).value;
}
