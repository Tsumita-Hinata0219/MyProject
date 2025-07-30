#pragma once

#include "Resource/BufferResource/BufferResource.h"
#include "../../Structure/EmitterStructure.h"
#include "../../Structure/ParticleStructure.h"
#include "../../Structure/FieldStructure.h"
#include "../../Particle/GPUParticle.h"
#include "Lua/Script/LuaScript.h"

#include  "Utilities/RandomGenerator/RandomGenerator.h"
#include "Utilities/TimeSystem/TimeSystem.h"


/* Emitterの基底クラス */
template <typename T>
class IEmitter {

public:

	/// <summary>
	/// コンストラク
	/// </summary>
	IEmitter() = default;

	/// <summary>
	/// デストラク
	/// </summary>
	~IEmitter() = default;

	/// <summary>
	/// 生成
	/// </summary>
	void Create(const std::string& rootPath, const std::string& fileName);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// タイマー更新
	/// </summary>
	void TimerUpdate();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw3D();

	/// <summary>
	/// 発生
	/// </summary>
	void Emit();

	/// <summary>
	/// LuaScriptからEmitterデータの読み込み
	/// </summary>
	virtual void Load_EmitData_From_Lua(const LuaScript& lua) = 0;

	/// <summary>
	/// LuaScriptからEmitRangeデータの読み込み
	/// </summary>
	void Load_EmitRangeData_From_Lua(const LuaScript& lua);

	/// <summary>
	/// LuaScriptからEmitConfigデータの読み込み
	/// </summary>
	void Load_EmitConfigData_From_Lua(const LuaScript& lua);


#pragma region Accessor

	// Particle
	const std::weak_ptr<GPUParticle> GetWeak_Particle() { return this->particle_; }

	// Emitter
	const std::weak_ptr<T> GetWeak_EmitData() { return this->emitData_; }

	// Emitter Range
	const std::weak_ptr<Emitter::Data::EmitRange> GetWeak_RangeData() { return this->rangeData_; }

	// Emitter Config
	const std::weak_ptr<Emitter::Data::EmitConfig> GetWeak_ConfigData() { return this->configData_; }

#pragma endregion 


private:

	/// <summary>
	/// Data生成
	/// </summary>
	void CreateData();

	/// <summary>
	/// Buffer生成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// 射出データの更新
	/// </summary>
	void Update_ConfigData();

	/// <summary>
	/// 乱数シードデータの更新
	/// </summary>
	void Update_RandomSeedData();

	/// <summary>
	/// 発生させるかのフラグチェック
	/// </summary>
	void Check_Emitting();

	/// <summary>
	/// Bufferへデータ書き込み
	/// </summary>
	void WriteData();

	/// <summary>
	/// パーティクルを発生させる
	/// </summary>
	void DispatchEmitCompute();


protected:

	// PipeLineの種類
	PipeLine::Category pipeLine_Category = PipeLine::Category::None;
	PipeLine::SubFilter pipeLine_SubFileter = PipeLine::SubFilter::None;


protected:

	// パーティクル
	std::shared_ptr<GPUParticle> particle_;

	// エミッター
	std::shared_ptr<T> emitData_;
	BufferResource<T> emitBuff_;

	// 範囲
	std::shared_ptr<Emitter::Data::EmitRange> rangeData_;
	BufferResource<Emitter::Data::EmitRange> rangeBuff_;

	// 射出
	std::shared_ptr<Emitter::Data::EmitConfig> configData_;
	BufferResource<Emitter::Data::EmitConfig> configBuff_;

	// 乱数シード
	std::shared_ptr<Emitter::Data::RandomSeed> randSeedData_;
	BufferResource<Emitter::Data::RandomSeed> randSeedBuff_;

	// 前回リセットした時点の時間
	float lastResetTime_ = 0.0f;
	// 時間記録クラス
	TimeSystem* timeSys_ = nullptr;
};



/// <summary>
/// 生成
/// </summary>
template<typename T>
inline void IEmitter<T>::Create(const std::string& rootPath, const std::string& fileName)
{
	// TimeSystem
	timeSys_ = TimeSystem::GetInstance();

	// Data作成
	CreateData();

	// Buffer作成
	CreateBuffer();

	// パーティクル初期化
	particle_->Init(rootPath, fileName);
}


/// <summary>
/// 更新処理
/// </summary>
template<typename T>
inline void IEmitter<T>::Update()
{
	// 乱数シード更新
	Update_RandomSeedData();

	// 発生フラグチェック
	Check_Emitting();

	// データ書き込み
	WriteData();

	// Dispatch
	DispatchEmitCompute();

	// パーティクル更新
	particle_->Update();
}


/// <summary>
/// タイマー更新
/// </summary>
template<typename T>
inline void IEmitter<T>::TimerUpdate()
{
	// 射出更新
	Update_ConfigData();
}


/// <summary>
/// 描画処理
/// </summary>
template<typename T>
inline void IEmitter<T>::Draw3D()
{
	// パーティクル描画
	particle_->Draw();
}


/// <summary>
/// 発生
/// </summary>
template<typename T>
inline void IEmitter<T>::Emit()
{
	configData_->isEmitting = true;

	// データ書き込み
	WriteData();

	// Dispatch
	DispatchEmitCompute();
}


/// <summary>
/// LuaScriptからEmitRangeデータの読み込み
/// </summary>
template<typename T>
inline void IEmitter<T>::Load_EmitRangeData_From_Lua(const LuaScript& lua)
{
	Emitter::Data::EmitRange result;

	result.scaleMin = lua.GetVariable<Vector4>("EmitterRange.scaleMin");
	result.scaleMax = lua.GetVariable<Vector4>("EmitterRange.scaleMax");

	result.rotateMin = lua.GetVariable<Vector4>("EmitterRange.rotateMin");
	result.rotateMax = lua.GetVariable<Vector4>("EmitterRange.rotateMax");

	result.colorMin = lua.GetVariable<Vector4>("EmitterRange.colorMin");
	result.colorMax = lua.GetVariable<Vector4>("EmitterRange.colorMax");

	result.velocityMin = lua.GetVariable<Vector4>("EmitterRange.velocityMin");
	result.velocityMax = lua.GetVariable<Vector4>("EmitterRange.velocityMax");

	result.baseLifeTime = lua.GetVariable<float>("EmitterRange.baseLifeTime");
	result.lifeTimeMin = lua.GetVariable<float>("EmitterRange.lifeTimeMin");
	result.lifeTimeMax = lua.GetVariable<float>("EmitterRange.lifeTimeMax");

	*rangeData_ = result;
}


/// <summary>
/// LuaScriptからEmitConfigデータの読み込み
/// </summary>
template<typename T>
inline void IEmitter<T>::Load_EmitConfigData_From_Lua(const LuaScript& lua)
{
	Emitter::Data::EmitConfig result;

	result.spawnInterval = lua.GetVariable<float>("EmitConfig.spawnInterval");
	result.elapsedTime = lua.GetVariable<float>("EmitConfig.elapsedTime");
	result.spawnCount = lua.GetVariable<int>("EmitConfig.spawnCount");
	result.isEmitting = lua.GetVariable<bool>("EmitConfig.isEmitting");

	*configData_ = result;
}


/// <summary>
/// Data生成
/// </summary>
template<typename T>
inline void IEmitter<T>::CreateData()
{
	// Particle
	particle_ = std::make_shared<GPUParticle>();
	// Emitter
	emitData_ = std::make_shared<T>();
	// Range
	rangeData_ = std::make_shared<Emitter::Data::EmitRange>();
	// Congig
	configData_ = std::make_shared<Emitter::Data::EmitConfig>();
	// RandomSeed
	randSeedData_ = std::make_shared<Emitter::Data::RandomSeed>();
	randSeedData_->gameTime = timeSys_->Get_SinceStart();
	randSeedData_->dynamicTime = RandomGenerator::getRandom(Scope(0.0f, 1000.0f));
}


/// <summary>
/// Buffer生成
/// </summary>
template<typename T>
inline void IEmitter<T>::CreateBuffer()
{
	// Emitter
	emitBuff_.CreateCBV();
	// Range
	rangeBuff_.CreateCBV();
	// Congig
	configBuff_.CreateCBV();
	// RandomSeed
	randSeedBuff_.CreateCBV();
}


/// <summary>
/// 射出データの更新
/// </summary>
template<typename T>
inline void IEmitter<T>::Update_ConfigData()
{
	configData_->elapsedTime -= 1.0f / 60.0f; // 減算
}


/// <summary>
/// 乱数シードデータの更新
/// </summary>
template<typename T>
inline void IEmitter<T>::Update_RandomSeedData()
{
	// ゲーム開始からの時間を取得
	randSeedData_->gameTime = timeSys_->Get_SinceStart();

	// 初期乱数値 + 開始からの時間
	randSeedData_->dynamicTime = RandomGenerator::getRandom(Scope(0.0f, 1000.0f));
	randSeedData_->dynamicTime += randSeedData_->gameTime;

	// 600秒（10分）ごとにリセット
	if (randSeedData_->gameTime - lastResetTime_ >= 600.0f) {
		randSeedData_->dynamicTime = RandomGenerator::getRandom(Scope(0.0f, 1000.0f));
		lastResetTime_ = randSeedData_->gameTime;
	}
}


/// <summary>
/// 発生させるかのフラグチェック
/// </summary>
template<typename T>
inline void IEmitter<T>::Check_Emitting()
{
	// タイマー0以下で
	// 射出許可 & タイマーリセット
	if (configData_->elapsedTime <= 0.0f) {
		// タイマーリセット
		configData_->elapsedTime = configData_->spawnInterval;
		configData_->isEmitting = true;
	}
	else {
		configData_->isEmitting = false;
	}
}


/// <summary>
/// Bufferへデータ書き込み
/// </summary>
template<typename T>
inline void IEmitter<T>::WriteData()
{
	// Emitter
	emitBuff_.UpdateData(emitData_.get());

	// range
	rangeBuff_.UpdateData(rangeData_.get());

	// config
	configBuff_.UpdateData(configData_.get());

	// randomSeed
	randSeedBuff_.UpdateData(randSeedData_.get());
}


/// <summary>
/// パーティクルを発生させる
/// </summary>
template<typename T>
inline void IEmitter<T>::DispatchEmitCompute()
{
	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::SetPipeLine(PipeLine::Container::Compute, pipeLine_Category, pipeLine_SubFileter);

	// Particle
	particle_->Bind_ParticleProp(0);

	// LifeTime
	particle_->Bind_ParticleLifeTime(1);

	// FreeList
	particle_->Bind_FreeList(2);

	// FreeListIndex
	particle_->Bind_FreeListIndex(3);

	// Emitter
	emitBuff_.BindComputeCBV(4);

	// Range
	rangeBuff_.BindComputeCBV(5);

	// EmitConfig
	configBuff_.BindComputeCBV(6);

	// RandomSeed
	randSeedBuff_.BindComputeCBV(7);

	// Dispatch
	commands.List->Dispatch(1, 1, 1);

	// Barrierを張る
	particle_->SetUAVBarrier();
}
