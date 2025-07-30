#pragma once

#include <memory>
#include <functional>
#include <random>

#include "../Bullet/EnemyBullet.h"

#include "../../../GameObject.h"


namespace enemy {


	enum class ShotDirection {
		None,
		Forward,
		TripleForward,
		Cross,
		Omni_Four,
		Omni_Five,
		Omni_Eight,
		Random,
	};

	enum class BulletBehavior {
		None,
		Common,
		Resistant,
		Random,
	};

	enum class MovementBehavior {
		None,
		Static,
		Follow,
		Horizontal,
		Circular,
	};

	struct ShotFuncData {
		ShotDirection direction;
		BulletBehavior behavior;
		std::string shotPattern;
		float shotInterval = 0.0f;
	};
	struct MovementFuncData {
		MovementBehavior behavior;
		bool isTilt = false;
		float velocity = 0.0f;
		Vector3 horizontal_start{};
		Vector3 horizontal_end{};
		Vector3 horizontal_middle{};
		Vector3 circular_center{};
		float circular_radius{};
	};

}