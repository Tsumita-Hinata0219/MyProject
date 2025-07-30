#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <numbers>


#include "json.hpp"

#include "Math/MyMath.h"
#include "GameObject/GameObject.h"

#include "JsonEntityData.h"



/* JsonManagerクラス */
class JsonManager {

private: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	JsonManager() = default;
	~JsonManager() = default;
	JsonManager(const JsonManager&) = delete;
	const JsonManager& operator=(const JsonManager&) = delete;


public: // メンバ関数

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static JsonManager* GetInstance();

	/// <summary>
	/// シーンのJsonの読み込み
	/// </summary>
	void LoadSceneFile(const std::string& path, const std::string& fileName);


#pragma region Accessor アクセッサ

#pragma endregion 


private:

	/// <summary>
	/// タイプ
	/// </summary>
	std::string ScanningType(nlohmann::json& object);

	/// <summary>
	/// エンティティ名
	/// </summary>
	std::string ScanningEntityName(nlohmann::json& object);

	/// <summary>
	/// SRTの読み込み
	/// </summary>
	SRTData ScanningSRT(nlohmann::json& object);


private: // メンバ変数

};

