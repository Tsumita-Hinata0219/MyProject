#include "ConfigManager.h"
#include <fstream>


// グループ作成
void ConfigManager::CreateGroup(const std::string& groupName)
{
	// 指定された名前で検索をかける
	auto it = datas_.find(groupName);

	// ヒットしたらreturn
	if (it != datas_.end()) {
		return;
	}

	// 見つからなければ追加
	datas_[groupName];
}


// 項目のセット
void ConfigManager::SetValue(const std::string& groupName, const std::string& key, int32_t value)
{
	// グループを取得
	ConfigGroup& group = datas_[groupName];
	// 新しい項目のデータ
	ConfigItem newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}
void ConfigManager::SetValue(const std::string& groupName, const std::string& key, float value)
{
	// グループを取得
	ConfigGroup& group = datas_[groupName];
	// 新しい項目のデータ
	ConfigItem newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}
void ConfigManager::SetValue(const std::string& groupName, const std::string& key, const Vector3& value)
{
	// グループを取得
	ConfigGroup& group = datas_[groupName];
	// 新しい項目のデータ
	ConfigItem newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}


// 項目の追加
void ConfigManager::AddItem(const std::string& groupName, const std::string& key, int32_t value)
{
	// グループの参照を取得
	ConfigGroup& group = datas_[groupName];

	// 指定された名前で検索をかける
	auto it = group.items.find(key);

	// ヒットしなかったら項目を作る
	if (it == group.items.end()) {
		SetValue(groupName, key, value);
	}
}
void ConfigManager::AddItem(const std::string& groupName, const std::string& key, float value)
{
	// グループの参照を取得
	ConfigGroup& group = datas_[groupName];

	// 指定された名前で検索をかける
	auto it = group.items.find(key);

	// ヒットしなかったら項目を作る
	if (it == group.items.end()) {
		SetValue(groupName, key, value);
	}
}
void ConfigManager::AddItem(const std::string& groupName, const std::string& key, const Vector3& value)
{
	// グループの参照を取得
	ConfigGroup& group = datas_[groupName];

	// 指定された名前で検索をかける
	auto it = group.items.find(key);

	// ヒットしなかったら項目を作る
	if (it == group.items.end()) {
		SetValue(groupName, key, value);
	}
}


// 更新処理
void ConfigManager::DrawImGui()
{
	if (!ImGui::Begin("Config Manager", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) return;

	
	// 各グループについて
	for (std::map<std::string, ConfigGroup>::iterator itGroup = datas_.begin(); 
		itGroup != datas_.end(); 
		++itGroup) {

		// グループ名とグループを取得
		const std::string& groupName = itGroup->first;
		ConfigGroup& group = itGroup->second;

		// グループのメニューを追加する処理
		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;

		// 各項目について
		for (std::map<std::string, ConfigItem>::iterator itItem = group.items.begin(); 
			itItem != group.items.end(); 
			++itItem) {

			// 項目名と項目を取得
			const std::string& itemName = itItem->first;
			ConfigItem& item = itItem->second;

			// int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item.value)) {
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::DragInt(itemName.c_str(), ptr, 1);
			}
			// flaot型の値を保持していれば
			else if (std::holds_alternative<float>(item.value)) {
				float* ptr = std::get_if<float>(&item.value);
				ImGui::DragFloat(itemName.c_str(), ptr, 0.01f);
			}
			// Vector3型の値を保持していれば
			else if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::DragFloat3(itemName.c_str(), &ptr->x, 0.1f);
			}
		}

		// 改行
		ImGui::Text("\n");
		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string mess = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, mess.c_str(), "ConfigVariables", 0);
		}

		ImGui::EndMenu();
	}


	ImGui::EndMenuBar();
	ImGui::End();
}


// Jsonファイルに書き出し
void ConfigManager::SaveFile(const std::string& groupName)
{
	// グループを検索
	std::map<std::string, ConfigGroup>::iterator itGroup = datas_.find(groupName);

	// 未登録チェック
	assert(itGroup != datas_.end());

	// まとめるJson型のデータコンテナ
	json root;
	// jsonオブジェク登録
	root[groupName] = json::object();

	// 各項目について
	for (std::map<std::string, ConfigItem>::iterator itItem = itGroup->second.items.begin(); 
		itItem != itGroup->second.items.end(); 
		++itItem) {

		// 項目名と項目の取得
		const std::string& itemName = itItem->first;
		ConfigItem item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item.value)) {
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);
		}
		// flaot型の値を保持していれば
		else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);
		}
		// Vector3型の値を保持していれば
		else if (std::holds_alternative<Vector3>(item.value)) {
			// Vector3型の値を登録
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({ value.x, value.y, value.z });
		}
	}


	// ディレクトリの作成
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(dir)) {
		std::filesystem::create_directory(dir);
	}

	// 書き込むJsonファイルのフルファイルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs{};
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープン失敗？
	if (ofs.fail()) {
		std::string mess = "Failed open data file for write.";
		MessageBoxA(nullptr, mess.c_str(), "ConfigVariables", 0);
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();
}


// ディレクトリの全ファイルの読み込み
void ConfigManager::LoadFile()
{
	// ディレクトリがなければスキップ
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(dir)) {
		return;
	}

	std::filesystem::directory_iterator dir_it(dir);
	for (const std::filesystem::directory_entry& entry : dir_it) {

		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .json以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		// ファイル読み込み
		LoadFile(filePath.stem().string());
	}
}
void ConfigManager::LoadFile(const std::string& groupName)
{
	// 読み込むJsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に開く
	ifs.open(filePath);

	// ファイルオープン失敗？
	if (ifs.fail()) {
		std::string mess = "Specified file does not exist.";
		MessageBoxA(nullptr, mess.c_str(), "ConfigVariables", 0);
		assert(0);
		return;
	}

	
	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループ検索
	json::iterator itGroup = root.find(groupName);
	// 未登録チェック
	assert(itGroup != root.end());


	// 各アイテムについて
	for (json::iterator itItem = itGroup->begin();
		itItem != itGroup->end();
		++itItem) {

		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			// int型の値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}
		// float型の値を保持していれば
		else if (itItem->is_number_float()) {
			// float型の値を登録
			float value = itItem->get<float>();
			SetValue(groupName, itemName, value);
		}
		// Vector3型の値を保持していれば
		else if (itItem->is_array() && itItem->size() == 3) {
			// float型のjson配列登録
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(groupName, itemName, value);
		}

	}
}

