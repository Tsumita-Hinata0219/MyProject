#pragma once

#include "GameObject/GameObject.h"
#include "../Property/EnemyProperty.h"
#include "../Bullet/EnemyBullet.h"
#include "ShotPattern/IEnemyShotPattern.h"
#include "ShotFactory/EnemyShotStateFactory.h"



// 前方宣言
class IEnemy;


/* Enemyそれぞれが持つ、Bulletの管理クラス */
class EnemyBulletContainer {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyBulletContainer() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBulletContainer() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// バレット追加
	/// </summary>
	void AddBulletInstance(Vector3 initVel);



#pragma region Accessor アクセッサ

	// 親エネミー
	void SetOwner(IEnemy* pEnemy) { this->owner_ = pEnemy; }

#pragma endregion


private:

	/// <summary>
	/// バレットのタイプの取得
	/// </summary>
	bool GetBulletState();


private:

	// 親エネミー
	IEnemy* owner_ = nullptr;

	// BulletのPool
	ObjectPool<EnemyBullet> bulletPool_;
	// BulletのList配列
	std::list<EnemyBullet*> bulletList_;

	// 射撃タイマー
	Timer shotTimer_{};

	// バレットの速度
	const float kBulletSpeed_ = 0.25f;

	// 射撃クラス
	std::unique_ptr<IEnemyShotPattern> exeShot_;

	std::mt19937 randEngine_; // ランダムエンジン
	std::uniform_int_distribution<> behabirDist_; // 0 or 1 の分布
	std::uniform_real_distribution<float> directionDist_; // -1 ~ +1 の分布


};