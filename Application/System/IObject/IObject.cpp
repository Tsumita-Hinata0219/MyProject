#include "IObject.h"
#include "Physics/CollisionSystem/Collider/Interface/ICollider.h"


/// <summary>
/// コンストラクタ
/// </summary>
IObject::IObject()
{
	modelManager_ = ModelManager::GetInstance();
}

