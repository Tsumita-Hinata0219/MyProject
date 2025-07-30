#pragma once

#include "Math/MyMath.h"
#include "Resource/BufferResource/BufferResource.h"


// 前方宣言
class CameraManager;

// 行列構造体
struct ObjectMatrices {
	Matrix4x4 World;
	Matrix4x4 WVP;
	Matrix4x4 WorldInverseTranspose;
};
// Scale Rotate Translate
struct SRT {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
	SRT() :
		scale(1.0f, 1.0f, 1.0f),
		rotate(0.0f, 0.0f, 0.0f),
		translate(0.0f, 0.0f, 0.0f) {
	}
};


/* シーン上にあるActorの座標関係 */
class TransformNode {

public:

	SRT srt{};

private:

	// 名前
	std::string name_;

	// バッファー
	ObjectMatrices objMatrices_;
	BufferResource<ObjectMatrices> buffer_;

	// 親子関係
	std::weak_ptr<TransformNode> parent_;
	std::vector<std::weak_ptr<TransformNode>> children_;

	// カメラ管理クラスのポインタ
	CameraManager* cameraManager_;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TransformNode(const std::string& name = "default");

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TransformNode() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	Vector3 GetWorldPos();

	/// <summary>
	/// 親の設定
	/// </summary>
	void AttachToParent(std::weak_ptr<TransformNode> parent);
	void DetachFromParent();

	/// <summary>
	/// データをバインドする
	/// </summary>
	void Bind_CBV(UINT num);

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui(std::string label = "");
};