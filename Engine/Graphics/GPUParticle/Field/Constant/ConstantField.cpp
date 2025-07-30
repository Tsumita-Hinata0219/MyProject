#include "ConstantField.h"



/// <summary>
/// Fieldデータの取得
/// </summary>
void GpuField::ConstantField::Load_FieldData_From_Lua(const LuaScript& lua)
{
	GpuField::Data::ConstantField result;

	result.acceleration = lua.GetVariable<Vector3>("EmitConfig.acceleration");
	result.damping = lua.GetVariable<float>("damping");
	result.angularVelocity = lua.GetVariable<Vector3>("EmitConfig.angularVelocity");
	result.drag = lua.GetVariable<float>("EmitConfig.drag");
	result.force = lua.GetVariable<Vector3>("EmitConfig.force");
	result.mass = lua.GetVariable<float>("EmitConfig.mass");
	result.isUse = lua.GetVariable<bool>("EmitConfig.isUse");

	*fieldData_ = result;
}
