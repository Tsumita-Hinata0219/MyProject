#pragma once

#include "Resource/BufferResource/BufferResource.h"
#include "Transform/Structure/Transform.h"
#include "../Structure/ModelStructure.h"


struct ModelDatas {

	// モデルの名前
	std::string name;

	// モデルファイルのフォーマット
	std::string fileFormat;

	// メッシュ
	MeshData mesh;

	// マテリアル
	MaterialDataN material{};

	// ライト
	DirectionalLightData light{};

	// 環境マップ
	EnvironmentData environment{};

	// 色加算
	ColorAddition colorAddition{};

	// 法線マップ
	NormalMap normalMap{};

	// デカール
	DecalSetting decal{};

	// ジョイントウェイト
	JointWeightData jointWeight{};

	// スケルトン
	Skeleton skeleton{};

	// スキンクラスター
	//SkinClusterData skinCluster{};
};

struct ModelBuffers {

	// メッシュ
	BufferResource<MeshData> mesh;

	// VertexDataBuffer
	BufferResource<VertexData> vertex;

	// IndexDataBuffer
	BufferResource<uint32_t> indices;

	// マテリアル
	BufferResource<MaterialDataN> material;

	// Transform
	BufferResource<TransformationMat> transform;

	// ライト
	BufferResource<DirectionalLightData> light;

	// 環境マップ
	BufferResource<EnvironmentData> enviroment;

	// 色加算
	BufferResource<ColorAddition> colorAddition;

	// 法線マップ
	BufferResource<NormalMap> normalMap;

	// デカール
	BufferResource<DecalSetting> decal;

	// インフルエンス
	BufferResource<VertexInfluence> influence;

	// パレット
	BufferResource<WellForGPU> palette;
};