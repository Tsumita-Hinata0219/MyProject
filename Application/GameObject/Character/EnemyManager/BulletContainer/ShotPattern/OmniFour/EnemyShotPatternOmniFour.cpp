#include "EnemyShotPatternOmniFour.h"
#include "../../EnemyBulletContainer.h"

void EnemyShotPatternOmniFour::Execute()
{
	owner_->AddBulletInstance(Temp::Direction::Front);
	owner_->AddBulletInstance(Temp::Direction::Back);
	owner_->AddBulletInstance(Temp::Direction::Left);
	owner_->AddBulletInstance(Temp::Direction::Right);
}
