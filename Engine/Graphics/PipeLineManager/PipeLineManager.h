#pragma once


#include <d3dx12.h>
#include <dxcapi.h>

#include "Math/MyMath.h"
#include "Math/Struct.h"
#include "Core/DirectXManager/DirectXManager.h"
#include "Graphics/ShaderManager/ShaderManager.h"
#include "Graphics/CommandManager/CommandManager.h"

#include "IPipeLineState/IPipeLineState.h"
#include "IPipeLineState/PipeLineStructure.h"

#include "ModelPipeLine/Object2DPipeLine/Object2DPipeLine.h"
#include "ModelPipeLine/Object3DPipeLine/Object3DPipeLine.h"
#include "ModelPipeLine/SkinningObject3dPipeLine/SkinningObject3dPipeLine.h"
#include "ModelPipeLine/SkyboxPipeLine/SkyboxPipeLine.h"
#include "ModelPipeLine/CPUParticlePipeLine/CPUParticlePipeLine.h"
#include "ModelPipeLine/GPUParticle_Draw/GPUParticle_Draw_PipeLine.h"
#include "ModelPipeLine/Decal/DecalPipeLine.h"

#include "PostEffectPipeLine/AbsentPipeLine/AbsentPipeLine.h"
#include "PostEffectPipeLine/BoxFilterPipeLine/BoxFilterPipeLine.h"
#include "PostEffectPipeLine/DissolvePipeLine/DissolvePipeLine.h"
#include "PostEffectPipeLine/GaussianFilterPipeLine/GaussianFilterPipeLine.h"
#include "PostEffectPipeLine/GlitchPipeLine/GlitchPipeLine.h"
#include "PostEffectPipeLine/GrayScalePipeLine/GrayScalePipeLine.h"
#include "PostEffectPipeLine/HSVPipeLine/HSVPipeLine.h"
#include "PostEffectPipeLine/OutLinePipeLine/OutLinePipeLine.h"
#include "PostEffectPipeLine/RadialBlurPipeLine/RadialBlurPipeLine.h"
#include "PostEffectPipeLine/RandomPipeLine/RandomPipeLine.h"
#include "PostEffectPipeLine/RetroCRTPipeLine/RetroCRTPipeLine.h"
#include "PostEffectPipeLine/SepiaTonePipeLine/SepiaTonePipeLine.h"
#include "PostEffectPipeLine/VignettingPipeLine/VignettingPipeLine.h"
#include "PostEffectPipeLine/GrainPipeLine/GrainPipeLine.h"

#include "CSPipeLine/Emitter/Sphere/SphereEmitterPipeLine.h"
#include "CSPipeLine/Field/Constant/ConstantFieldPipeLine.h"
#include "CSPipeLine/GPUParticle/Init/GPUParticle_Init_PipeLine.h"
#include "CSPipeLine/GPUParticle/Update/GPUParticle_Update_PipeLine.h"
#include "CSPipeLine/GPUParticle/FadeOut/GPUParticle_FadeOut_PipeLine.h"


using namespace PipeLine;


/* PipeLineManagerクラス */
class PipeLineManager {

public: // メンバ関数

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static PipeLineManager* GetInstance() {
		static PipeLineManager instance;
		return &instance;
	}

	/// <summary>
	/// PipeLineを作成する
	/// </summary>
	static void CreatePipeLine();

	/// <summary>
	/// PipeLineの設定
	/// </summary>
	static void SetPipeLine(Container cantainer = Container::None, Category category = Category::None, SubFilter subFilter = SubFilter::None);

	/// <summary>
	/// PipeLineTypeのリセット
	/// </summary>
	static void PipeLineReset() {
		preCategory_ = Category::None;
		preSubFilter_ = SubFilter::None;
	}


private:

	/// <summary>
	/// オブジェクト関連
	/// </summary>
	void CreatePipeLine_Object3d();
	void CreatePipeLine_Object2d();
	void CreatePipeLine_SkinningObject3D();
	void CreatePipeLine_Skybox();
	void CreatePipeLine_CPUParticle();
	void CreatePipeLine_GPUParticle_Draw();
	void CreatePipeLine_Decal();

	/// <summary>
	/// ポストエフェクト
	/// </summary>
	void CreatePipeLine_Absent();
	void CreatePipeLine_BoxFilter();
	void CreatePipeLine_Dissolve();
	void CreatePipeLine_GaussianFilter();
	void CreatePipeLine_Glitch();
	void CreatePipeLine_Grain();
	void CreatePipeLine_GrayScale();
	void CreatePipeLine_HSV();
	void CreatePipeLine_OutLine();
	void CreatePipeLine_RadialBlur();
	void CreatePipeLine_Random();
	void CreatePipeLine_RetroCRT();
	void CreatePipeLine_SepiaTone();
	void CreatePipeLine_Vignetting();

	/// <summary>
	/// ComputeShader
	/// </summary>
	void CreatePipeLine_SphereEmitter();
	void CreatePipeLine_ConstantField();
	void CreatePipeLine_GPUParticle_Init();
	void CreatePipeLine_GPUParticle_Update();
	void CreatePipeLine_GPUParticle_FadeOut();


private: // メンバ変数
	
	// PipeLineの情報を格納するコンテナ
	std::map<Category, std::map<SubFilter, PsoProperty>> pipeLineMaps_;

	// それぞれの区分
	static Category preCategory_;
	static SubFilter preSubFilter_;

};

