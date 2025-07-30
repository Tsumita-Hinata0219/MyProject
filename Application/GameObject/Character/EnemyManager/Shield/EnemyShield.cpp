#include "EnemyShield.h"



/// <summary>
/// 初期化処理
/// </summary>
void EnemyShield::Init()
{
	// BodyModelのロードと初期化
	modelManager_ = ModelManager::GetInstance();
	modelManager_->LoadModel("Obj/Enemys/Shield", "Shield.obj");
	model_ = modelManager_->GetModel("Shield");
	model_->SetRenderState(RenderState::CullingMode_None);

	// カラーの初期設定
	color_ = Vector4::one;
	color_.w = 0.5f;

	// uvTransformの初期設定
	uvtransform_.Init();

	// BodyTransfromの初期化
	transform_.Init();

	// Colliderの初期化
	sphere_ = std::make_unique<SphereCollider>(this);
	sphere_->data_.center = transform_.GetWorldPos();
	sphere_->data_.radius = 5.0f;

	// 破壊フラグは折っておく
	isBroken_ = false;
}


/// <summary>
/// 更新処理
/// </summary>
void EnemyShield::Update()
{
	// 壊れていたら処理に入らない
	if (isBroken_) { return; }

	// uvTransformの更新
	uvtransform_.UpdateMatrix();
	// 少しずつ値を上げる
	uvtransform_.srt.translate.y += 0.005f;

	// コライダー更新
	sphere_->data_.center = transform_.GetWorldPos();

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}


/// <summary>
/// 更新処理
/// </summary>
void EnemyShield::Draw3D()
{
	// 壊れていたら処理に入らない
	if (isBroken_) { return; }

	// BodyModelの描画
	model_->SetMaterialColor(color_);
	model_->SetMaterialUVMat(uvtransform_.matWorld);
	model_->Draw(transform_);
}
void EnemyShield::Draw2DFront()
{
}
void EnemyShield::Draw2DBack()
{
}


/// <summary>
/// 衝突判定コールバック関数
/// </summary>
void EnemyShield::onCollision([[maybe_unused]]IObject* object)
{
	if (object->GetCategory() == Attributes::Category::PLAYER &&
		object->GetType() == Attributes::Type::BULLET) {
		Log("Hit\n");
	}
}


/// <summary>
/// バリアが壊れた時の処理
/// </summary>
void EnemyShield::OnShieldBroken()
{
	// 破壊フラグ
	isBroken_ = true;

	// コライダーをマネージャーから除外
	sphere_->Deactivate();
}


/// <summary>
/// DrawImGuiの描画
/// </summary>
void EnemyShield::DrawImGui()
{
	if (ImGui::TreeNode("EnemyShield")) {

		transform_.DrawImGui();
		ImGui::Text("");

		ImGui::ColorEdit4("Color", &color_.x);
		ImGui::Text("");

		ImGui::DragFloat2("UVtransform_Translate", &uvtransform_.srt.translate.x, 0.01f);
		ImGui::Text("");

		ImGui::TreePop();
	}
}
