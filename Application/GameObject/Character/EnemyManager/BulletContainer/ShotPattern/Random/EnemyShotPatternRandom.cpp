#include "EnemyShotPatternRandom.h"
#include "../../EnemyBulletContainer.h"

EnemyShotPatternRandom::EnemyShotPatternRandom()
{
	// ランダム生成器作成
	std::random_device rd;
	randEngine_ = std::mt19937(rd());
	behabirDist_ = std::uniform_int_distribution<>(0, 1);
	directionDist_ = std::uniform_real_distribution<float>(-1.0f, 1.0f);

}

void EnemyShotPatternRandom::Execute()
{
	Vector3 direction = Vector3::zero;
	direction.x = directionDist_(randEngine_);
	direction.z = directionDist_(randEngine_);
	owner_->AddBulletInstance(direction);
}
