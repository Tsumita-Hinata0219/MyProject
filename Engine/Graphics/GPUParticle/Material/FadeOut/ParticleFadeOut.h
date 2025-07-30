#pragma once

#include "Resource/BufferResource/BufferResource.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "Transform/Structure/Transform.h"
#include "Graphics/PipeLineManager/PipeLineManager.h"

#include "../../Structure/ParticleStructure.h"
#include "../../Structure/MaterialStructure.h"
#include "../../Particle/GPUParticle.h"

#include "Lua/Script/LuaScript.h"

#include "Math/MyMath.h"


namespace GpuParticle {

namespace Material {

/* GPUParticleのFadeOut */
class ParticleFadeOut {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParticleFadeOut() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParticleFadeOut() = default;

	/// <summary>
	/// 生成
	/// </summary>
	void Create(std::weak_ptr<GPUParticle> pParticle);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// LuaScriptからFadeOutデータの読み込み
	/// </summary>
	void Load_Data_From_Lua(const LuaScript& lua);



#pragma region Accessor

#pragma endregion 


private:

	/// <summary>
	/// データ書き込み
	/// </summary>
	void WriteData();

	/// <summary>
	/// バインド & ディスパッチ
	/// </summary>
	void Bind_and_Dispatch();


protected:

	// パーティクルのPtr
	std::weak_ptr<GPUParticle> particlePtr_;

	// FadeOut
	std::shared_ptr<FadeOut> data_;
	BufferResource<FadeOut> buffer_;

};

}
}