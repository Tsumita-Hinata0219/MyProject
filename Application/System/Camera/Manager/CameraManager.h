#pragma once

#include "GameObject/GameObject.h"
#include "../Resources/CameraResource.h"

/* CameraManagerクラス */
class CameraManager {

private: // シングルトンデザインパターン

	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(const CameraManager&) = delete;
	const CameraManager& operator=(const CameraManager&) = delete;

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static CameraManager* GetInstance() {
		static CameraManager instance;
		return &instance;
	}

	/// <summary>
	/// データをセットする
	/// </summary>
	void ReSet();

	/// <summary>
	/// カメラデータのバインド
	/// </summary>
	void Bind_CameraData(UINT num);


#pragma region Accessor

	// カメラデータの Ptr を取得
	std::weak_ptr<CameraData> GetCameraDataWeak() { return camera_; }

#pragma endregion


private:

	// カメラデータ本体
	std::shared_ptr<CameraData> camera_;

};

