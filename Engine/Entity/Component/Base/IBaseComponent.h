#pragma once

#include "Math/MyMath.h"
#include "../../Actor/IActor.h"


/* 機能を追加するためのコンポーネント */
class IBaseComponent {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IBaseComponent();

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IBaseComponent() = default;

	/// <summary>
	/// 生成
	/// </summary>
	void Create(const std::string& name);

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() {};

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(float deltaTime) { deltaTime; };


#pragma region Accessor 

	// 名前
	std::string Get_Name() const { return name_; }

	// Owner
	void SetOwner(std::weak_ptr<IActor> setOwner) { owner_ = setOwner; }

#pragma endregion 


protected:

	// Componentを持つ親Actor
	std::weak_ptr<IActor> owner_;

	// Componentの名前
	std::string name_;

};
