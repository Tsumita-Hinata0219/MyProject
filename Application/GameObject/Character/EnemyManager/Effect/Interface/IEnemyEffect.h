#pragma once

#include "System/IObject/IObject.h"
#include "GameObject/GameObject.h"


// 前方宣言
class IEnemy;


/* エネミーのエフェクトの基底クラス */
class IEnemyEffect {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IEnemyEffect();

	/// <summary>
	/// 仮想デストラク
	/// </summary>
	virtual ~IEnemyEffect() = default;

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

	// 親エネミー
	void SetEnemyPtr(IEnemy* enemyPtr) { this->pEnemy_ = enemyPtr; }

	// 死亡フラグ
	bool IsDead() const { return this->isDead_; }

	// SRTの設定
	void SetScale(const Vector3& setScale) { this->trans_.srt.scale = setScale; }
	void SetRotate(const Vector3& setRotate) { this->trans_.srt.rotate = setRotate; }
	void SetTranslate(const Vector3& setTranslate) { this->trans_.srt.translate = setTranslate; }

#pragma endregion 


protected:

	// 親エネミーのポインタ
	IEnemy* pEnemy_ = nullptr;

	// モデルマネージャー
	ModelManager* modelManager_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_;

	// トランスフォーム
	Transform trans_{};

	// 死亡フラグ
	bool isDead_ = false;

	// タイマー
	Timer timer_{};

};