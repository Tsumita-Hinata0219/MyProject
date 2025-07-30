#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>

class Logger {

public:

    /// <summary>
    /// インスタンスの取得
    /// </summary>
    static Logger& GetInstance();

    /// <summary>
    /// 初期化（ログファイルの指定）
    /// </summary>
    void Init(const std::string& logName);

    /// <summary>
    /// ログの記録
    /// </summary>
    void Log(const std::string& category, const std::string& message);

private:
    Logger() = default;                              // コンストラクタを非公開化
    ~Logger();                                       // デストラクタ
    Logger(const Logger&) = delete;                 // コピーコンストラクタを禁止
    Logger& operator=(const Logger&) = delete;      // 代入演算子を禁止

    std::ofstream logFile_;   // ログファイルストリーム
    std::string fileName_;    // ログファイル名
};
