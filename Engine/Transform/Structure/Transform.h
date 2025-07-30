#pragma once

#include "Math/MyMath.h"
#include "Math/Struct.h"

#include <vector>



// 定数バッファ構造体
struct TransformationMat {
	Matrix4x4 World;
	Matrix4x4 WVP;
	Matrix4x4 WorldInverseTranspose;
};
// SRT
struct SRTData {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
	SRTData() : scale(1.0f, 1.0f, 1.0f), rotate(0.0f, 0.0f, 0.0f), translate(0.0f, 0.0f, 0.0f) {}
};


/* Transform構造体 */
struct Transform
{
	// コンストラクタ
	Transform() : srt() {};

	// SRT
	SRTData srt{};

	// ローカル -> ワールド変換行列
	Matrix4x4 matWorld{};

	// TransformationMatに設定する変数
	TransformationMat transformationMatData{};

	// ペアレント
	const Transform* parent = nullptr;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 行列の更新処理
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui(std::string label = "", float diff = 0.1f);


#pragma region Get

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	Vector3 GetWorldPos();

#pragma endregion

#pragma region Set

	/// <summary>
	/// 親子関係を結ぶ
	/// </summary>
	void SetParent(const Transform* parentTransform);

#pragma endregion
};
