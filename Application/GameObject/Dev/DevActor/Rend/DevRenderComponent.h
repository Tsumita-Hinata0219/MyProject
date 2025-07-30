#pragma once

#include "Entity/Entity.h"


class DevRenderComponent : public IRenderComponent {

public:

	DevRenderComponent() = default;
	~DevRenderComponent() = default;

	void Init() override;
	void Update() override;

private:

};

inline void DevRenderComponent::Init()
{
	IRenderComponent::Create("Axis");
}

inline void DevRenderComponent::Update()
{
}
