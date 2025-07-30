#include "PlayerParticleManager.h"
#include "../../Player.h"


/// <summary>
/// 初期化処理
/// </summary>
void PlayerParticleManager::Init()
{
	// モデルの読み込み
	modelManager_ = ModelManager::GetInstance();
	modelManager_->LoadModel("Obj/MovementParticle1", "MovementParticle1.obj");
	modelManager_->LoadModel("Obj/MovementParticle2", "MovementParticle2.obj");

	particlePool_.Create(30);

	// タイマー
	movement1Timer_.Init(0.0f, 0.5f * 60.0f); // (0.5秒)
	movement2Timer_.Init(0.0f, 0.25f * 60.0f); // (0.25秒)
}


/// <summary>
/// 更新処理
/// </summary>
void PlayerParticleManager::Update()
{
	MovementParticle();

	// IParticle
	for (auto& element : particleList_) {
		element->Update();
	}
	// 死亡フラグが立っていたら削除
	particleList_.remove_if([this](PlayerMovementParticle* particle) {
		if (particle->IsDead()) {
			// 死亡したバレットをプールに返却
			particlePool_.Return(particle);
			return true;
		}
		return false;
		}
	);

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}


/// <summary>
/// 描画処理
/// </summary>
void PlayerParticleManager::Draw()
{
	// IParticle
	for (auto& element : particleList_) {
		element->Draw3D();
	}
}


void PlayerParticleManager::MovementParticle()
{
	if (player_->IsMovement()) {

		movement1Timer_.Update(true);
		movement2Timer_.Update(true);


		if (movement1Timer_.IsFinish()) {
			AddMovementPartiucle1();
		}
		if (movement2Timer_.IsFinish()) {
			AddMovementPartiucle2();
		}
	}
}


/// <summary>
/// 移動パーティクルの追加
/// </summary>
void PlayerParticleManager::AddMovementPartiucle1()
{
	// 追加するインスタンス
	PlayerMovementParticle* newParticle = particlePool_.Get();

	newParticle->Init(); // 初期化
	newParticle->SetModel(modelManager_->GetModel("MovementParticle1"));
	Scope scope = { -1.0f, 1.0f };
	Vector3 diff = {
		RandomGenerator::getRandom(scope),
		RandomGenerator::getRandom(scope),
		RandomGenerator::getRandom(scope),
	};
	newParticle->SetTranslate(player_->GetWorldPos() + diff);
	newParticle->SetRotate(diff);

	this->particleList_.push_back(newParticle);
}
void PlayerParticleManager::AddMovementPartiucle2()
{
	// 追加するインスタンス
	PlayerMovementParticle* newParticle = particlePool_.Get();

	newParticle->Init(); // 初期化
	newParticle->SetModel(modelManager_->GetModel("MovementParticle2"));
	Scope scope = { -1.0f, 1.0f };
	Vector3 diff = {
		RandomGenerator::getRandom(scope),
		RandomGenerator::getRandom(scope),
		RandomGenerator::getRandom(scope),
	};
	newParticle->SetTranslate(player_->GetWorldPos() + diff);
	newParticle->SetRotate(diff);

	this->particleList_.push_back(newParticle);
}


/// <summary>
/// ImGuiの描画
/// </summary>
void PlayerParticleManager::DrawImGui()
{
	if (ImGui::TreeNode("PlayerParticleManager")) {

		if (ImGui::Button("Add_MovementParticle1")) {
			AddMovementPartiucle1();
		}
		if (ImGui::Button("Add_MovementParticle2")) {
			AddMovementPartiucle2();
		}
		ImGui::Text("");


		ImGui::TreePop();
	}
}
