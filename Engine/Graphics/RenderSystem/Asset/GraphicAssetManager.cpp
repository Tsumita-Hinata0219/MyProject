#include "GraphicAssetManager.h"



/// <summary>
/// インスタンスの取得
/// </summary>
RenderSystem::GraphicAssetManager* RenderSystem::GraphicAssetManager::GetInstance()
{
	static GraphicAssetManager instance;
	return &instance;
}


/// <summary>
/// ファイルを読み込む
/// </summary>
void RenderSystem::GraphicAssetManager::LoadData(const std::string& path, const std::string fileName)
{
	// ファイル名で検索を掛ける
	auto it = sceneDataMap_.find(fileName.substr(0, fileName.size() - 4));

	if (it != sceneDataMap_.end()) { // 既に存在していれば早期return
		return;
	}

	// ないので新しく作る
	// 今回作るModelDatas
	RenderSystem::Rendering::SceneData newData{};
	std::string format = GetExtension(fileName);

	// フォーマットを取得して分岐
	if (format == RenderSystem::FileFormat::OBJ.first) {
		newData = RenderSystem::Loader::LoadObj(path, fileName);
	}
	else if (format == RenderSystem::FileFormat::GLTF.first) {
		newData = RenderSystem::Loader::LoadGLTF(path, fileName);
	}

	// マップコンテナに追加
	sceneDataMap_[newData.name] = newData;
}


/// <summary>
/// SceneDataの取得
/// </summary>
std::optional<RenderSystem::Rendering::SceneData> RenderSystem::GraphicAssetManager::GetSceneData(const std::string& name) const
{
	// 指定された名前で検索をかける
	const auto it = sceneDataMap_.find(name);

	// なかったら std::nullopt を返す
	if (it == sceneDataMap_.end()) {
		return std::nullopt;
	}

	// 見つかった場合、コピーを返す
	return it->second;
}


/// <summary>
/// 指定のモデルデータの破棄
/// </summary>
void RenderSystem::GraphicAssetManager::ModelRemove(std::string name)
{
	// 指定された名前で検索をかける
	const auto& model = sceneDataMap_.find(name);
	// なかったらリターン
	if (model == sceneDataMap_.end()) {
		return;
	}
	// ヒットしたら破棄する
	sceneDataMap_.erase(name);
}


/// <summary>
/// 全てのモデルデータの破棄
/// </summary>
void RenderSystem::GraphicAssetManager::Clear()
{
	sceneDataMap_.clear();
}
