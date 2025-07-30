#include "ParticleFadeOut.h"



/// <summary>
/// 生成
/// </summary>
void GpuParticle::Material::ParticleFadeOut::Create(std::weak_ptr<GPUParticle> pParticle)
{
	// パーティクルのptrを受け取る
	particlePtr_ = pParticle;

	// Emitterデータ作成
	data_ = std::make_shared<GpuParticle::Material::FadeOut>();
	// Emitterデータのバッファー作成
	buffer_.CreateCBV();
}


/// <summary>
/// 更新処理
/// </summary>
void GpuParticle::Material::ParticleFadeOut::Update()
{
	Bind_and_Dispatch();
}


/// <summary>
/// LuaScriptからFadeOutデータの読み込み
/// </summary>
void GpuParticle::Material::ParticleFadeOut::Load_Data_From_Lua(const LuaScript& lua)
{
	GpuParticle::Material::FadeOut result{};

	result.start = lua.GetVariable<float>("FadeOut.fadeStart");
	result.end = lua.GetVariable<float>("FadeOut.fadeEnd");
	result.power = lua.GetVariable<float>("FadeOut.fadePower");

	*data_ = result;
}


/// <summary>
/// データ書き込み
/// </summary>
void GpuParticle::Material::ParticleFadeOut::WriteData()
{
	// FadeOutData
	buffer_.UpdateData(data_.get());
}


/// <summary>
/// バインド & ディスパッチ
/// </summary>
void GpuParticle::Material::ParticleFadeOut::Bind_and_Dispatch()
{
	// データの書き込み
	WriteData();

	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::SetPipeLine(PipeLine::Container::Compute, PipeLine::Category::GPUParticle_FadeOut);

	// Particle
	particlePtr_.lock()->Bind_ParticleProp(0);

	// LifeTime
	particlePtr_.lock()->Bind_ParticleLifeTime(1);

	// FadeOut
	buffer_.BindComputeCBV(2);

	// Dispach
	commands.List->Dispatch(1, 1, 1);

	// Barrierを張る
	particlePtr_.lock()->SetUAVBarrier();
}
