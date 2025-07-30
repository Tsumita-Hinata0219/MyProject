#include "PipeLineManager.h"

using namespace PipeLine;

Category PipeLineManager::preCategory_ = Category::None;
SubFilter PipeLineManager::preSubFilter_ = SubFilter::None;


/// <summary>
/// PipeLineを作成する
/// </summary>
void PipeLineManager::CreatePipeLine()
{
	// インスタンスの取得
	PipeLineManager* instance = PipeLineManager::GetInstance();

	// オブジェクト関連
	instance->CreatePipeLine_Object3d();
	instance->CreatePipeLine_Object2d();
	instance->CreatePipeLine_SkinningObject3D();
	instance->CreatePipeLine_Skybox();
	instance->CreatePipeLine_CPUParticle();
	instance->CreatePipeLine_GPUParticle_Draw();
	instance->CreatePipeLine_Decal();

	// ポストエフェクト
	instance->CreatePipeLine_Absent();
	instance->CreatePipeLine_BoxFilter();
	instance->CreatePipeLine_Dissolve();
	instance->CreatePipeLine_GaussianFilter();
	instance->CreatePipeLine_Glitch();
	instance->CreatePipeLine_Grain();
	instance->CreatePipeLine_GrayScale();
	instance->CreatePipeLine_HSV();
	instance->CreatePipeLine_OutLine();
	instance->CreatePipeLine_RadialBlur();
	instance->CreatePipeLine_Random();
	instance->CreatePipeLine_RetroCRT();
	instance->CreatePipeLine_SepiaTone();
	instance->CreatePipeLine_Vignetting();

	// ComputeShader
	instance->CreatePipeLine_SphereEmitter();
	instance->CreatePipeLine_ConstantField();
	instance->CreatePipeLine_GPUParticle_Init();
	instance->CreatePipeLine_GPUParticle_Update();
	instance->CreatePipeLine_GPUParticle_FadeOut();
}


/// <summary>
/// PipeLineの設定
/// </summary>
void PipeLineManager::SetPipeLine(Container cantainer, Category category, SubFilter subFilter)
{
	// カテゴリーもしくはサブフィルターが異なればコマンドを積みなおす
	if (preCategory_ != category || preSubFilter_ != subFilter) {

		// Commandの取得
		Commands commands = CommandManager::GetInstance()->GetCommands();
		// インスタンスの取得
		PipeLineManager* instance = PipeLineManager::GetInstance();

		// GraphicとComputeを分ける
		if (cantainer == Container::Graphic) {

			// 引数にあったPipeLineを積みなおす
			commands.List->SetGraphicsRootSignature(instance->pipeLineMaps_[category][subFilter].rootSignature);
			commands.List->SetPipelineState(instance->pipeLineMaps_[category][subFilter].graphicsPipelineState);
			// 形状を設定。基本PipeLineで設定したものと同じもの
			commands.List->IASetPrimitiveTopology(instance->pipeLineMaps_[category][subFilter].primitiveTopologyType);
		}
		else if (cantainer == Container::Compute) {

			// 引数にあったPipeLineを積みなおす
			commands.List->SetComputeRootSignature(instance->pipeLineMaps_[category][subFilter].rootSignature);
			commands.List->SetPipelineState(instance->pipeLineMaps_[category][subFilter].graphicsPipelineState);
		}

		// カテゴリーとサブフィルターを再設定
		instance->preCategory_ = category;
		instance->preSubFilter_ = subFilter;
	}
}


/// <summary>
/// オブジェクト関連
/// </summary>
void PipeLineManager::CreatePipeLine_Object3d()
{
	std::unique_ptr<Object3DPipeLine> pipeline = std::make_unique<Object3DPipeLine>();

	// 背面カリング
	pipeLineMaps_[Category::Object3D][SubFilter::Cull_Mode_Back] = pipeline->SetUpPso();

	// 背面描画
	pipeline->SetCullMode(D3D12_CULL_MODE_NONE); // CullModeを背面描画するように設定
	pipeLineMaps_[Category::Object3D][SubFilter::Cull_Mode_None] = pipeline->SetUpPso();

	// ワイヤーフレーム
	pipeline->SetCullMode(D3D12_CULL_MODE_BACK); // 背面カリングに戻しておく
	pipeline->SetFillMode(D3D12_FILL_MODE_WIREFRAME); // ワイヤーフレーム
	pipeLineMaps_[Category::Object3D][SubFilter::Fill_Mode_Wireframe] = pipeline->SetUpPso();

	// Depth Zero
	pipeline->SetFillMode(D3D12_FILL_MODE_SOLID); // 塗りつぶし
	pipeline->SetUpDepthWriteMask(D3D12_DEPTH_WRITE_MASK_ZERO); // DepthWriteMaskをZeroに設定
	pipeLineMaps_[Category::Object3D][SubFilter::DepthWriteMask_Zero] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_Object2d()
{
	std::unique_ptr<Object2DPipeLine> pipeline = std::make_unique<Object2DPipeLine>();
	pipeLineMaps_[Category::Object2D][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_SkinningObject3D()
{
	std::unique_ptr<SkinningObject3dPipeLine> pipeline = std::make_unique<SkinningObject3dPipeLine>();
	pipeLineMaps_[Category::SkinningObject3D][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_Skybox()
{
	std::unique_ptr<SkyboxPipeLine> pipeline = std::make_unique<SkyboxPipeLine>();
	pipeLineMaps_[Category::Skybox][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_CPUParticle()
{
	/*std::unique_ptr<CPUParticlePipeLine> pipeline = std::make_unique<CPUParticlePipeLine>();
	pipeLineMaps_[Category::CPUParticle][SubFilter::None] = pipeline->SetUpPso();*/
}
void PipeLineManager::CreatePipeLine_GPUParticle_Draw()
{
	// 背面カリング
	std::unique_ptr<GPUParticle_Draw_PipeLine> pipeline = std::make_unique<GPUParticle_Draw_PipeLine>();
	pipeLineMaps_[Category::GPUParticle_Draw][SubFilter::None] = pipeline->SetUpPso();

	// Depth Zero
	pipeline->SetUpDepthWriteMask(D3D12_DEPTH_WRITE_MASK_ZERO); // DepthWriteMaskをZeroに設定
	pipeLineMaps_[Category::GPUParticle_Draw][SubFilter::DepthWriteMask_Zero] = pipeline->SetUpPso();
}

void PipeLineManager::CreatePipeLine_Decal()
{
	std::unique_ptr<DecalPipeLine> pipeline = std::make_unique<DecalPipeLine>();
	pipeLineMaps_[Category::Decal][SubFilter::None] = pipeline->SetUpPso();
}


/// <summary>
/// ポストエフェクト
/// </summary>
void PipeLineManager::CreatePipeLine_Absent()
{
	std::unique_ptr<AbsentPipeLine> pipeline = std::make_unique<AbsentPipeLine>();
	pipeLineMaps_[Category::Absent][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_BoxFilter()
{
	std::unique_ptr<BoxFilterPipeLine> pipeline = std::make_unique<BoxFilterPipeLine>();
	pipeLineMaps_[Category::BoxFilter][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_Dissolve()
{
	std::unique_ptr<DissolvePipeLine> pipeline = std::make_unique<DissolvePipeLine>();
	pipeLineMaps_[Category::Dissolve][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_GaussianFilter()
{
	std::unique_ptr<GaussianFilterPipeLine> pipeline = std::make_unique<GaussianFilterPipeLine>();
	pipeLineMaps_[Category::GaussianFilter][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_Glitch()
{
	std::unique_ptr<GlitchPipeLine> pipeline = std::make_unique<GlitchPipeLine>();
	pipeLineMaps_[Category::Glitch][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_Grain()
{
	std::unique_ptr<GrainPipeLine> pipeline = std::make_unique<GrainPipeLine>();
	pipeLineMaps_[Category::Grain][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_GrayScale()
{
	std::unique_ptr<GrayScalePipeLine> pipeline = std::make_unique<GrayScalePipeLine>();
	pipeLineMaps_[Category::GrayScale][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_HSV()
{
	std::unique_ptr<HSVPipeLine> pipeline = std::make_unique<HSVPipeLine>();
	pipeLineMaps_[Category::HSV][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_OutLine()
{
	std::unique_ptr<OutLinePipeLine> pipeline = std::make_unique<OutLinePipeLine>();
	pipeLineMaps_[Category::OutLine][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_RadialBlur()
{
	std::unique_ptr<RadialBlurPipeLine> pipeline = std::make_unique<RadialBlurPipeLine>();
	pipeLineMaps_[Category::RadialBlur][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_Random()
{
	std::unique_ptr<RandomPipeLine> pipeline = std::make_unique<RandomPipeLine>();
	pipeLineMaps_[Category::Random][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_RetroCRT()
{
	std::unique_ptr<RetroCRTPipeLine> pipeline = std::make_unique<RetroCRTPipeLine>();
	pipeLineMaps_[Category::RetroCRT][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_SepiaTone()
{
	std::unique_ptr<SepiaTonePipeLine> pipeline = std::make_unique<SepiaTonePipeLine>();
	pipeLineMaps_[Category::SepiaTone][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_Vignetting()
{
	std::unique_ptr<VignettingPipeLine> pipeline = std::make_unique<VignettingPipeLine>();
	pipeLineMaps_[Category::Vignetting][SubFilter::None] = pipeline->SetUpPso();
}


/// <summary>
/// ComputeShader
/// </summary>
void PipeLineManager::CreatePipeLine_SphereEmitter()
{
	std::unique_ptr<SphereEmitterPipeLine> pipeline = std::make_unique<SphereEmitterPipeLine>();
	pipeLineMaps_[Category::SphereEmitter][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_ConstantField()
{
	std::unique_ptr<ConstantFieldPipeLine> pipeline = std::make_unique<ConstantFieldPipeLine>();
	pipeLineMaps_[Category::ConstantField][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_GPUParticle_Init()
{
	std::unique_ptr<GPUParticle_Init_PipeLine> pipeline = std::make_unique<GPUParticle_Init_PipeLine>();
	pipeLineMaps_[Category::GPUParticle_Init][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_GPUParticle_Update()
{
	std::unique_ptr<GPUParticle_Update_PipeLine> pipeline = std::make_unique<GPUParticle_Update_PipeLine>();
	pipeLineMaps_[Category::GPUParticle_Update][SubFilter::None] = pipeline->SetUpPso();
}
void PipeLineManager::CreatePipeLine_GPUParticle_FadeOut()
{
	std::unique_ptr<GPUParticle_FadeOut_PipeLine> pipeline = std::make_unique<GPUParticle_FadeOut_PipeLine>();
	pipeLineMaps_[Category::GPUParticle_FadeOut][SubFilter::None] = pipeline->SetUpPso();
}
