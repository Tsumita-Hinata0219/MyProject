#include "DummyParticle.h"
#include "Graphics/3D/Model/Manager/ModelManager.h"


DummyParticle::DummyParticle()
{
	sEmit_ = std::make_unique<Emitter::SphereEmitter>();
	cField_ = std::make_unique<GpuField::ConstantField>();
	fadeOut_ = std::make_unique<GpuParticle::Material::ParticleFadeOut>();

	// Luaスクリプト
	particlePropeLua_ = std::make_unique<LuaScript>();
	movementLua_ = std::make_unique<LuaScript>();
	fadeOutLua_ = std::make_unique<LuaScript>();
	modelManager_ = ModelManager::GetInstance();
}

void DummyParticle::Init()
{
	// DummyParticle
	particlePropeLua_->LoadScript("LuaScript/Dummy", "DummyParticle_Params.lua");
	// DummyMovement
	movementLua_->LoadScript("LuaScript/Dummy", "Dummy_Movement.lua");
	// DummyFadeOut
	fadeOutLua_->LoadScript("LuaScript/Dummy", "Dummy_FadeOut.lua");

	// Emitter生成
	sEmit_->Create("Obj/Dev/Test", "Test.obj");
	// Field生成
	cField_->Create(sEmit_->GetWeak_Particle());
	// FadeOut
	fadeOut_->Create(sEmit_->GetWeak_Particle());

	// Luaから値を受け取る
	ReLoad_ParticlePrope();
	particlePropeLua_->SetReloadCallback([this]() { ReLoad_ParticlePrope(); });
	
	ReLoad_FadeOutData();
	fadeOutLua_->SetReloadCallback([this]() {ReLoad_FadeOutData(); });

	// 確認用Emitterの描画用
	modelManager_->LoadModel("Obj/Emitter/Sphere", "SphereEmitter.obj");
	model_ = modelManager_->GetModel("SphereEmitter");
	model_->SetRenderState(RenderState::FillMode_WireFrame);
	trans_.Init();
}

void DummyParticle::Update()
{
	// Emitterの座標を動かす処理
	if (auto lockedData = sEmit_->GetWeak_EmitData().lock()) {

		Vector4 vel = { 0.0f, 0.0f, 0.01f, 0.0f };
		// Lua側の関数でEmitterを動かす
		if (movementLua_->CallFunction("MoveCircular", vel)) {
			lockedData->translate = movementLua_->GetVariable<Vector4>("position");
		}

		// 確認用Emitterの座標更新
		trans_.srt.translate = {
			lockedData->translate.x,
			lockedData->translate.y,
			lockedData->translate.z,
		};
	}

	// EmitterとFieldの更新
	sEmit_->Update();
	sEmit_->TimerUpdate();
	cField_->Update();
	fadeOut_->Update();

	trans_.DrawImGui();

	//particlePropeLua_->ShowLuaEditorWindow();
	//particlePropeLua_->ShowAllLuaTables();
}

void DummyParticle::Draw()
{
	sEmit_->Draw3D();
	model_->Draw(trans_);
}

void DummyParticle::ReLoad_ParticlePrope()
{
	sEmit_->Load_EmitData_From_Lua(*particlePropeLua_);
	sEmit_->Load_EmitRangeData_From_Lua(*particlePropeLua_);
	sEmit_->Load_EmitConfigData_From_Lua(*particlePropeLua_);
	//cField_->Load_FieldData_From_Lua(*particlePropeLua_);
}

void DummyParticle::ReLoad_FadeOutData()
{
	fadeOut_->Load_Data_From_Lua(*fadeOutLua_);
}
