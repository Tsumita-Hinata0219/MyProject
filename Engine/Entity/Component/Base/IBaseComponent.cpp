#include "IBaseComponent.h"



/// <summary>
/// コンストラクタ
/// </summary>
IBaseComponent::IBaseComponent()
{
	// 名前はdefaultで初期化
	name_ = "default";
}


/// <summary>
/// 生成
/// </summary>
void IBaseComponent::Create(const std::string& name)
{
	name_ = name;
}
