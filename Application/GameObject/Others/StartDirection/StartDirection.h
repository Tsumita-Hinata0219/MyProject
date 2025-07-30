#pragma once

#include <list>
#include <memory>

#include "System/IObject/IObject.h"
#include "../../GameObject.h"

/* StartDirectionクラス */
class StartDirection {

public:

	// コンストラクタ、デストラクタ
	StartDirection() = default;
	~StartDirection() = default;

	// 初期化処理、更新処理、描画処理
	void Init();
	void Update();
	void Draw2DFront();


#pragma region Accessor アクセッサ

	// 終了フラグ
	bool IsFinish() const { return this->isFinish_; }

#pragma endregion


private:

	// サイズの移行処理
	void SizeFunc();

	// 姿勢の移行処理
	void RotateFunc();

	// ImGuiの描画
	void DrawImGui();


private:

	// texture
	std::vector<uint32_t> textures_;
	uint32_t texArrayNum_ = 0;

	// Sprite
	std::unique_ptr<Sprite> sprite_;

	// Transform
	Transform trans_{};

	// Size
	Vector2 texSize_{};
	Vector2 targetSize_{};

	// Rotate
	float startRotateZ_ = 0.0f;
	float targetRotateZ_ = 0.0f;

	// timer
	Timer timer_{};

	// IsFinish
	bool isFinish_ = false;
};

