#include "EnemyShotPatternTripleForward.h"
#include "../../EnemyBulletContainer.h"

void EnemyShotPatternTripleForward::Execute()
{
	owner_->AddBulletInstance(Temp::Direction::Front);
	owner_->AddBulletInstance(Temp::Direction::FrontLeft);
	owner_->AddBulletInstance(Temp::Direction::FrontRight);
}
