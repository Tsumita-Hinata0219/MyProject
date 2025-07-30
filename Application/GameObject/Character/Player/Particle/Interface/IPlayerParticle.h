#pragma once

#include <list>
#include <memory>

#include "System/IObject/IObject.h"
#include "GameObject/GameObject.h"
#include "Math/MyMath.h"


/* PlayerのParticleの基底クラス */
class IPlayerParticle {

public:

	/// <summary>
	/// 仮想デストラク
	/// </summary>
	~IPlayerParticle() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw3D() = 0;


#pragma region Accessor アクセッサ

	// 死亡フラグ
	bool IsDead() const { return this->isDead_; }

	// モデル
	void SetModel(std::unique_ptr<Model> setModel) { this->model_ = std::move(setModel); }

	// スケール
	void SetScale(const Vector3& setScale) { this->trans_.srt.scale = setScale; }

	// 姿勢
	void SetRotate(const Vector3& setRotate) { this->trans_.srt.rotate = setRotate; }

	// 座標
	void SetTranslate(const Vector3& setTranslate) { this->trans_.srt.translate = setTranslate; }

	// カラー
	void SetColor(const Vector4& setColor) { this->color_ = setColor; }

#pragma endregion 


protected:

	//死亡フラグ
	bool isDead_ = false;

	// モデル
	std::unique_ptr<Model> model_;

	// トランスフォーム
	Transform trans_{};

	// カラー
	Vector4 color_ = Temp::Color::WHITE;

};