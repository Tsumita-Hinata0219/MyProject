#include "TransformNode.h"
#include "System/Camera/Manager/CameraManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
TransformNode::TransformNode(const std::string& name)
{
	// 名前に適当に入れておく
	name_ = name;

	// 行列構造体の初期化
	objMatrices_.World = Matrix4x4::identity;
	objMatrices_.WVP = Matrix4x4::identity;
	objMatrices_.WorldInverseTranspose = Matrix4x4::identity;

	// バッファー作成
	buffer_.CreateCBV();

	// カメラ管理クラスのインスタンスの取得
	cameraManager_ = CameraManager::GetInstance();
}


/// <summary>
/// 更新処理
/// </summary>
void TransformNode::Update()
{
	// World行列の計算
	objMatrices_.World = MakeAffineMatrix(srt.scale, srt.rotate, srt.translate);

	// 親があれば親のワールド行列を掛ける
	if (parent_.lock() != nullptr) {
		objMatrices_.World *= parent_.lock()->objMatrices_.World;
	}

	// WVP行列の計算
	objMatrices_.WVP =
		objMatrices_.World *
		cameraManager_->GetCameraDataWeak().lock()->viewMatrix *
		cameraManager_->GetCameraDataWeak().lock()->projectionMatrix;

	// WorldInverseTranspose行列の計算
	objMatrices_.WorldInverseTranspose = Transpose(Inverse(objMatrices_.World));
}


/// <summary>
/// ワールド座標の取得
/// </summary>
Vector3 TransformNode::GetWorldPos()
{
	Update();
	return { objMatrices_.World.m[3][0], objMatrices_.World.m[3][1], objMatrices_.World.m[3][2] };
}


/// <summary>
/// 親子関係の設定
/// </summary>
void TransformNode::AttachToParent(std::weak_ptr<TransformNode> parent)
{
	parent_ = parent;
}
void TransformNode::DetachFromParent()
{
	parent_.reset();
}


/// <summary>
/// データをバインドする
/// </summary>
void TransformNode::Bind_CBV(UINT num)
{
	// データ書き込み
	buffer_.UpdateData(&objMatrices_);
	// バインド
	buffer_.BindGraphicsCBV(num);
}


/// <summary>
/// ImGuiの描画
/// </summary>
void TransformNode::DrawImGui(std::string label)
{
	ImGui::DragFloat3((label + "_Scale").c_str(), &srt.scale.x, 0.1f);
	ImGui::DragFloat3((label + "_Rotate").c_str(), &srt.rotate.x, 0.001f);
	ImGui::DragFloat3((label + "_Translate").c_str(), &srt.translate.x, 0.1f);
}
