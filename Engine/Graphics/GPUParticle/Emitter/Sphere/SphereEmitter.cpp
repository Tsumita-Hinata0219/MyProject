#include "SphereEmitter.h"
#include "Lua/Script/LuaScript.h"


/// <summary>
/// LuaScriptからEmitterデータの読み込み
/// </summary>
void Emitter::SphereEmitter::Load_EmitData_From_Lua(const LuaScript& lua)
{
	Emitter::Data::SphereEmit result;

	result.translate = lua.GetVariable<Vector4>("SphereEmitter.translate");
	result.radius = lua.GetVariable<Vector4>("SphereEmitter.radius");

	*emitData_ = result;
}
