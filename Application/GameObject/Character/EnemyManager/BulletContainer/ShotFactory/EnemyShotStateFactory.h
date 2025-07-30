#pragma once

#include "../ShotPattern/IEnemyShotPattern.h"
#include "../ShotPattern/Cross/EnemyShotPatternCross.h"	
#include "../ShotPattern/Forward/EnemyShotPatternForward.h"
#include "../ShotPattern/None/EnemyShotPatternNone.h"
#include "../ShotPattern/OmniEight/EnemyShotPatternOmniEight.h"
#include "../ShotPattern/OmniFive/EnemyShotPatternOmniFive.h"
#include "../ShotPattern/OmniFour/EnemyShotPatternOmniFour.h"
#include "../ShotPattern/Random/EnemyShotPatternRandom.h"
#include "../ShotPattern/TripleForward/EnemyShotPatternTripleForward.h"


/* エネミーの射撃パターンのFactory*/
class EnemyShotPatternFactory {

public:

	static std::unique_ptr<IEnemyShotPattern> CreateShotPattern(const std::string& key) {

		if (key == "NONE")
		{
			return std::make_unique<EnemyShotPatternNone>();
		}
		else if (key == "CROSS")
		{
			return std::make_unique<EnemyShotPatternCross>();
		}
		else if (key == "FORWARD")
		{
			return std::make_unique<EnemyShotPatternForward>();
		}
		else if (key == "OMNI_EIGHT")
		{
			return std::make_unique<EnemyShotPatternOmniEight>();
		}
		else if (key == "OMNI_FIVE")
		{
			return std::make_unique<EnemyShotPatternOmniFive>();
		}
		else if (key == "OMNI_FOUR")
		{
			return std::make_unique<EnemyShotPatternOmniFour>();
		}
		else if (key == "RANDOM")
		{
			return std::make_unique<EnemyShotPatternRandom>();
		}
		else if (key == "TRIPLE_FORWARD")
		{
			return std::make_unique<EnemyShotPatternTripleForward>();
		}

		return nullptr;
	}
};

