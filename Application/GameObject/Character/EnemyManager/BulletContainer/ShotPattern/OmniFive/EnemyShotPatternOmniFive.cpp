#include "EnemyShotPatternOmniFive.h"
#include "../../EnemyBulletContainer.h"

void EnemyShotPatternOmniFive::Execute()
{
	owner_->AddBulletInstance(Temp::Direction::Front);
	owner_->AddBulletInstance(Temp::Direction::FrontLeft);
	owner_->AddBulletInstance(Temp::Direction::FrontRight);
	owner_->AddBulletInstance(Temp::Direction::Left);
	owner_->AddBulletInstance(Temp::Direction::Right);
}
