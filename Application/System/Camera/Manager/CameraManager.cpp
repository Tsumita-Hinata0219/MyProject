#include "CameraManager.h"



/// <summary>
/// データをセットする
/// </summary>
void CameraManager::ReSet()
{
	// カメラデータを完全にリセット（再生成）
	camera_ = std::make_shared<CameraData>();
	camera_->Init();
}


/// <summary>
/// カメラデータのバインド
/// </summary>
void CameraManager::Bind_CameraData(UINT num)
{
	camera_->Bind_CameraData(num);
}
