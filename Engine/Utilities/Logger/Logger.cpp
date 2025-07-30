#include "Logger.h"
#include <filesystem>
#include <iostream>


/// <summary>
/// インスタンスの取得
/// </summary>
Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;

}

/// <summary>
/// 初期化：ファイル名を指定
/// </summary>
void Logger::Init(const std::string& logName)
{
    fileName_ = logName;

    // ログディレクトリが存在しなければ作成
    std::filesystem::create_directory("Resources/Logger");

    // ログファイルのパスを決定
    std::string filePath = "Resources/Logger/" + fileName_ + ".txt";

    // ファイルが既に存在していれば、その中身を空にする
    logFile_.open(filePath, std::ios::out | std::ios::trunc);
    if (!logFile_.is_open()) {
        std::cerr << "Failed to open log file: " << filePath << std::endl;
        return;
    }

    // ファイルが空になったことを記録（オプション）
    logFile_ << "Log started for " << fileName_ << "\n";
}


/// <summary>
/// ログを記録
/// </summary>
void Logger::Log(const std::string& category, const std::string& message)
{
    if (logFile_.is_open()) {
        logFile_ << "[" << category << "] " << message << std::endl;
    }
}

/// <summary>
/// デストラクタ
/// </summary>
Logger::~Logger()
{
    if (logFile_.is_open()) {
        logFile_.close();
    }
}
