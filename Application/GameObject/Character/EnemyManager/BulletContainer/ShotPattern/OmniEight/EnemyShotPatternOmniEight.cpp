#include "EnemyShotPatternOmniEight.h"
#include "../../EnemyBulletContainer.h"

void EnemyShotPatternOmniEight::Execute()
{
	owner_->AddBulletInstance(Temp::Direction::Front);
	owner_->AddBulletInstance(Temp::Direction::Back);
	owner_->AddBulletInstance(Temp::Direction::Left);
	owner_->AddBulletInstance(Temp::Direction::Right);
	owner_->AddBulletInstance(Temp::Direction::FrontLeft);
	owner_->AddBulletInstance(Temp::Direction::FrontRight);
	owner_->AddBulletInstance(Temp::Direction::BackLeft);
	owner_->AddBulletInstance(Temp::Direction::BackRight);
}
