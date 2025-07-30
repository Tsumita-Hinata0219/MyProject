#pragma once

#include "../../Structure/FieldStructure.h"
#include "../../Structure/EmitterStructure.h"
#include "../../Structure/ParticleStructure.h"
#include "../../Resources/ParticleResources.h"
#include "../../Particle/GPUParticle.h"
#include "Lua/Script/LuaScript.h"

#include  "Utilities/RandomGenerator/RandomGenerator.h"
#include "Utilities/TimeSystem/TimeSystem.h"


/* GPUParticleのフィールド */
template <typename T>
class IParticleField {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IParticleField() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IParticleField() = default;

	/// <summary>
	/// 生成
	/// </summary>
	virtual void Create(std::weak_ptr<GPUParticle> pParticle);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// Fieldデータの取得
	/// </summary>
	virtual void Load_FieldData_From_Lua(const LuaScript& lua) = 0;


#pragma region Accessor

	// Emitter
	virtual const std::weak_ptr<T> GetWeak_FieldData() { return this->fieldData_; }

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

	// Field
	std::shared_ptr<T> fieldData_;
	BufferResource<T> fieldBuffer_;

};



/// <summary>
/// 生成
/// </summary>
template<typename T>
inline void IParticleField<T>::Create(std::weak_ptr<GPUParticle> pParticle)
{
	// パーティクルのptrを受け取る
	particlePtr_ = pParticle;

	// Emitterデータ作成
	fieldData_ = std::make_shared<T>();
	// Emitterデータのバッファー作成
	fieldBuffer_.CreateCBV();
}


/// <summary>
/// 更新処理
/// </summary>
template<typename T>
inline void IParticleField<T>::Update()
{



	// Dispach
	Bind_and_Dispatch();
}


/// <summary>
/// データ書き込み
/// </summary>
template<typename T>
inline void IParticleField<T>::WriteData()
{
	// FieldData
	fieldBuffer_.UpdateData(fieldData_.get());
}


/// <summary>
/// バインド & ディスパッチ
/// </summary>
template<typename T>
inline void IParticleField<T>::Bind_and_Dispatch()
{
	// データの書き込み
	WriteData();

	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::SetPipeLine(PipeLine::Container::Compute, PipeLine::Category::ConstantField);

	// Particle
	particlePtr_.lock()->Bind_ParticleProp(0);

	// Field
	fieldBuffer_.BindComputeCBV(1);

	// Dispach
	commands.List->Dispatch(1, 1, 1);

	// Barrierを張る
	particlePtr_.lock()->SetUAVBarrier();
}
