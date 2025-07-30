#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>

#include <wrl.h>
using Microsoft::WRL::ComPtr;
using namespace std;

#include <vector>
#include <iostream>
#include <cstdint>
#include <optional>
#include <map>
#include <span>
#include <utility>
#include <array>

#include "MyMath.h"


// 頂点データ
struct VertexData {
	Vector4 position;
	Vector2 texCoord;
	Vector3 normal;
};


// スプライト構造体
struct SpriteData {
	Vector4 topLeft;
	Vector4 bottomLeft;
	Vector4 topRight;
	Vector4 bottomRight;
};


// uvTransform
struct UVTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// 球
struct SphereData {
	Vector3 center;
	float radius;
};

// EulerTransform
struct EulerTransform {
	Vector3 scale;
	Vector3 rotate; // Eulerでの回転
	Vector3 translate;
};

// QuaternionTransform
struct QuaternionTransform {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

// Node
struct Node {
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> Children;
};

// Joint
struct Joint {
	QuaternionTransform transform; // Transform構造体
	Matrix4x4 localMatrix; // localMatrix
	Matrix4x4 skeletonSpaceMatrix; // skeltonSpaceでの変換行列
	string name; // 名前
	vector<int32_t> children; // 子NodeのIndexのリスト。いなければ空
	int32_t index; // 自身のIndex
	optional<int32_t> parent; // 親JointのInsex。いなければnull
};

// Skeleton
struct Skeleton {
	int32_t root; // RootJointのIndex
	std::map<std::string, int32_t> jointMap; // Joint名とIndexとの辞書
	vector<Joint> joints; // 所属しているジョイント
};

// VertexWeightData
struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};

// JointWeightData
struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

// VertexInfluence
const uint32_t kNumMaxInfluence = 4;
struct VertexInfluence {
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};

// WellForGPU
struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix; // 位置用
	Matrix4x4 skeletonSpaceInverseTransposeMatrix; // 法線用
};

// SkinCluster
struct SkinCluster {
	std::vector<Matrix4x4> inverseBindPosematrices;
	Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;
	Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPallette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE>paletteSrvHandle;
	uint32_t srvHandle;
};

// マテリアル
struct Material {
	Vector4 color;
};
struct MaterialSprite {
	Vector4 color;
	Matrix4x4 uvTransform;
};
struct MaterialParticle {
	Vector4 color;
	Matrix4x4 uvTransform;
};


// マテリアルデータ
struct MaterialData {
	std::string textureFilePath;
};


// モデルデータ
struct ModelData {
	std::map<string, JointWeightData> skinClusterData;
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	MaterialData material;
	uint32_t textureHD;
	Node rootNode;
};


// 平行光源
struct DirectionalLight {
	Vector4 color{};          // ライトの色
	Vector3 direction{};      // ライトの向き
	//Vector3 SpecularFColor; // 鏡面反射色
	float intensity{};        // 輝度
	float sininess{};         // 光沢度
	bool enableLightting{};   // ライティングするか
};

// CompileShader
struct CompileShaders {
	IDxcBlobEncoding* Source;
	DxcBuffer SourceBuffer{};
	IDxcResult* Result;
	IDxcBlobUtf8* Error;
	IDxcBlob* Blob;
};


// Objデータ
struct ObjData {
	uint32_t index;
	uint32_t textureHD;
	ComPtr<ID3D12Resource> resource;
	vector<VertexData> vertices;
	MaterialData material;
	Node node;
};


//
//struct TransformationMatrix {
//	Matrix4x4 WVP;
//	Matrix4x4 World;
//};

//
//struct TransformationViewMatrix {
//	Matrix4x4 view;
//	Matrix4x4 viewProjection;
//	Matrix4x4 orthoGraphic;
//	Vector3 cameraPosition;
//};


struct ParticleTransformationMatrix {
	Matrix4x4 World;
	Vector4 Color;
	Matrix4x4 uvTansform;
};

