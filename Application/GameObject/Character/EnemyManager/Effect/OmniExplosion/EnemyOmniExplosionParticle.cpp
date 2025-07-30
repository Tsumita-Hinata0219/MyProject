#include "EnemyOmniExplosionParticle.h"

void EnemyOmniExplosionParticle::Init()
{
	emitter_ = std::make_unique<Emitter::SphereEmitter>();
	emitter_->Create("Obj/Enemys/Effect/ExplosionParticle", "ExplosionParticle.obj");
	field_ = std::make_unique<GpuField::ConstantField>();
	field_->Create(emitter_->GetWeak_Particle());

	particlePropeLua_ = std::make_unique<LuaScript>();
	particlePropeLua_->LoadScript("LuaScript/EnemyOmniExplosion", "EnemyOmniExplosion_Params.lua");

	// Luaから値を受け取る
	emitter_->Load_EmitData_From_Lua(*particlePropeLua_);
	emitter_->Load_EmitRangeData_From_Lua(*particlePropeLua_);
	emitter_->Load_EmitConfigData_From_Lua(*particlePropeLua_);
	field_->Load_FieldData_From_Lua(*particlePropeLua_);
	particlePropeLua_->SetReloadCallback([this]() { ReLoad_ParticlePrope(); });
}

void EnemyOmniExplosionParticle::Update()
{
	emitter_->Update();
	field_->Update();
}

void EnemyOmniExplosionParticle::Draw()
{
	emitter_->Draw3D();
}

void EnemyOmniExplosionParticle::Emit()
{
	emitter_->Emit();
}

void EnemyOmniExplosionParticle::SetEmitPos(Vector3 setPos)
{
	if (auto lokedData = emitter_->GetWeak_EmitData().lock()) {
		Vector4 newPos = {
			setPos.x, setPos.y, setPos.z, 0.0f
		};
		lokedData->translate = newPos;
	}
}

void EnemyOmniExplosionParticle::ReLoad_ParticlePrope()
{
	emitter_->Load_EmitData_From_Lua(*particlePropeLua_);
	emitter_->Load_EmitRangeData_From_Lua(*particlePropeLua_);
	emitter_->Load_EmitConfigData_From_Lua(*particlePropeLua_);
	field_->Load_FieldData_From_Lua(*particlePropeLua_);
}
