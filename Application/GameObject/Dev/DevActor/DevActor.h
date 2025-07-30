#pragma once

#include "Entity/Entity.h"

#include "Col/DevActorSphereColComponent.h"
#include "Comp/Move/DevActorMoveComponent.h"
#include "Rend/DevRenderComponent.h"


class DevActor : public IActor {

private:

	std::unique_ptr<LuaScript> lua_;


public:

	DevActor();
	~DevActor() = default;

	void Init() override;
	void UpdateActor(float deltaTime) override;


private:

	void LoadData_From_Lua();

};

