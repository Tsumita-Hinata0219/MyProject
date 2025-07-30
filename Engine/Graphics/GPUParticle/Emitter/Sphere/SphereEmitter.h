#pragma once

#include "../Interface/IEmitter.h"


namespace Emitter {

class SphereEmitter : public IEmitter<Emitter::Data::SphereEmit> {

public:

	/// <summary>
	/// コンストラク
	/// </summary>
	SphereEmitter()
	{
		pipeLine_Category = PipeLine::Category::SphereEmitter;
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SphereEmitter() = default;

	/// <summary>
	/// LuaScriptからEmitterデータの読み込み
	/// </summary>
	void Load_EmitData_From_Lua(const LuaScript& lua) override;


private:

};

}