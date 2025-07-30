#include "TitleLabelObject.h"



// パラメータ付きコンストラクタ
TitleLabelObject::TitleLabelObject(std::pair<std::string, std::string> file)
{
	this->file_ = file;
}


// 初期化処理
void TitleLabelObject::Init()
{
	// Modelの設定
	modelManager_ = ModelManager::GetInstance();
	modelManager_->LoadModel(file_.first, file_.second);
	model_ = modelManager_->GetModel(RemoveNameSuffix(file_.second)); // ファイル名だけを抽出
	
	uint32_t dds = TextureManager::LoadTexture(file_.first, RemoveNameSuffix(file_.second) + ".dds");
	model_->SetMaterialTexture(dds);

	// 初期カラーは黒
	model_->SetMaterialColor(Temp::Color::BLACK);

	// Transformの初期化
	trans_.Init();

	// セレクトされているときのスケール
	selectingScale_ = 1.3f;
}


// 更新処理
void TitleLabelObject::Update() 
{

#ifdef _DEBUG

	if (ImGui::TreeNode("TitleLabel")) {

		trans_.DrawImGui(RemoveNameSuffix(file_.second));
		ImGui::TreePop();
	}

#endif // _DEBUG
}


// 描画処理
void TitleLabelObject::Draw3D()
{
	model_->Draw(trans_);
}
void TitleLabelObject::Draw2DFront() {}
void TitleLabelObject::Draw2DBack() {}


// 衝突判定コールバック関数
void TitleLabelObject::onCollision([[maybe_unused]] IObject* object)
{
}


// セレクトされていない時の処理
void TitleLabelObject::NonSelecting()
{
	// カラーを黒へ
	model_->SetMaterialColor(Temp::Color::BLACK);
	// scale = 1.0fへ
	trans_.srt.scale = Vector3::one;
}


// セレクトされているときの処理
void TitleLabelObject::OnSelecting()
{
	// カラーを白へ
	model_->SetMaterialColor(Temp::Color::WHITE);
	// 少し大きくする
	trans_.srt.scale = { selectingScale_ , selectingScale_ , 1.0f };
}
