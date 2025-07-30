#pragma once


#include "Math/MyMath.h"
#include "Math/Struct.h"


// メッシュデータ
struct MeshData {
	std::map<std::string, JointWeightData> skinClusterData;
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	Node rootNode;
};

// マテリアルデータ
struct MaterialDataN {
	Vector4 color = Vector4::one;
	Matrix4x4 uvTransform = Matrix4x4::identity;
	uint32_t textureHandle = 1u;
};

// ライトデータ
struct DirectionalLightData {
	Vector4 color = Vector4::one; // 色(RGBA)
	Vector3 direction = { 0.0f, 1.0f, 0.0f }; // ライトの向き
	float intensity = 1.0f; // 輝度
	float shininess = 1.0f; // 光沢
	int enable = false; // フラグ

	void DrawImGui(std::string label = "") {
		ImGui::RadioButton((label + "_Disabled").c_str(), &enable, 0); ImGui::SameLine();
		ImGui::RadioButton((label + "_Enabled").c_str(), &enable, 1);
		ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
		ImGui::DragFloat3((label + "_direction").c_str(), &direction.x, 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat((label + "_intennsity").c_str(), &intensity, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat((label + "_shininess").c_str(), &shininess, 0.01f, 0.0f, 1.0f);
	}
};

// 環境マップデータ
struct EnvironmentData {
	float scale = 0.0f;
	int enable = false;
	int textureHandle = 1u;

	void DrawImGui(std::string label = "") {
		ImGui::RadioButton((label + "_Disabled").c_str(), &enable, 0); ImGui::SameLine();
		ImGui::RadioButton((label + "_Enabled").c_str(), &enable, 1);
		ImGui::DragFloat((label + "_Scale").c_str(), &scale, 0.01f, 0.0f, 1.0f);
	}
};

// 色加算
struct ColorAddition {
	Vector4 addColor{};
	float intensity = 0.0f;
	int enable = false;

	void DrawImGui(std::string label = "") {
		ImGui::RadioButton((label + "_Disabled").c_str(), &enable, 0); ImGui::SameLine();
		ImGui::RadioButton((label + "_Enabled").c_str(), &enable, 1);
		ImGui::ColorEdit4((label + "_Color").c_str(), &addColor.x);
		ImGui::DragFloat((label + "_Intensity").c_str(), &intensity, 0.01f, 0.0f, 1.0f);
	}
};

// 法線マップ
struct NormalMap {
	int enable = false;
	int textureHandle = 1u;

	void DrawImGui(std::string label = "") {
		ImGui::RadioButton((label + "_Disabled").c_str(), &enable, 0); ImGui::SameLine();
		ImGui::RadioButton((label + "_Enabled").c_str(), &enable, 1);
	}
};

// デカール
struct DecalSetting {
	Matrix4x4 decalMatrix; // デカールの位置、回転、スケールを決定する行列
	Vector4 decalColor; // デカールの色
	float decalStrength; // デカールの適用強度
	int enable;
	SRTData srt{};
	int textureHandle = 1u;

	void DrawImGui(std::string label = "") {
		ImGui::RadioButton((label + "_Disabled").c_str(), &enable, 0); ImGui::SameLine();
		ImGui::RadioButton((label + "_Enabled").c_str(), &enable, 1);
	}

	void CalcMatrix() {
		// デカールのワールド行列を計算
		Matrix4x4 decalWorldMatrix = MakeAffineMatrix(srt.scale, srt.rotate, srt.translate);
		// ワールド座標をデカール空間に変換する行列 (デカールのワールド行列の逆行列) を設定します。
		decalMatrix = Inverse(decalWorldMatrix);
		// デカールを有効にする
		enable = 1;
	}
};

// スキンクラスターデータ
struct SkinClusterData {
	std::vector<Matrix4x4> inverseBindPosematrices;
	//Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;
	//D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;
	//Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPallette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE>paletteSrvHandle;
	uint32_t srvHandle;
};