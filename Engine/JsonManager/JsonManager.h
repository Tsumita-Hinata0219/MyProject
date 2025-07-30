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

	/// <summary>
	/// マップを空にする
	/// </summary>
	void Clear() { entityMap_.clear(); }


#pragma region Accessor アクセッサ

	/// <summary>
	/// EntityDataのリストの取得
	/// </summary>
	const std::vector<std::unique_ptr<EntityData>>& GetEntityData(const std::string& key) const {
		static const std::vector<std::unique_ptr<EntityData>> empty; // デフォルトの空リスト
		auto it = entityMap_.find(key);
		return (it != entityMap_.end()) ? it->second : empty;
	}

	/// <summary>
	/// データを追加
	/// </summary>
	void AddEntityData(std::unique_ptr<EntityData> entity) {
		entityMap_[entity->entityName].emplace_back(std::move(entity));
	}

#pragma endregion 


private:

	/// <summary>
	/// オブジェクトの走査
	/// </summary>
	std::unique_ptr<EntityData> ScanningEntityData(const std::string& path, nlohmann::json& object);

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

	/// <summary>
	/// PlayerDataの読み込み
	/// </summary>
	EntityPlayerData ScanningPlayerData(nlohmann::json& object);

	/// <summary>
	/// EnemyDataの読み込み
	/// </summary>
	EntityEnemyData ScanningEnemyData(nlohmann::json& object);



private: // メンバ変数

	// 読み込んだ情報をまとめておくコンテナ
	std::map<const std::string, std::vector<std::unique_ptr<EntityData>>> entityMap_;

};

