#pragma once

#include "Math/MyMath.h"
#include "Math/Struct.h"


namespace SP {

	// メッシュデータ
	struct MeshData {
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices;
	};

	// マテリアルデータ
	struct MaterialData {
		Vector4 color = Temp::Color::WHITE;
		Matrix4x4 uvTransform = Matrix4x4::identity;
		uint32_t textureHandle = 1u;
	};

	// DissolveData
	struct DissolveData {
		int isActive = false;
		float threshold = 0.0f;
		uint32_t maskTexHandle = 0u;
	};
}