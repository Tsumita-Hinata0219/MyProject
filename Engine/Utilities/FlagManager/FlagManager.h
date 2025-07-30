#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <mutex>
#include <../externals/json/json.hpp>


/* フラグ管理クラス */
class FlagManager {

private:

	// フラグのマップ
	std::unordered_map<std::string, bool> m_flags;


private: // シングルトン

	// コンストラクタ、デストラクタ
	FlagManager() = default;
	~FlagManager() = default;
	FlagManager(const FlagManager&) = delete;
	const FlagManager& operator=(const FlagManager&) = delete;

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static FlagManager* GetInstance() {
		static FlagManager instance;
		return &instance;
	}

	/// <summary>
	/// JSONファイルからフラグをロード
	/// </summary>
	void LoadFlagFromJson(const std::string& rootPath, const std::string& fileName);

	/// <summary>
	/// フラグの取得
	/// </summary>
	bool GetFlag(const std::string& key) const;

	/// <summary>
	/// フラグの設定
	/// </summary>
	void SetFlag(const std::string& key, bool value);

	/// <summary>
	/// 全フラグをリセット
	/// </summary>
	void ClearFlag();

	/// <summary>
	/// JSONファイルにフラグを保存
	/// </summary>
	void SaveFlagToJson(const std::string& rootPath, const std::string& fileName);


};
