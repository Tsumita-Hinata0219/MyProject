#include "TestPostEffect.h"



// 初期化処理
void TestPostEffect::Init()
{
	// Absent
	absent_ = std::make_unique<AbsentEffect>();
	absent_->Init();

	// BoxFilter
	boxFilter_ = std::make_unique<BoxFilterEffect>();
	boxFilter_->Init();

	// ColorGrading
	colorGrading_ = std::make_unique<ColorGradingEffect>();
	colorGrading_->Init();

	// Dissolve
	dissolve_ = std::make_unique<DissolveEffect>();
	dissolve_->Init();

	// GaussianFilter
	gaussianFilter_ = std::make_unique<GaussianFilterEffect>();
	gaussianFilter_->Init();

	// Glitch
	glitch_ = std::make_unique<GlitchEffect>();
	glitch_->Init();

	// Grain
	grain_ = std::make_unique<GrainEffect>();
	grain_->Init();

	// HSV
	hsv_ = std::make_unique<HSVEffect>();
	hsv_->Init();

	// LuminanceOutLine
	luminanceOutLine_ = std::make_unique<LuminanceOutLineEffect>();
	luminanceOutLine_->Init();

	// RadialBlur
	radialBlur_ = std::make_unique<RadialBlurEffect>();
	radialBlur_->Init();

	// Random
	random_ = std::make_unique<RandomEffect>();
	random_->Init();

	// RetroCRT
	retroCRT_ = std::make_unique<RetroCRTEffect>();
	retroCRT_->Init();

	// SepiaTone
	sepiaTone_ = std::make_unique<SepiaToneEffect>();
	sepiaTone_->Init();

	// Vignetting
	vignetting_ = std::make_unique<VignettingEffect>();
	vignetting_->Init();


	// IPostEffect::Typeを表す文字
	imguiEffectName_ = {
		"Absent",
		"BoxFilter",
		"ColorGrading",
		"Dissolve",
		"GaussianFilter",
		"Glitch",
		"Grain",
		"GrayScale",
		"HSV",
		"LuminanceOutLine",
		"RadialBlur",
		"Random",
		"RetroCRT",
		"SepiaTone",
		"Vignetting"
	};

	activeEffects_ = EffectType::Absent;
}


// 更新処理
void TestPostEffect::Update()
{
#ifdef _DEBUG

	if (ImGui::TreeNode("TestPostEffect")) {

		ChangeImGui();

		if (activeEffects_ == EffectType::Absent) {
			absent_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::BoxFilter) {
			boxFilter_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::ColorGrading) {
			colorGrading_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::Dissolve) {
			dissolve_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::GauusianFilter) {
			gaussianFilter_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::Glitch) {
			glitch_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::Grain) {
			grain_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::HSV) {
			hsv_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::LuminanceOutLine) {
			luminanceOutLine_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::RadialBlur) {
			radialBlur_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::Random) {
			random_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::RetroCRT) {
			retroCRT_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::SepiaTone) {
			sepiaTone_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::Vignetting) {
			vignetting_->DrawImGui();
		}
		ImGui::Text("");

		ImGui::TreePop();
	}

#endif // _DEBUG
}


// 描画処理
void TestPostEffect::Draw()
{
	if (activeEffects_ == EffectType::Absent) {
		absent_->Draw();
	}
	else if (activeEffects_ == EffectType::BoxFilter) {
		boxFilter_->Draw();
	}
	else if (activeEffects_ == EffectType::ColorGrading) {
		colorGrading_->Draw();
	}
	else if (activeEffects_ == EffectType::Dissolve) {
		dissolve_->Draw();
	}
	else if (activeEffects_ == EffectType::GauusianFilter) {
		gaussianFilter_->Draw();
	}
	else if (activeEffects_ == EffectType::Glitch) {
		glitch_->Draw();
	}
	else if (activeEffects_ == EffectType::Grain) {
		grain_->Draw();
	}
	else if (activeEffects_ == EffectType::HSV) {
		hsv_->Draw();
	}
	else if (activeEffects_ == EffectType::LuminanceOutLine) {
		luminanceOutLine_->Draw();
	}
	else if (activeEffects_ == EffectType::RadialBlur) {
		radialBlur_->Draw();
	}
	else if (activeEffects_ == EffectType::Random) {
		random_->Draw();
	}
	else if (activeEffects_ == EffectType::RetroCRT) {
		retroCRT_->Draw();
	}
	else if (activeEffects_ == EffectType::SepiaTone) {
		sepiaTone_->Draw();
	}
	else if (activeEffects_ == EffectType::Vignetting) {
		vignetting_->Draw();
	}

}


// ImGuiの描画
void TestPostEffect::ChangeImGui()
{
	// std::vector<std::string>をstd::vector<const char*>に変換
	std::vector<const char*> cstrEffectNames;
	for (const auto& name : imguiEffectName_) {
		cstrEffectNames.push_back(name.c_str());
	}

	if (ImGui::Combo("EffectType", &imguiComboIndex_, cstrEffectNames.data(), int(cstrEffectNames.size()))) {

		switch (imguiComboIndex_) {
		case 0: activeEffects_ = EffectType::Absent; break;
		case 1: activeEffects_ = EffectType::BoxFilter; break;
		case 2: activeEffects_ = EffectType::ColorGrading; break;
		case 3: activeEffects_ = EffectType::Dissolve; break;
		case 4: activeEffects_ = EffectType::GauusianFilter; break;
		case 5: activeEffects_ = EffectType::Glitch; break;
		case 6: activeEffects_ = EffectType::Grain; break;
		case 7: activeEffects_ = EffectType::GrayScale; break;
		case 8: activeEffects_ = EffectType::HSV; break;
		case 9: activeEffects_ = EffectType::LuminanceOutLine; break;
		case 10: activeEffects_ = EffectType::RadialBlur; break;
		case 11: activeEffects_ = EffectType::Random; break;
		case 12: activeEffects_ = EffectType::RetroCRT; break;
		case 13: activeEffects_ = EffectType::SepiaTone; break;
		case 14: activeEffects_ = EffectType::Vignetting; break;
		}
	}
}
