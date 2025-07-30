#pragma once

#include "Math/MyMath.h"
#include "../AnimationDataArray/AnimationDataArray.h"
#include "../KeyFrameAnimation/KeyFrameAnimation.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


/* AnimationManagerクラス */
class AnimationManager {

private: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	AnimationManager() = default;
	~AnimationManager() = default;
	AnimationManager(const AnimationManager&) = default;
	const AnimationManager& operator=(const AnimationManager&) = delete;

public: // メンバ関数

	// いインスタンスの取得
	static AnimationManager* GetInstance() {
		static AnimationManager instance;
		return &instance;
	}

	// 初期化処理、更新処理
	static void Initialize();
	static void Update();

	// Animationを読み込む
	static Animation LoadAnimationFile(const std::string& routeFilePath, const std::string& fileName);


private: // メンバ関数

	// 同じファイルは読み込まない
	bool CheckAnimationFile(string fileName);

private: // メンバ変数

	map<string, unique_ptr<AnimationDataArray>> animationDataMap_{};
};

