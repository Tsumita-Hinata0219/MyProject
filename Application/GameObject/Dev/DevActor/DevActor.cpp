#include "DevActor.h"

DevActor::DevActor()
{
	IActor::Create("DevActor");
	lua_ = std::make_unique<LuaScript>();
}

void DevActor::Init()
{
	transNode_->srt.scale = { 0.1f, 0.1f, 0.1f };
	transNode_->srt.translate = { 0.0f, 0.0f, 10.0f };

	IActor::AddComponent(std::make_shared<DevActorSphereColComponent>());
	IActor::AddComponent(std::make_shared<DevActorMoveComponent>());
	IActor::AddComponent(std::make_shared<DevRenderComponent>());

	lua_->LoadScript("LuaScript", "Dev.lua");
	lua_->SetReloadCallback([this]() {LoadData_From_Lua(); });
}

void DevActor::UpdateActor(float deltaTime)
{
	deltaTime;
	lua_->ShowLuaEditorWindow();
	//lua_->ShowAllLuaTables();
}

void DevActor::LoadData_From_Lua()
{
}
