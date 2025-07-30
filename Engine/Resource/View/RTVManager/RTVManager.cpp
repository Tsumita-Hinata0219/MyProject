#include "RTVManager.h"



// 初期化処理
void RTVManager::Initialize() {}


// 開始処理
void RTVManager::BeginFrame()
{
	if (RTVManager::GetInstance()->index_ >= RTV_Index_Max) {
		// ディスクリプタインデックスが最大値を超えた場合エラーログを出力
		Log("RTVManager index Overflow\n");
		// アサートでプログラムを停止
		assert(0);
	}
}


// ImGui描画
void RTVManager::DrawImGui()
{
	ImGui::Text("RTV_Index : %d", RTVManager::GetInstance()->index_);
}


// RTVDataの追加
void RTVManager::AddRTV(const std::string name, RTVProperty prope)
{
	// インスタンスの取得
	RTVManager* instance = RTVManager::GetInstance();

	// 検索
	auto rtv = instance->GetRTV(name);

	// ヒットしたらreturn。同じものは作らないよ
	if (rtv) { return; }

	// ヒットしなければマップに追加
	unique_ptr<RTVData> newRTV = make_unique<RTVData>(prope);
	instance->rtvMap_[name] = move(newRTV);

	// indexをインクリメント
	instance->index_++;
}


// RTVDataの検索
RTVData* RTVManager::GetRTV(std::string name)
{
	// インスタンスの取得
	RTVManager* instance = RTVManager::GetInstance();

	// 検索
	const auto& rtv = instance->rtvMap_.find(name);

	// ヒットしたらそのポインタを返す
	if (rtv != instance->rtvMap_.end()) {
		return rtv->second.get();
	}

	// なければNULLを返す
	return nullptr;
}


// 指定RTVDataの破棄
void RTVManager::RemoveRTVData(std::string name)
{
	// インスタンスの取得
	RTVManager* instance = RTVManager::GetInstance();

	// 検索
	const auto& rtv = instance->rtvMap_.find(name);

	// なかったらreturn
	if (rtv == instance->rtvMap_.end()) { return; }

	// ヒットしたら破棄
	instance->rtvMap_.erase(name);

	// indexをデクリメント
	instance->index_--;
}


// 全RTVDataの破棄
void RTVManager::AllRemoveRTVData()
{
	// インスタンスの取得
	RTVManager* instance = RTVManager::GetInstance();

	// マップのクリア
	instance->rtvMap_.clear();

	// indexを0で初期化
	instance->index_ = 0;
}

