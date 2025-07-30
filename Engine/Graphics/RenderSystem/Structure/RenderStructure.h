#pragma once

#include "Math/MyMath.h"
#include "Resource/BufferResource/BufferResource.h"
#include "Transform/Structure/Transform.h"

namespace RenderSystem {

namespace Skinning {

// スキニングの頂点ウェイトデータ
struct VertexWeight {
    float weight;
    uint32_t vertexIndex;
};

// スキニングのジョイントウェイトデータ
struct JointWeight {
    Matrix4x4 inverseBindPoseMatrix;
    std::vector<VertexWeight> vertexWeights;
};

// Node
struct Node {
    QuaternionTransform transform;
    Matrix4x4 localMatrix;
    std::string name;
    std::vector<Node> Children;
};

} // namespace Skinning


namespace Mesh {

// 頂点データ
struct Vertex {
    Vector4 position;
    Vector2 texCoord;
    Vector3 normal;
};

// メッシュデータ
struct MeshData {
    std::map<std::string, Skinning::JointWeight> skinClusterData;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    Skinning::Node rootNode;
};

} // namespace Mesh


namespace Material {

// マテリアルデータ
struct MaterialData {
    Vector4 color = Vector4::one;
    Matrix4x4 uvTransform = Matrix4x4::identity;
    uint32_t textureHandle = 1u;
};

} // namespace Material


namespace Lighting {

// ライトデータ
struct LightData {
    Vector4 color = Vector4::one; // 色(RGBA)
    Vector3 direction = { 0.0f, 1.0f, 0.0f }; // ライトの向き
    float intensity = 1.0f; // 輝度
    float shininess = 1.0f; // 光沢
    int enable = false; // フラグ

    void DrawImGui(std::string label = "") {
        ImGui::RadioButton((label + "_Disabled").c_str(), &enable, 0); ImGui::SameLine();
        ImGui::RadioButton((label + "_Enabled").c_str(), &enable, 1);
        ImGui::ColorEdit4((label + "_color").c_str(), &color.x);
        ImGui::DragFloat3((label + "_direction").c_str(), &direction.x, 0.01f, -1.0f, 1.0f);
        ImGui::DragFloat((label + "_intensity").c_str(), &intensity, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat((label + "_shininess").c_str(), &shininess, 0.01f, 0.0f, 1.0f);
    }
};

// 環境マップデータ
struct EnvironmentData {
    float scale = 0.0f;
    int enable = false;
    int textureHandle = 1u;

    void DrawImGui(std::string label = "") {
        ImGui::RadioButton((label + "_Disabled").c_str(), &enable, 0); ImGui::SameLine();
        ImGui::RadioButton((label + "_Enabled").c_str(), &enable, 1);
        ImGui::DragFloat((label + "_Scale").c_str(), &scale, 0.01f, 0.0f, 1.0f);
    }
};

} // namespace Lighting


namespace PostProcess {

// 色加算
struct ColorAddition {
    Vector4 addColor{};
    float intensity = 0.0f;
    int enable = false;

    void DrawImGui(std::string label = "") {
        ImGui::RadioButton((label + "_Disabled").c_str(), &enable, 0); ImGui::SameLine();
        ImGui::RadioButton((label + "_Enabled").c_str(), &enable, 1);
        ImGui::ColorEdit4((label + "_Color").c_str(), &addColor.x);
        ImGui::DragFloat((label + "_Intensity").c_str(), &intensity, 0.01f, 0.0f, 1.0f);
    }
};

} // namespace PostProcess


namespace Rendering {

// シーンデータ（モデル、マテリアル、ライティングなど）
struct SceneData {
    std::string name;          // モデルの名前
    std::string fileFormat;    // モデルファイルのフォーマット
    Mesh::MeshData mesh;       // メッシュ
    Material::MaterialData material{}; // マテリアル
    Lighting::LightData light{}; // ライト
    Lighting::EnvironmentData environment{}; // 環境マップ
    PostProcess::ColorAddition colorAddition{}; // 色加算
};

// バッファリソース
struct BufferResources {
    BufferResource<Mesh::MeshData> mesh;
    BufferResource<Mesh::Vertex> vertex;
    BufferResource<uint32_t> indices;
    BufferResource<Material::MaterialData> material;
    BufferResource<Lighting::LightData> light;
    BufferResource<Lighting::EnvironmentData> environment;
    BufferResource<PostProcess::ColorAddition> colorAddition;
};

} // namespace Rendering

} // namespace RenderSystem