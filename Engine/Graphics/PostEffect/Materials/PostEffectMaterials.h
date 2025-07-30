#pragma once

#include <sstream>

#include "Platform/WinApp/WinApp.h"
#include "Resource/BufferResource/BufferResource.h"

#include "Math/MyMath.h"
#include "Math/Struct.h"

/* AbsentMtl */
struct AbsentMtl{};

/* BoxFilter用 */
struct BoxFilterMtl {
	Vector4 color{};

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
	}
};

/* ColorGrading用 */
struct ColorGradingMtl {
	Vector4 color{};

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
	}
};

/* Dissolve用 */
struct DissolveMtl {
	Vector4 color = Vector4::one;
	float threshold = 0.0f;
	float thinkness = 0.0f;
	uint32_t maskTexture = 0;

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
		ImGui::DragFloat((label + "_threshold").c_str(), &threshold, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat((label + "_thinkness").c_str(), &thinkness, 0.01f, 0.0f, 1.0f);
	}
};

/* GaussianFilter用 */
struct GaussianFilterMtl {
	Vector4 color{};

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
	}
};

/* Glitch用 */
struct GlitchMtl {
	Vector4 color{};
	Vector2 noiseScale = Vector2::one; // ノイズのスケール（X, Y）
	float noiseSpeed; // ノイズの動く速さ
	float noiseFrequency; // ノイズの動きの頻度
	Vector2 maxDisplacement; // 最大置き換え（x: 水平, y: 垂直）
	float time; // 現在の時間
	uint32_t maskTexture = 0;

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
		ImGui::Text("Noise");
		ImGui::DragFloat2((label + "_noiseScale").c_str(), &noiseScale.x, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat((label + "_noiseSpeed").c_str(), &noiseSpeed, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat((label + "_noiseFrequency").c_str(), &noiseFrequency, 0.01f, 0.0f, 1.0f);
		ImGui::Text("Map");
		ImGui::DragFloat2((label + "_maxDisplacement").c_str(), &maxDisplacement.x, 0.01f, 0.0f, 10.0f);
		ImGui::Text("");
		ImGui::DragFloat((label + "_time").c_str(), &time, 0.0f);
	}
};

/* Grain用 */
struct GrainMtl {
	Vector4 color{};
	float grainAmount = 0.0f;
	Vector2 noiseFactors = Vector2::zero;

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
		ImGui::DragFloat((label + "_grainAmount").c_str(), &grainAmount, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat2((label + "_noiseFactors").c_str(), &noiseFactors.x, 0.01f, 0.0f, 1.0f);
	}
};

/* GrayScale用 */
struct GrayScaleMtl {
	Vector4 color{};

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
	}
};

/* HSV用 */
struct HSVMtl {
	Vector4 color{};
	float hue = 0.0f;
	float saturation = 0.0f;
	float value = 0.0f;

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
		ImGui::DragFloat((label + "_hue").c_str(), &hue, 0.01f);
		ImGui::DragFloat((label + "_saturation").c_str(), &saturation, 0.01f);
		ImGui::DragFloat((label + "_value").c_str(), &value, 0.01f);
	}
};

/* OutLine用 */
struct LuminanceOutLineMtl {
	Vector4 color{};

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
	}
};
struct DepthOutLine {
	Vector4 color{};

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
	}
};

/* RadialBlur用 */
struct RadialBlurMtl {
	Vector4 color{};

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
	}
};

/* Random用 */
struct RandomMtl {
	Vector4 color{};
	float time = 0.0f;

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
		ImGui::DragFloat((label + "_time").c_str(), &time, 0.01f);
	}
};

/* RetroCRT用 */
struct RetroCRTMtl {
	Vector4 color{};            // エッジカラー
	float scanlineStrength = 0.0f;  // スキャンラインの強度
	int scanlineActive = false;     // スキャンラインの有効フラグ
	Vector2 chromaOffsetR{};        // Rチャンネルの色収差オフセット (x, y) 
	Vector2 chromaOffsetB{};        // Bチャンネルの色収差オフセット (x, y) 
	int chromaActive = false;       // 色収差の有効フラグ
	float barrelDistortion = 0.0f;  // バレル歪みの強度
	int barrelActive = false;       // バレル歪みの有効フラグ
	float noiseStrength = 0.0f;     // ノイズの強度
	int noiseActive = false;        // ノイズの有効フラグ
	float bloomStrength = 0.0f;     // ブルームの強度
	int bloomActive = false;        // ブルームの有効フラグ
	Vector2 resolution = WinApp::WindowSize(); // ウィンドウサイズ
	float time = 0.0f;             // ゲーム開始時からのタイマー

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
		ImGui::DragFloat((label + "_scanlineStrength").c_str(), &scanlineStrength, 0.01f, 0.0f, 1.0f);
		ImGui::RadioButton((label + "_scanlineDisabled").c_str(), &scanlineActive, 0); ImGui::SameLine();
		ImGui::RadioButton((label + "_scanlineEnabled").c_str(), &scanlineActive, 1);
		ImGui::DragFloat2((label + "_chromaOffset_R").c_str(), &chromaOffsetR.x, 0.0001f, -1.0f, 1.0f);
		ImGui::DragFloat2((label + "_chromaOffset_B").c_str(), &chromaOffsetB.x, 0.0001f, -1.0f, 1.0f);
		ImGui::RadioButton((label + "_chromaDisabled").c_str(), &chromaActive, 0); ImGui::SameLine();
		ImGui::RadioButton((label + "_chromaEnabled").c_str(), &chromaActive, 1);
		ImGui::DragFloat((label + "_barrelDistortion").c_str(), &barrelDistortion, 0.01f, 0.0f, 1.0f);
		ImGui::RadioButton((label + "_barrelDisabled").c_str(), &barrelActive, 0); ImGui::SameLine();
		ImGui::RadioButton((label + "_barrelEnabled").c_str(), &barrelActive, 1);
		ImGui::DragFloat((label + "_noiseStrength").c_str(), &noiseStrength, 0.01f, 0.0f, 1.0f);
		ImGui::RadioButton((label + "_noiseDisabled").c_str(), &noiseActive, 0); ImGui::SameLine();
		ImGui::RadioButton((label + "_noiseEnabled").c_str(), &noiseActive, 1);
		ImGui::DragFloat((label + "_bloomStrength").c_str(), &bloomStrength, 0.01f, 0.0f, 1.0f);
		ImGui::RadioButton((label + "_bloomDisabled").c_str(), &bloomActive, 0); ImGui::SameLine();
		ImGui::RadioButton((label + "_bloomEnabled").c_str(), &bloomActive, 1);
		ImGui::DragFloat2((label + "_resolution").c_str(), &resolution.x, 0.0f);
		ImGui::DragFloat((label + "_time").c_str(), &time, 0.0f);
	}
};

/* SepiaTone用 */
struct SepiaToneMtl {
	Vector4 color{};

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
	}
};

/* Vignetting用 */
struct VignettingMtl {
	Vector4 color = Vector4::one;
	float intensity = 0;
	float center = 0.5f;
	Vector2 radius = Vector2::one / 2.0f;
	float smoothness = 0.01f;
	float roundness = 1.0f;

	void DrawImGui(std::string label = "") {
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
		ImGui::DragFloat((label + "_intensity").c_str(), &intensity);
		ImGui::DragFloat((label + "_center").c_str(), &center);
		ImGui::DragFloat2((label + "_radius").c_str(), &radius.x);
		ImGui::DragFloat((label + "_smoothness").c_str(), &smoothness);
		ImGui::DragFloat((label + "_roundness").c_str(), &roundness);
	}
};
