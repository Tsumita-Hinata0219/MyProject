#pragma once

#include "Platform/WinApp/WinApp.h"
#include "Resource/BufferResource/BufferResource.h"
#include "Transform/Structure/Transform.h"
#include "Math/MyMath.h"


// GPUに送る行列
struct TransformationViewMatrix {
	Matrix4x4 view;
	Matrix4x4 viewProjection;
	Matrix4x4 orthoGraphic;
	Vector3 cameraPosition;
};


/* -----------------------------------
		CameraResource構造体
		カメラに必要な値をまとめた構造体
--------------------------------------*/
struct CameraData {

public:

	// 初期化処理
	void Init(Vector3 initRotate = { 0.0f, 0.0f, 0.0f }, Vector3 initTranslate = { 0.0f, 0.0f, 0.0f });

	// 行列の更新
	void Update();

	// バッファーの更新
	void UpdateBuffer();

	// カメラデータをバンドする
	void Bind_CameraData(UINT num);

	// ImGuiの描画
	void DrawImGui();

#pragma region Accessor アクセッサ

	// ワールド座標の取得
	Vector3 GetWorldPos();

#pragma endregion 


public:

#pragma region ビュー行列の設定

	SRTData srt{};

	// バッファーに書き込むデータ
	TransformationViewMatrix* bufferData = nullptr;

	// バッファー
	std::unique_ptr<BufferResource<TransformationViewMatrix>> buffer;

#pragma endregion 

#pragma region 射影行列用変数

	// 垂直方向視野角
	float fov = 0.45f;

	// ビューポートのアスペクト比
	float aspectRatio = float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight);

	// 深度限界 (手前側)
	float nearZ = 0.1f;

	// 深度限界 (奥側)
	float farZ = 1000.0f;

#pragma endregion

#pragma region 行列

	// 回転行列
	Matrix4x4 rotateMat{};

	// 平行移動行列
	Matrix4x4 translateMat{};

	// アフィン行列
	Matrix4x4 matWorld{};

	// ビュー行列
	Matrix4x4 viewMatrix{};

	// 投影行列
	Matrix4x4 projectionMatrix{};

	// 投影逆行列
	Matrix4x4 projectionInverseMatrix{};

	// 正射影行列
	Matrix4x4 orthoGraphicMatrix{};

	// ビューポート行列
	Matrix4x4 viewportMatrix{};

	// ビュープロジェクション行列
	Matrix4x4 viewProjectionMatrix{};

	// ビュープロジェクションビューポート合成行列
	Matrix4x4 viewProjectionViewportMatrix{};

	// 合成行列の逆行列
	Matrix4x4 inverseViewProjectionViewportMatrix{};

#pragma endregion

};

