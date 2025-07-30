#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include "../Particle/GPUParticle.h"
#include "../Emitter/Interface/IEmitter.h"
#include "../Field/ParticleField.h"


/* パーティクルマネージャ */
class ParticleManager {

private: // シングルトン

	// コンストラクタ、デストラクタ
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	const ParticleManager& operator=(const ParticleManager&) = delete;


public:

	/// <summary>
	/// インスタンスの取得
	/// <returns></returns>
	static ParticleManager* GetInstance() {
		static ParticleManager instance;
		return &instance;
	}

	/// <summary>
	/// パーティクル生成
	/// </summary>
	void Create();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private:


private:

	
};

