#pragma once

#include "GameObject/GameObject.h"


class EnemyOmniBackParticle {

public:

	EnemyOmniBackParticle() = default;
	~EnemyOmniBackParticle() = default;

	void Init();

	void Update();

	void Draw();

	void Emit();

	void SetEmitPos(Vector3 setPos);

	void ReLoad_ParticlePrope();

private:

	std::unique_ptr<Emitter::SphereEmitter> emitter_;
	std::unique_ptr<GpuField::ConstantField> field_;


	std::unique_ptr<LuaScript> particlePropeLua_;
};