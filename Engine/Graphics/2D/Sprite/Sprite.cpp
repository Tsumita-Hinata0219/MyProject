#include "Sprite.h"
#include "System/Camera/Manager/CameraManager.h"



// 初期化処理
void Sprite::Init(Vector2 size)
{
	// CameraManagerのインスタンスの取得
	cameraManager_ = CameraManager::GetInstance();

	// サイズの初期設定
	size_ = size;

	// 基準の初期設定。左上
	anchor_ = SpriteAnchor::TopLeft;

	// TextureはデフォではuvChecker
	datas_.material.textureHandle = 
		TextureManager::LoadTexture("Texture", "uvChecker.png");

	// Mask画像はデフォではuvChecker
	datas_.dissolve.maskTexHandle = 
		TextureManager::LoadTexture("Texture", "uvChecker.png");

	// DatasをもとにBufferを作成
	CreateBufferResource();
}


// 描画処理
void Sprite::Draw(Transform& transform)
{
	// cameraResourceの取得
	auto cameraResource = cameraManager_->GetCameraDataWeak();

	// --- 諸々の計算 ---
	transform.UpdateMatrix();
	transform.transformationMatData.World = transform.matWorld;
	transform.transformationMatData.WVP =
		transform.transformationMatData.World * cameraResource.lock()->viewMatrix * cameraResource.lock()->projectionMatrix;
	transform.transformationMatData.WorldInverseTranspose = Transpose(Inverse(transform.matWorld));

	// --- Meshの設定 --- 
	SetMeshData();

	// --- ここで書き込み ---
	// VBV
	buffers_.vertex.UpdateData(datas_.mesh.vertices.data());
	// IBV
	buffers_.indeces.UpdateData(datas_.mesh.indices.data());
	// Material
	buffers_.material.UpdateData(&datas_.material);
	// Dissolve
	buffers_.dissolve.UpdateData(&datas_.dissolve);
	// Transform
	buffers_.transform.UpdateData(&transform.transformationMatData);

	// コマンドコール
	CommandCall();
}


// コマンドコール
void Sprite::CommandCall()
{
	// Commandの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::SetPipeLine(PipeLine::Container::Graphic, PipeLine::Category::Object2D);

	// VertexBufferView
	buffers_.vertex.IASetVertexBuffers(1);
	// IndexBufferView
	buffers_.indeces.IASetIndexBuffer();
	// Material
	buffers_.material.BindGraphicsCBV(0);
	// Dissolve
	buffers_.dissolve.BindGraphicsCBV(4);
	// Transform
	buffers_.transform.BindGraphicsCBV(1);
	// View
	cameraManager_->Bind_CameraData(2);
	// MaterialTexture
	//SRVManager::SetGraphicsRootDescriptorTable(3, datas_.material.textureHandle);
	buffers_.material.BindGraphicsSRV(3, datas_.material.textureHandle);
	// MaskTexture
	//SRVManager::SetGraphicsRootDescriptorTable(5, datas_.dissolve.maskTexHandle);
	buffers_.material.BindGraphicsSRV(5, datas_.dissolve.maskTexHandle);
	// Draw!!
	commands.List->DrawIndexedInstanced(6, 1, 0, 0, 0);
}


// DatasをもとにBufferを作成
void Sprite::CreateBufferResource()
{
	// VerticesとIndicesのresize
	datas_.mesh.vertices.resize(verticesSize_);
	datas_.mesh.indices.resize(indicesSize_);

	// mesh
	buffers_.mesh.CreateCBV(UINT(datas_.mesh.vertices.size()));
	// vertexBufferView
	buffers_.vertex.CreateCBV(UINT(datas_.mesh.vertices.size()));
	buffers_.vertex.CreateVertexBufferView();
	// indexBufferView
	buffers_.indeces.CreateCBV(UINT(datas_.mesh.indices.size()));
	buffers_.indeces.CreateIndexBufferView();
	// material
	buffers_.material.CreateCBV();
	// Dissolve
	buffers_.dissolve.CreateCBV();
	// transform
	buffers_.transform.CreateCBV();
}


// 四角形の頂点の設定
void Sprite::SetMeshData()
{
	// Anchorで処理を変える
	SP::MeshData mesh{};
	// VerticesとIndicesのresize
	mesh.vertices.resize(verticesSize_);
	mesh.indices.resize(indicesSize_);

	// --- Vertices ---
	switch (anchor_)
	{
	case SpriteAnchor::TopLeft:
		// 左下
		mesh.vertices[0].position = { 0.0f, size_.y, 0.0f, 1.0f };
		// 左上
		mesh.vertices[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };
		// 右下
		mesh.vertices[2].position = { size_.x, size_.y, 0.0f, 1.0f };
		// 右上
		mesh.vertices[3].position = { size_.x, 0.0f, 0.0f, 1.0f };
		break;

	case SpriteAnchor::Center:
		// 左下
		mesh.vertices[0].position = { -size_.x / 2.0f, size_.y / 2.0f, 0.0f, 1.0f };
		// 左上
		mesh.vertices[1].position = { -size_.x / 2.0f, -size_.y / 2.0f, 0.0f, 1.0f };
		// 右下
		mesh.vertices[2].position = { size_.x / 2.0f, size_.y / 2.0f, 0.0f, 1.0f };
		// 右上
		mesh.vertices[3].position = { size_.x / 2.0f, -size_.y / 2.0f, 0.0f, 1.0f };
		break;
	}
	// 左下
	mesh.vertices[0].texCoord = src_.LeftDown;
	// 左上
	mesh.vertices[1].texCoord = src_.LeftUp;
	// 右下
	mesh.vertices[2].texCoord = src_.RightDown;
	// 右上
	mesh.vertices[3].texCoord = src_.RightUp;

	// --- Indices ---
	mesh.indices[0] = 0; mesh.indices[1] = 1; mesh.indices[2] = 2;
	mesh.indices[3] = 1; mesh.indices[4] = 3; mesh.indices[5] = 2;

	// DatasのMeshに値をいれる
	datas_.mesh = mesh;
}
