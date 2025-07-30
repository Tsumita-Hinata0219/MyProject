#include "EnemyShotPatternForward.h"
#include "../../EnemyBulletContainer.h"

void EnemyShotPatternForward::Execute()
{
	owner_->AddBulletInstance(Temp::Direction::Front);
}
