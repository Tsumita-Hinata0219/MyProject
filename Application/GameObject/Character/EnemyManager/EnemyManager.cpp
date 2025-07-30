#include "EnemyManager.h"
#include "../Player/Player.h"


/// <summary>
/// 初期化処理
/// </summary>
void EnemyManager::Init()
{
	// エネミー関連のモデルの読み込み
	modelManager_ = ModelManager::GetInstance();
	modelManager_->LoadModel("Obj/Enemys/Bullet", "EnemyBullet.obj");

	// 全滅したかのフラグは折っておく
	isCommonEliminated_ = false;
	isBossEliminated_ = false;
}


/// <summary>
/// 更新処理
/// </summary>
void EnemyManager::Update()
{
	// 全滅していたら処理を通さない
	if (isBossEliminated_) { return; }

	// EnemyListの更新処理
	for (auto& enemy : commonEnemies_) {
		enemy->Update();
	}
	// 死亡フラグが立っていたら削除
	commonEnemies_.remove_if([](std::unique_ptr<IEnemy>& enemy) {
		if (enemy->IsDead()) {
			return true;
		}
		return false;
		}
	);

	// BossEnemyListの更新処理
	for (auto& boss : bossEnemies_) {
		boss->Update();
	}
	// 死亡フラグが立っていたら削除
	bossEnemies_.remove_if([](std::unique_ptr<BossEnemy>& boss) {
		if (boss->IsDead()) {
			return true;
		}
		return false;
		}
	);

	// OmniBackParticleの更新処理
	for (auto& particle : backParticles_) {
		particle->Update();
	}
	// OmniBarstParticleの更新処理
	for (auto& particle : barstParticles_) {
		particle->Update();
	}
	// OmniExplosionParticleの更新処理
	for (auto& particle : explosionParticles_) {
		particle->Update();
	}

	// 全滅したかのチェック
	EliminatedChecker();


#ifdef _DEBUG
	DrawimGui();
#endif // _DEBUG
}


/// <summary>
/// 描画処理
/// </summary>
void EnemyManager::Draw3D()
{
	// EnemyListの描画
	for (auto& enemy : commonEnemies_) {
		enemy->Draw3D();
	}
	for (auto& boss : bossEnemies_) {
		boss->Draw3D();
	}
	// OmniBackParticleの描画処理
	for (auto& particle : backParticles_) {
		particle->Draw();
	}
	// OmniBarstParticleListの描画
	for (auto& particle : barstParticles_) {
		particle->Draw();
	}
	// OmniExplosionParticleの更新処理
	for (auto& particle : explosionParticles_) {
		particle->Draw();
	}
}


/// <summary>
/// Jsonで読み込んだ情報を受け取る
/// </summary>
void EnemyManager::LoadEntityData(const std::vector<std::unique_ptr<EntityData>>& datas)
{
	for (const auto& entityData : datas) {
		if (entityData) {

			if (entityData->entityName == "BasicEnemy") {
				CreateBasicEnemy(*entityData.get());
			}
			else if (entityData->entityName == "StaticEnemy") {
				CreateStaticEnemy(*entityData.get());
			}
			else if (entityData->entityName == "BossEnemy") {
				CreateBossEnemy(*entityData.get());
			}
		}
	}
}


/// <summary>
/// 全滅したかのチェック
/// </summary>
void EnemyManager::EliminatedChecker()
{
	// 通常エネミーが全滅したかのチェックをし、
	// trueになったら、ボスエネミーのチェックに行く
	if (!isCommonEliminated_) {
		Common_EliminatedChecker();
	}
	else {
		Boss_EliminatedChecker();
	}
}


/// <summary>
/// 通常エネミーが全滅したかのチェック
/// </summary>
void EnemyManager::Common_EliminatedChecker()
{
	int instanceNum = int(commonEnemies_.size());

	// インスタンスが0なので全滅フラグを立てる
	if (instanceNum <= 0) {

		isCommonEliminated_ = true;
		
		// ボスのシールドを破壊
		for (auto& boss : bossEnemies_) {
			boss->CollapseShield();
		}
	}
}


/// <summary>
/// ボスエネミーが全滅したかのチェック
/// </summary>
void EnemyManager::Boss_EliminatedChecker()
{
	int instanceNum = int(bossEnemies_.size());

	// インスタンスが0なので全滅フラグを立てる
	if (instanceNum <= 0) {
		isBossEliminated_ = true;
	}
}


/// <summary>
/// 新しいEnemyを生成する
/// </summary>
void EnemyManager::CreateBasicEnemy(const EntityData& setEntityData)
{
	// 新しいEnemyのインスタンス
	std::unique_ptr<BasicEnemy> newEnemy = std::make_unique<BasicEnemy>();
	std::shared_ptr<EnemyOmniBackParticle> newBackPart = std::make_unique<EnemyOmniBackParticle>();
 	std::shared_ptr<EnemyOmniBarstParticle> newBarstPart = std::make_shared<EnemyOmniBarstParticle>();
	std::shared_ptr<EnemyOmniExplosionParticle> newexplosionPart = std::make_shared<EnemyOmniExplosionParticle>();
	newBackPart->Init();
	newBackPart->Update();
	newBarstPart->Init();
	newBarstPart->Update();
	newexplosionPart->Init();
	newexplosionPart->Update();

	// newEnemyの初期化
	newEnemy->SetPlayer(this->player_);
	newEnemy->SetEnemyManager(this);
	newEnemy->SetOmniBackParticle(newBackPart);
	newEnemy->SetOmniBarstParticle(newBarstPart);
	newEnemy->SetOmniExplosionParticle(newexplosionPart);
	newEnemy->SetShotProperty(setEntityData.enemyData.shotFuncData);
	newEnemy->SetMovementProperty(setEntityData.enemyData.movementFuncData);
	newEnemy->SetInitSRT(setEntityData.srt);
	newEnemy->Init();

	// リストに追加
	commonEnemies_.push_back(std::move(newEnemy));
	backParticles_.push_back(std::move(newBackPart));
	barstParticles_.push_back(std::move(newBarstPart));
	explosionParticles_.push_back(std::move(newexplosionPart));
}
void EnemyManager::CreateStaticEnemy(const EntityData& setEntityData)
{
	// 新しいEnemyのインスタンス
	std::unique_ptr<StaticEnemy> newEnemy = std::make_unique<StaticEnemy>();
	std::shared_ptr<EnemyOmniBackParticle> newBackPart = std::make_unique<EnemyOmniBackParticle>();
	std::shared_ptr<EnemyOmniBarstParticle> newBarstPart = std::make_shared<EnemyOmniBarstParticle>();
	std::shared_ptr<EnemyOmniExplosionParticle> newexplosionPart = std::make_shared<EnemyOmniExplosionParticle>();
	newBackPart->Init();
	newBackPart->Update();
	newBarstPart->Init();
	newBarstPart->Update();
	newexplosionPart->Init();
	newexplosionPart->Update();

	// newEnemyの初期化
	newEnemy->SetPlayer(this->player_);
	newEnemy->SetEnemyManager(this);
	newEnemy->SetOmniBackParticle(newBackPart);
	newEnemy->SetOmniBarstParticle(newBarstPart);
	newEnemy->SetOmniExplosionParticle(newexplosionPart);
	newEnemy->SetShotProperty(setEntityData.enemyData.shotFuncData);
	newEnemy->SetMovementProperty(setEntityData.enemyData.movementFuncData);
	newEnemy->SetInitSRT(setEntityData.srt);
	newEnemy->Init();

	// リストに追加
	commonEnemies_.push_back(std::move(newEnemy));
	backParticles_.push_back(std::move(newBackPart));
	barstParticles_.push_back(std::move(newBarstPart));
	explosionParticles_.push_back(std::move(newexplosionPart));
}
void EnemyManager::CreateBossEnemy(const EntityData& setEntityData)
{
	// 新しいインスタンス
	std::unique_ptr<BossEnemy> newEnemy = std::make_unique<BossEnemy>();
	std::shared_ptr<EnemyOmniBackParticle> newBackPart = std::make_unique<EnemyOmniBackParticle>();
	std::shared_ptr<EnemyOmniBarstParticle> newBarstPart = std::make_shared<EnemyOmniBarstParticle>();
	std::shared_ptr<EnemyOmniExplosionParticle> newexplosionPart = std::make_shared<EnemyOmniExplosionParticle>();
	newBackPart->Init();
	newBackPart->Update();
	newBarstPart->Init();
	newBarstPart->Update();
	newexplosionPart->Init();
	newexplosionPart->Update();

	// newEnemyの初期化
	newEnemy->SetPlayer(this->player_);
	newEnemy->SetEnemyManager(this);
	newEnemy->SetOmniBackParticle(newBackPart);
	newEnemy->SetOmniBarstParticle(newBarstPart);
	newEnemy->SetOmniExplosionParticle(newexplosionPart);
	newEnemy->SetShotProperty(setEntityData.enemyData.shotFuncData);
	newEnemy->SetMovementProperty(setEntityData.enemyData.movementFuncData);
	newEnemy->SetInitSRT(setEntityData.srt);
	newEnemy->Init();

	// リストに追加
	bossEnemies_.push_back(std::move(newEnemy));
	backParticles_.push_back(std::move(newBackPart));
	barstParticles_.push_back(std::move(newBarstPart));
	explosionParticles_.push_back(std::move(newexplosionPart));
}


/// <summary>
/// ImGuiの描画
/// </summary>
void EnemyManager::DrawimGui()
{
	if (ImGui::TreeNode("EnemyManager")) {

		ImGui::Text("IEnemyInstance = %d", int(commonEnemies_.size()));
		ImGui::Text("");

		ImGui::TreePop();
	}
}

