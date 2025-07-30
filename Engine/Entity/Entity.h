#pragma once

#include "Math/MyMath.h"

#include "Actor/IActor.h"

#include "Physics/Collision/Collider/IEntityCollider.h"
#include "Physics/Collision/Collider/CollisionSphere.h"
#include "Physics/Collision/Collider/CollisionAABB.h"

#include "Component/Base/IBaseComponent.h"
#include "Component/Render/IRenderComponent.h"

#include "Lua/Script/LuaScript.h"

#include "Utilities/FlagManager/FlagManager.h"
#include "Graphics/3D/Model/Model.h"
#include "Graphics/2D/Sprite/Sprite.h"
#include "Resource/TextureManager/TextureManager.h"
#include "Audio/Audio.h"
#include "Input/Input.h"

#include "Graphics/GPUParticle/Particle/GPUParticle.h"
#include "Graphics/GPUParticle/Field/ParticleField.h"
#include "Graphics/GPUParticle/Field/Constant/ConstantField.h"
#include "Graphics/GPUParticle/Emitter/Sphere/SphereEmitter.h"

#include "Utilities/ImGuiManager/ImGuiManager.h"
#include "Utilities/ObjectPool/ObjectPool.h"
#include "Utilities/RandomGenerator/RandomGenerator.h"
#include "Utilities/TextLog/TextLog.h"
#include "Utilities/Timer/Timer.h"
#include "Utilities/EnumUtilities/EnumUtilities.h"
#include "Utilities/PtrUtilities/PtrUtilities.h"

#include "System/Camera/Manager/CameraManager.h"