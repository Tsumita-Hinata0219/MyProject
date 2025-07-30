#pragma once

#include "../Interface/IParticleField.h"


namespace GpuField {

class ConstantField : public IParticleField<GpuField::Data::ConstantField> {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ConstantField() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ConstantField() = default;

	/// <summary>
	/// Fieldデータの取得
	/// </summary>
	void Load_FieldData_From_Lua(const LuaScript& lua) override;

private:

};
}