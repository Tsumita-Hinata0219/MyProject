#include "Skybox.h"
#include "Resource/DescriptorManager/DescriptorManager.h"
#include "Skybox.h"


/// <summary>
/// コンストラクタ
/// </summary>


/// <summary>
/// 初期化処理
/// </summary>
void Skybox::Init(uint32_t dds)
{
	// テクスチャの読み込み
	this->texture_ = dds;

	// Scale
	initScale_ = 1000.0f;

	// Transformの初期化処理
	transform_.Init();
	transform_.srt.scale = { initScale_, initScale_, initScale_ };
	transform_.srt.rotate.y = ToRadians(180.0f);

	// カラー
	datas_.color = Temp::Color::WHITE;

	// カメラマネージャーのインスタンス取得
	cameraManager_ = CameraManager::GetInstance();

	// verticesとindicesの配列のresize
	datas_.vertices.resize(kNumVertices_);
	datas_.indices.resize(kNumIndices_);

	// リソースの作成
	// mesh
	buffers_.vetrices.CreateCBV(UINT(kNumVertices_));
	buffers_.vetrices.CreateVertexBufferView();
	buffers_.indices.CreateCBV(UINT(kNumIndices_));
	buffers_.indices.CreateIndexBufferView();
	// material
	buffers_.material.CreateCBV();
	// Transform
	buffers_.transform.CreateCBV();

	// MeshDataの書き込み
	WriteMeshData();
}


/// <summary>
/// 更新処理
/// </summary>
void Skybox::Update()
{

#ifdef _DEBUG
	if (ImGui::TreeNode("Skybox")) {
		ImGui::DragFloat3("Scale", &transform_.srt.scale.x, 0.1f);
		ImGui::DragFloat3("Rotate", &transform_.srt.rotate.x, 0.1f);
		ImGui::DragFloat3("Translate", &transform_.srt.translate.x, 0.1f);
		ImGui::TreePop();
	}
#endif // _DEBUG
}


/// <summary>
/// 描画処理
/// </summary>
void Skybox::Draw()
{
	// CameraResourceの取得
	auto cameraResource = cameraManager_->GetCameraDataWeak();

	// 諸々の計算
	transform_.UpdateMatrix();
	transform_.transformationMatData.World = transform_.matWorld;
	transform_.transformationMatData.WVP =
		transform_.transformationMatData.World * cameraResource.lock()->viewMatrix * cameraResource.lock()->projectionMatrix;
	transform_.transformationMatData.WorldInverseTranspose = Transpose(Inverse(transform_.matWorld));


	// ここで書き込み
	// VBV
	buffers_.vetrices.UpdateData(datas_.vertices.data());
	// IBV
	buffers_.indices.UpdateData(datas_.indices.data());
	// Transform
	buffers_.transform.UpdateData((&transform_.transformationMatData));
	// Material
	buffers_.material.UpdateData(&datas_.color);

	// コマンドコール
	CommandCall();
}


/// <summary>
/// MeshDataの書き込み
/// </summary>
void Skybox::WriteMeshData()
{
	// ───── verticesの書き込み
	// 右面
	datas_.vertices[0].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	datas_.vertices[1].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	datas_.vertices[2].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	datas_.vertices[3].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	// 左面
	datas_.vertices[4].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	datas_.vertices[5].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	datas_.vertices[6].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	datas_.vertices[7].position = { -1.0f, -1.0f, 1.0f, 1.0f };

	// 前面
	datas_.vertices[8].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	datas_.vertices[9].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	datas_.vertices[10].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	datas_.vertices[11].position = { 1.0f, -1.0f, 1.0f, 1.0f };

	// 後面
	datas_.vertices[12].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	datas_.vertices[13].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	datas_.vertices[14].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	datas_.vertices[15].position = { -1.0f, -1.0f, -1.0f, 1.0f };

	// 上面
	datas_.vertices[16].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	datas_.vertices[17].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	datas_.vertices[18].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	datas_.vertices[19].position = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 下面
	datas_.vertices[20].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	datas_.vertices[21].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	datas_.vertices[22].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	datas_.vertices[23].position = { 1.0f, -1.0f, -1.0f, 1.0f };


	// ───── indicesの書き込み
	// 右面
	datas_.indices[0] = 0; datas_.indices[1] = 1;   
	datas_.indices[2] = 2; datas_.indices[3] = 2;   
	datas_.indices[4] = 1; datas_.indices[5] = 3;

	// 左面
	datas_.indices[6] = 4; datas_.indices[7] = 5;   
	datas_.indices[8] = 6; datas_.indices[9] = 6;   
	datas_.indices[10] = 5;datas_.indices[11] = 7;

	// 前面
	datas_.indices[12] = 8; datas_.indices[13] = 9;  
	datas_.indices[14] = 10; datas_.indices[15] = 10; 
	datas_.indices[16] = 9; datas_.indices[17] = 11;

	// 後面
	datas_.indices[18] = 12; datas_.indices[19] = 13; 
	datas_.indices[20] = 14; datas_.indices[21] = 14; 
	datas_.indices[22] = 13; datas_.indices[23] = 15;

	// 上面
	datas_.indices[24] = 16; datas_.indices[25] = 17; 
	datas_.indices[26] = 18; datas_.indices[27] = 18; 
	datas_.indices[28] = 17; datas_.indices[29] = 19;

	// 下面
	datas_.indices[30] = 20; datas_.indices[31] = 21; 
	datas_.indices[32] = 22; datas_.indices[33] = 22; 
	datas_.indices[34] = 21; datas_.indices[35] = 23;
}


/// <summary>
/// コマンドコール
/// </summary>
void Skybox::CommandCall()
{
	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::SetPipeLine(PipeLine::Container::Graphic, PipeLine::Category::Skybox);


	// VertexBufferView
	buffers_.vetrices.IASetVertexBuffers(1);
	// IndexBufferView
	buffers_.indices.IASetIndexBuffer();
	// Transform
	buffers_.transform.BindGraphicsCBV(1);
	// Material
	buffers_.material.BindGraphicsCBV(0);
	// Camera
	cameraManager_->Bind_CameraData(2);
	// MaterialTexture
	buffers_.material.BindGraphicsSRV(3, texture_);
	// Draw!!
	commands.List->DrawIndexedInstanced(UINT(datas_.indices.size()), 1, 0, 0, 0);
}
