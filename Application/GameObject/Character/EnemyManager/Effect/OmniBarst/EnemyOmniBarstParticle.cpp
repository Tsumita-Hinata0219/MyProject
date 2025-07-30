#include "EnemyOmniBarstParticle.h"

void EnemyOmniBarstParticle::Init()
{
	emitter_ = std::make_unique<Emitter::SphereEmitter>();
	emitter_->Create("Obj/Enemys/Effect/BarstParticle", "BarstParticle.obj");
	field_ = std::make_unique<GpuField::ConstantField>();
	field_->Create(emitter_->GetWeak_Particle());

	particlePropeLua_ = std::make_unique<LuaScript>();
	particlePropeLua_->LoadScript("LuaScript/EnemyOmniBarst", "EnemyOmniBarst_Params.lua");

	// Luaから値を受け取る
	emitter_->Load_EmitData_From_Lua(*particlePropeLua_);
	emitter_->Load_EmitRangeData_From_Lua(*particlePropeLua_);
	emitter_->Load_EmitConfigData_From_Lua(*particlePropeLua_);
	field_->Load_FieldData_From_Lua(*particlePropeLua_);
	particlePropeLua_->SetReloadCallback([this]() { ReLoad_ParticlePrope(); });
}

void EnemyOmniBarstParticle::Update()
{
	emitter_->Update();
	field_->Update();
}

void EnemyOmniBarstParticle::Draw()
{
	emitter_->Draw3D();
}

void EnemyOmniBarstParticle::Emit()
{
	emitter_->Emit();
}

void EnemyOmniBarstParticle::SetEmitPos(Vector3 setPos)
{
	if (auto lokedData = emitter_->GetWeak_EmitData().lock()) {
		Vector4 newPos = {
			setPos.x, setPos.y, setPos.z, 0.0f
		};
		lokedData->translate = newPos;
	}
}

void EnemyOmniBarstParticle::ReLoad_ParticlePrope()
{
	emitter_->Load_EmitData_From_Lua(*particlePropeLua_);
	emitter_->Load_EmitRangeData_From_Lua(*particlePropeLua_);
	emitter_->Load_EmitConfigData_From_Lua(*particlePropeLua_);
	field_->Load_FieldData_From_Lua(*particlePropeLua_);
}
