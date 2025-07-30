#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include <mutex>

using namespace std;

class TextLog {

public:

	static TextLog* GetInstance();

	static void Init();

	static void Load(const char* text);

	static void Close();

private:

    // コンストラクタとデストラクタを private にして外部からのインスタンス生成を禁止する
    TextLog() {}
    ~TextLog() {}

    // シングルトンのインスタンスとミューテックス
    static TextLog* instance_;
    static std::mutex mutex_;

    // ログファイルのストリーム
    static ofstream outputfile;
};
