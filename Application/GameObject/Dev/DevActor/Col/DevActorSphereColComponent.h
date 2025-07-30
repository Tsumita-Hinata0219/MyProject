#pragma once

#include "Entity/Entity.h"


class DevActorSphereColComponent: public IBaseComponent {

public:

	DevActorSphereColComponent();
	~DevActorSphereColComponent() = default;

	void Init() override;
	void Update(float deltaTime) override;


private:

	std::shared_ptr<Entity::Collision::CollisionSphere> sphereCol_;

};


inline DevActorSphereColComponent::DevActorSphereColComponent()
{
	sphereCol_ = std::make_shared<Entity::Collision::CollisionSphere>();
	sphereCol_->SetFunction([this](const std::string& key) {owner_.lock()->OnCollision(key); });
}

inline void DevActorSphereColComponent::Init()
{
	IBaseComponent::Create("DevActorSphereColComponent");
	sphereCol_->OnRegister(owner_.lock()->GetName());
}

inline void DevActorSphereColComponent::Update([[maybe_unused]]float deltaTime)
{


}