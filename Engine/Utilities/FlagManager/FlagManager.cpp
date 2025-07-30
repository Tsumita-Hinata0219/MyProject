#include "FlagManager.h"

using json = nlohmann::json;


/// <summary>
/// JSONファイルからフラグをロード
/// </summary>
void FlagManager::LoadFlagFromJson(const std::string& rootPath, const std::string& fileName)
{
    // パスを組み立てる
    std::string filePath = "Resources/" + rootPath + "/" + fileName;

    // ファイルの存在チェック
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "Error: Flag file not found: " << filePath << std::endl;
        return;
    }

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open flag file: " << filePath << std::endl;
        return;
    }

    json flagJson;
    try {
        file >> flagJson;
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON Parse Error: " << e.what() << " in file: " << filePath << std::endl;
        return;
    }
    file.close();

    // フラグを格納
    m_flags.clear();
    for (auto& [key, value] : flagJson.items()) {
        if (value.is_boolean()) {
            m_flags[key] = value.get<bool>();
        }
    }
}


/// <summary>
/// フラグの取得
/// </summary>
bool FlagManager::GetFlag(const std::string& key) const
{
	auto it = m_flags.find(key);
	return (it != m_flags.end()) ? it->second : false;
}


/// <summary>
/// フラグの設定
/// </summary>
void FlagManager::SetFlag(const std::string& key, bool value)
{
	m_flags[key] = value;
}


/// <summary>
/// 全フラグをリセット
/// </summary>
void FlagManager::ClearFlag()
{
	m_flags.clear();
}


/// <summary>
/// JSONファイルにフラグを保存
/// </summary>
void FlagManager::SaveFlagToJson(const std::string& rootPath, const std::string& fileName)
{
    // パスを組み立てる
    std::string filePath = rootPath + "/" + fileName;

    // ディレクトリが存在しない場合は作成
    if (!std::filesystem::exists(rootPath)) {
        try {
            std::filesystem::create_directories(rootPath);
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: Failed to create directory: " << rootPath << " (" << e.what() << ")" << std::endl;
            return;
        }
    }

    // JSONデータを作成
    json flagJson;
    for (const auto& [key, value] : m_flags) {
        flagJson[key] = value;
    }

    // JSONファイルに書き込む
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for writing: " << filePath << std::endl;
        return;
    }

    try {
        file << flagJson.dump(4);  // インデントを4に設定して整形
    }
    catch (const std::exception& e) {
        std::cerr << "Error: Failed to write JSON data to file: " << e.what() << std::endl;
    }

    file.close();
    std::cout << "Flags saved successfully to: " << filePath << std::endl;
}
