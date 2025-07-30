#pragma once

#include "GameObject/GameObject.h"



class DummyParticle {

public:

	DummyParticle();
	~DummyParticle() = default;

	void Init();
	void Update();
	void Draw();

	void ReLoad_ParticlePrope();
	void ReLoad_FadeOutData();

private:

	// パーティクルに必要なもの
	std::unique_ptr<Emitter::SphereEmitter> sEmit_;
	std::weak_ptr<Emitter::Data::SphereEmit> sEmitData_;
	std::unique_ptr<GpuField::ConstantField> cField_;
	std::unique_ptr<GpuParticle::Material::ParticleFadeOut> fadeOut_;

	// Luaスクリプト
	std::unique_ptr<LuaScript> particlePropeLua_;
	std::unique_ptr<LuaScript> movementLua_;
	std::unique_ptr<LuaScript> fadeOutLua_;

	// 確認用
	std::unique_ptr<Model> model_;
	ModelManager* modelManager_ = nullptr;
	Transform trans_{};
};
