#include "EnemyBulletContainer.h"
#include "../Enemy/IEnemy.h"



/// <summary>
/// 初期化処理
/// </summary>
void EnemyBulletContainer::Init()
{
	// バレットの新スタンスを予め作っておく
	bulletPool_.Create(50);

	// 射撃タイマーの設定 & スタート
	shotTimer_.Init(0.0f, owner_->GetShotProperty().shotInterval);
	shotTimer_.Start();

	// ランダム生成器作成
	std::random_device rd;
	randEngine_ = std::mt19937(rd());
	behabirDist_ = std::uniform_int_distribution<>(0, 1);
	directionDist_ = std::uniform_real_distribution<float>(-1.0f, 1.0f);

	// 射撃ステートの設定
	exeShot_ = EnemyShotPatternFactory::CreateShotPattern(owner_->GetShotProperty().shotPattern);
	exeShot_->Init(this);
}


/// <summary>
/// 更新処理
/// </summary>
void EnemyBulletContainer::Update()
{
	// タイマー更新・終了してもループ
	shotTimer_.Update(true);
	// タイマー終了で射撃
	if (shotTimer_.IsFinish()) {
		exeShot_->Execute();
	}

	// Bulletの更新
	for (EnemyBullet* bullet : bulletList_) {
		bullet->Update();
	}
	bulletList_.remove_if([this](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			// リセット処理
			bullet->Reset();
			bulletPool_.Return(bullet);
			return true;
		}
		return false;
		}
	);
}


/// <summary>
/// 描画処理
/// </summary>
void EnemyBulletContainer::Draw()
{
	for (EnemyBullet* bullet : bulletList_) {
		bullet->Draw3D();
	}
}


/// <summary>
/// バレット追加
/// </summary>
void EnemyBulletContainer::AddBulletInstance(Vector3 initVel)
{
	// オブジェクトプールから新しいバレットを取得
	EnemyBullet* newBullet = bulletPool_.Get();
	
	// newBulletの初期化
	newBullet->SetResistant(GetBulletState());
	newBullet->Init();
	newBullet->SetTranslate(owner_->GetWorldPos());
	newBullet->SetVelocity(TransformNormal(Normalize(initVel) * kBulletSpeed_, owner_->GetMatWorld()));
	newBullet->SetRotationFromVelocity();

	// リストに追加
	bulletList_.push_back(newBullet);
}


/// <summary>
/// バレットのタイプの取得
/// </summary>
bool EnemyBulletContainer::GetBulletState()
{
	if (owner_->GetShotProperty().behavior == enemy::BulletBehavior::Common)
	{
		return false;
	}
	else if (owner_->GetShotProperty().behavior == enemy::BulletBehavior::Resistant)
	{
		return true;
	}
	else if (owner_->GetShotProperty().behavior == enemy::BulletBehavior::Random)
	{
		return behabirDist_(randEngine_) == 1;
	}

	// どの条件にも当てはまらない場合、デフォルト値を返す
	return false;
}

