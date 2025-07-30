#pragma once

#include "Base/WinApp/WinApp.h"
#include "BufferResource/BufferResource.h"
#include "Math/MyMath.h"
#include "Transform/Transform.h"


/* カメラの基底クラス */
class ICamera {

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ICamera() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;


private:



	
};