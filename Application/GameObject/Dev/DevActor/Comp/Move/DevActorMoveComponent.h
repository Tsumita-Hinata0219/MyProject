#pragma once

#include "Entity/Entity.h"


class DevActorMoveComponent : public IBaseComponent {

public:

	DevActorMoveComponent() = default;
	~DevActorMoveComponent() = default;

	void Init() override;
	void Update(float deltaTime) override;


private:

};


inline void DevActorMoveComponent::Init()
{
	IBaseComponent::Create("DevActorMove");
}

inline void DevActorMoveComponent::Update(float deltaTime)
{
	deltaTime;
}