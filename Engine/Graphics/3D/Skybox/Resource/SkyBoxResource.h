#pragma once

#include "Resource/BufferResource/BufferResource.h"
#include "Transform/Structure/Transform.h"


struct SkyBoxDatas {

	// メッシュ
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;

	// マテリアル
	Vector4 color = Vector4::one;
};

struct SkyBoxBuffers {

	// メッシュ
	BufferResource<VertexData> vetrices;
	BufferResource<uint32_t> indices;

	// マテリアル
	BufferResource<Vector4> material;

	// Transform
	BufferResource<TransformationMat> transform;
};