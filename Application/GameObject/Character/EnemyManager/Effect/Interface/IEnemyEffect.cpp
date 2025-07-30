#include "IEnemyEffect.h"
#include "Graphics/3D/Model/Manager/ModelManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
IEnemyEffect::IEnemyEffect()
{
	// ModelManagerのインスタンスを取得
	modelManager_ = ModelManager::GetInstance();
}
