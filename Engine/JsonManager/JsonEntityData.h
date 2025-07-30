#pragma once

#include "Math/MyMath.h"
#include "GameObject/GameObject.h"

#include "GameObject/Character/EnemyManager/Property/EnemyProperty.h"


/* JsonからよみとるPlayerのデータ */
struct EntityPlayerData {

};

/* JsonからよみとるEnemyのデータ */
struct EntityEnemyData {
	enemy::ShotFuncData shotFuncData{};
	enemy::MovementFuncData movementFuncData{};
};


/* JsonからよみとるEntityData */
struct EntityData {
	std::string type;
	std::string entityName;
	SRTData srt{};
	EntityPlayerData playerData{};
	EntityEnemyData enemyData{};
	std::map<std::string, std::unique_ptr<EntityData>> children;
};

