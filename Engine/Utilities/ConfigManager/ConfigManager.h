#pragma once

#include <variant>
#include <stdexcept>
#include <filesystem>
#include <iosfwd>
#include <json.hpp>

#include "Math/MyMath.h"



// 項目構造体
struct ConfigItem {
	// 項目の値
	std::variant<int32_t, float, Vector3> value;
};
// グループ構造体
struct ConfigGroup {
	std::map<std::string, ConfigItem> items;
};

using json = nlohmann::json;
// 保存先のファイルパス
const std::string kDirectoryPath = "Resources/ConfigVariables/";


/* 調整項目クラス */
class ConfigManager {

private: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	ConfigManager() = default;
	~ConfigManager() = default;
	ConfigManager(const ConfigManager&) = delete;
	const ConfigManager& operator=(const ConfigManager&) = delete;


public: // メンバ関数

	// インスタンス取得
	static ConfigManager* GetInstance() {
		static ConfigManager instance;
		return &instance;
	}

	/// <summary>
	/// グループ作成
	/// </summary>
	/// <param name="groupName"> グループ名 </param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// 項目のセット
	/// </summary>
	/// <param name="groupName"> グループ名 </param>
	/// <param name="key"> 項目名 </param>
	/// <param name="value"> 値 </param>
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 項目の追加
	/// </summary>
	/// <param name="groupName"> グループ名 </param>
	/// <param name="key"> 項目名 </param>
	/// <param name="value"> 値 </param>
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	void AddItem(const std::string& groupName, const std::string& key, float value);
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 更新処理
	/// </summary>
	void DrawImGui();

	/// <summary>
	/// Jsonファイルに書き出し
	/// </summary>
	/// <param name="groupName"> グループ名 </param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイルの読み込み
	/// </summary>
	/// <param name="groupName"> グループ名 </param>
	void LoadFile();
	void LoadFile(const std::string& groupName);


#pragma region Accessor アクセッサ


#pragma endregion 


private: // メンバ変数

	// 全データ
	std::map<std::string, ConfigGroup> datas_;

};

