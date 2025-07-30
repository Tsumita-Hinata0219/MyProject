#pragma once

#include "Resource/BufferResource/BufferResource.h"
#include "Transform/Structure/Transform.h"
#include "Graphics/3D/Model/Structure/ModelStructure.h"
#include "../Structure/ParticleStructure.h"



namespace GpuParticle {

struct InstanceProperties {
	Matrix4x4 world;
	Vector4 color;
};


struct ModelData {

	// モデルの名前
	std::string name;

	// メッシュ
	MeshData mesh;

	// ライト
	//DirectionalLightData light{};
};

struct ResourceHandles {

	// メッシュ
	BufferResource<MeshData> mesh;

	// VertexDataBuffer
	BufferResource<VertexData> vertex;

	// IndexDataBuffer
	BufferResource<uint32_t> indeces;

	// ParticleElement
	BufferResource<GpuParticle::PropertyCS> particleElement;

	// PreView
	BufferResource<GpuParticle::PreView> preView;

	// Transform
	BufferResource<TransformationMat> transform;

	// マテリアル
	BufferResource<MaterialDataN> material;

	// 生存時間
	BufferResource<GpuParticle::ParticleLifeTime> lifeTimeBuffer_;

	// ライト
	//BufferResource<DirectionalLightData> light;
};
}

