#include "EnemyShotPatternCross.h"
#include "../../EnemyBulletContainer.h"

void EnemyShotPatternCross::Execute()
{
	owner_->AddBulletInstance(Temp::Direction::FrontLeft);
	owner_->AddBulletInstance(Temp::Direction::FrontRight);
	owner_->AddBulletInstance(Temp::Direction::BackLeft);
	owner_->AddBulletInstance(Temp::Direction::BackRight);
}
