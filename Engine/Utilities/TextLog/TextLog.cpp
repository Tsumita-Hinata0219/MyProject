#include "TextLog.h"

// 静的メンバ変数の初期化
TextLog* TextLog::instance_ = nullptr;
std::mutex TextLog::mutex_;
ofstream TextLog::outputfile;

TextLog* TextLog::GetInstance()
{
    // ミューテックスをロックしてインスタンスを取得する
    std::lock_guard<std::mutex> lock(mutex_);
    // インスタンスがまだ存在しない場合は新しく作成する
    if (!instance_) {
        instance_ = new TextLog();
    }
    return instance_;
}

void TextLog::Init()
{
    // ミューテックスをロックして操作を同期する
    std::lock_guard<std::mutex> lock(mutex_);
    outputfile.open("Resources/TextLog/TextLog.txt");
}

void TextLog::Load(const char* text)
{
    // ミューテックスをロックして操作を同期する
    std::lock_guard<std::mutex> lock(mutex_);
    outputfile << text << endl;
    cout << endl;
}

void TextLog::Close()
{
    // ミューテックスをロックして操作を同期する
    std::lock_guard<std::mutex> lock(mutex_);
    outputfile.close();
}

