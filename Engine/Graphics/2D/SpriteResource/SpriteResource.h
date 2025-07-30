#pragma once

#include "Resource/BufferResource/BufferResource.h"
#include "Transform/Structure/Transform.h"
#include "../Structure/SpriteStructure.h"

struct SpriteDatas {

	// メッシュ
	SP::MeshData mesh{};

	// マテリアル
	SP::MaterialData material{};

	// Dissolve
	SP::DissolveData dissolve{};
};

struct SpriteBuffers {

	// メッシュ
	BufferResource<SP::MeshData> mesh;

	// VertexDataBuffer
	BufferResource<VertexData> vertex;

	// IndexDataBuffer
	BufferResource<uint32_t> indeces;

	// マテリアル
	BufferResource<SP::MaterialData> material;

	// Dissolve
	BufferResource<SP::DissolveData> dissolve;

	// Transform
	BufferResource<TransformationMat> transform;
};