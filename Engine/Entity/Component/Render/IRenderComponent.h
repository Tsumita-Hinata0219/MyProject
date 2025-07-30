#pragma once

#include "Entity/Actor/IActor.h"
#include "Graphics/3D/Model/Model.h"
#include "Graphics/RenderSystem/State/RenderState .h"


/* 描画用のコンポーネント */
class IRenderComponent {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IRenderComponent() = default;

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IRenderComponent() = default;

	/// <summary>
	/// 生成
	/// </summary>
	void Create(const std::string& renderDataName = "Test");

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


#pragma region Accessor 

	void SetOwner(std::weak_ptr<IActor> owner) 
	{ 
		owner_ = owner;
	}

#pragma endregion 


protected:

	// Componentを持つ親Actor 
	std::weak_ptr<IActor> owner_;

	// レンダーステート
	std::unique_ptr<RenderSystem::RenderState> renderState_;

};