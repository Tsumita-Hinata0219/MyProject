#pragma once

#include <sol/sol.hpp>
#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <type_traits>
#include "Math/MyMath.h"
#include "../Helpers/LuaHelpers.h"


/* 個々のスクリプトを管理するクラス */
class LuaScript {

public:

	/// <summary>
	/// コンストラク
	/// </summary>
	LuaScript();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~LuaScript() = default;

	/// <summary>
	/// Luaスクリプトを読み込む
	/// </summary>
	bool LoadScript(const std::string& rootPath, const std::string& fileName);

	/// <summary>
	/// 変数取得
	/// </summary>
	template<typename T>
	T GetVariable(const std::string& varName) const;

	/// <summary>
	/// Lua側の関数の呼び出し
	/// </summary>
	template <typename... Args>
	bool CallFunction(const std::string& functionName, Args&&... args);

	/// <summary>
	/// 返り値付きLua側の関数の呼び出し
	/// </summary>
	template <typename Ret, typename... Args>
	std::optional<Ret> CallFunctionWithReturn(const std::string& function, Args&&... args);

	/// <summary>
	/// ImGui上でLuaスクリプトを編集・保存・再読込
	/// </summary>
	void ShowLuaEditorWindow();

	/// <summary>
	/// コールバック登録
	/// </summary>
	void SetReloadCallback(std::function<void()> cb);


private:

	/// <summary>
	/// ファイルを読み込んでバッファにセット
	/// </summary>
	bool LoadFileToBuffer(const std::filesystem::path& path);

	/// <summary>
	/// バッファ内容をファイルへ保存
	/// </summary>
	bool SaveBufferToFile(const std::filesystem::path& path);

	/// <summary>
	/// luaBuffer_の内容でスクリプトを再読み込み
	/// </summary>
	bool ReloadFromBuffer();

	/// <summary>
	/// ファイルパス設定
	/// </summary>
	void SetCurrentFilePath(const std::filesystem::path& path);

	/// <summary>
	/// ユーティリティ関数
	/// </summary>
	std::string LoadFileToString(const std::filesystem::path& path);
	void SaveStringToFile(const std::filesystem::path& path, const std::string& data);

private:

	sol::state lua_;
	std::string luaBuffer_;
	std::filesystem::path currentFilePath_;
	bool hasUnsavedChanges_ = false;
	std::function<void()> reloadCallback_;
	std::string saveErrorMessage_;

};


/// <summary>
/// コンストラクタ
/// </summary>
inline LuaScript::LuaScript()
{
	lua_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);

	// Vector4 構造体を Lua に登録
	lua_.new_usertype<Vector4>("Vector4",
		sol::constructors<Vector4(), Vector4(float, float, float, float)>(),
		"x", &Vector4::x,
		"y", &Vector4::y,
		"z", &Vector4::z,
		"w", &Vector4::w
	);

	// 他の型も登録可能（必要に応じて）
	lua_.new_usertype<Vector3>("Vector3",
		"x", &Vector3::x,
		"y", &Vector3::y,
		"z", &Vector3::z
	);

	lua_.new_usertype<Vector2>("Vector2",
		"x", &Vector2::x,
		"y", &Vector2::y
	);
}


/// <summary>
/// Luaスクリプトを読み込む
/// </summary>
inline bool LuaScript::LoadScript(const std::string& rootPath, const std::string& fileName)
{
	std::filesystem::path fullPath = std::filesystem::path("Resources") / rootPath / fileName;
	SetCurrentFilePath(fullPath);
	if (!LoadFileToBuffer(fullPath)) return false;
	return ReloadFromBuffer();
}


/// <summary>
/// 変数取得
/// </summary>
template<typename T>
T LuaScript::GetVariable(const std::string& varName) const
{
	sol::object obj;
	sol::table current = lua_.globals();
	std::istringstream ss(varName);
	std::string token;

	while (std::getline(ss, token, '.')) {

		obj = current[token];
		if (!obj.valid()) {
			std::cerr << "[Lua Error] Variable part not found: " << token << std::endl;
			return T{};
		}

		if (obj.is<sol::table>()) {
			current = obj.as<sol::table>();
		}
		else if (ss.peek() != EOF) {
			std::cerr << "[Lua Error] Intermediate token is not a table: " << token << std::endl;
			return T{};
		}
	}
	try {
		if constexpr (std::is_same_v<T, Vector4>) {
			return LuaTableToVector4(obj.as<sol::table>());
		}
		else if constexpr (std::is_same_v<T, Vector3>) {
			return LuaTableToVector3(obj.as<sol::table>());
		}
		else if constexpr (std::is_same_v<T, Vector2>) {
			return LuaTableToVector2(obj.as<sol::table>());
		}
		else if constexpr (std::is_same_v<T, float>) {
			return static_cast<float>(obj.as<double>());
		}
		else if constexpr (std::is_same_v<T, int>) {
			return static_cast<int>(obj.as<int>());
		}
		else if constexpr (std::is_same_v<T, std::string>) {
			return obj.as<std::string>();
		}
		else if constexpr (std::is_same_v<T, bool>) {
			return obj.as<bool>();
		}
		else {
			return obj.as<T>();
		}
	}
	catch (const sol::error& e) {
		std::cerr << "[Lua Error] Failed to convert '" << varName << "' : " << e.what() << std::endl;
		return T{};
	}
}

/// <summary>
/// Lua側の関数の呼び出し
/// </summary>
template<typename ...Args>
inline bool LuaScript::CallFunction(const std::string& functionName, Args && ...args)
{
	sol::function func = lua_[functionName];
	if (!func.valid()) {
		std::cerr << "[Lua Error] Function not found: " << functionName << std::endl;
		return false;
	}
	try {
		sol::protected_function_result result = func(std::forward<Args>(args)...);
		if (!result.valid()) {
			sol::error err = result;
			std::cerr << "[Lua Error] Call failed: " << err.what() << std::endl;
			return false;
		}
		return true;
	}
	catch (const sol::error& e) {
		std::cerr << "[Lua Error] Exception during call: " << e.what() << std::endl;
		return false;
	}
}


/// <summary>
/// 返り値付きLua側の関数の呼び出し
/// </summary>
template<typename Ret, typename ...Args>
inline std::optional<Ret> LuaScript::CallFunctionWithReturn(const std::string& functionName, Args && ...args)
{
	sol::function func = lua_[functionName];
	if (!func.valid()) {
		std::cerr << "[Lua Error] Function not found: " << functionName << std::endl;
		return std::nullopt;
	}

	sol::protected_function_result result = func(std::forward<Args>(args)...);
	if (!result.valid()) {
		sol::error err = result;
		std::cerr << "[Lua Error] Call failed: " << err.what() << std::endl;
		return std::nullopt;
	}
	try {
		return result.get<Ret>();
	}
	catch (const sol::error& e) {
		std::cerr << "[Lua Error] Failed to get return value: " << e.what() << std::endl;
		return std::nullopt;
	}
}


/// <summary>
/// ImGui上でLuaスクリプトを編集・保存・再読込
/// </summary>
inline void LuaScript::ShowLuaEditorWindow()
{
	ImGui::Begin("Lua Script Editor");

	// ImGui用編集バッファ（luaBuffer_をコピー）
	static std::string editBuffer = luaBuffer_;
	// バッファ拡張用（InputTextMultilineは固定長が前提なので少し工夫が必要）
	static std::vector<char> tmpBuffer;

	if (tmpBuffer.size() < editBuffer.size() + 1) {
		tmpBuffer.resize(editBuffer.size() + 1024);
	}
	memcpy(tmpBuffer.data(), editBuffer.data(), editBuffer.size());
	tmpBuffer[editBuffer.size()] = '\0';

	if (ImGui::InputTextMultiline("##LuaScript", tmpBuffer.data(), tmpBuffer.size(),
		ImVec2(700, 500))) {
		hasUnsavedChanges_ = true;
		editBuffer = std::string(tmpBuffer.data());
	}
	if (hasUnsavedChanges_) {
		ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1.0f), "*Unsaved changes");
	}

	if (ImGui::Button("Save")) {
		luaBuffer_ = editBuffer; // ここでluaBuffer_に反映させる
		if (SaveBufferToFile(currentFilePath_)) {
			hasUnsavedChanges_ = false;
		}
		else {
			ImGui::OpenPopup("Save Error");
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Reload")) {
		if (ReloadFromBuffer()) {
			editBuffer = luaBuffer_;
			tmpBuffer.resize(editBuffer.size() + 1024);
			memcpy(tmpBuffer.data(), editBuffer.data(), editBuffer.size());
			tmpBuffer[editBuffer.size()] = '\0';
			hasUnsavedChanges_ = false;
		}
	}

	if (ImGui::BeginPopupModal("Save Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::TextWrapped("Failed to save Lua script to file.");
		if (ImGui::Button("OK")) ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}

	ImGui::End();
}


/// <summary>
/// コールバック登録
/// </summary>
inline void LuaScript::SetReloadCallback(std::function<void()> cb)
{
	reloadCallback_ = std::move(cb);
}


/// <summary>
/// ファイルを読み込んでバッファにセット
/// </summary>
inline bool LuaScript::LoadFileToBuffer(const std::filesystem::path& path)
{
	try {
		luaBuffer_ = LoadFileToString(path);
		hasUnsavedChanges_ = false;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "[LuaScript] LoadFileToBuffer failed: " << e.what() << std::endl;
		return false;
	}
}


/// <summary>
/// バッファ内容をファイルへ保存
/// </summary>
inline bool LuaScript::SaveBufferToFile(const std::filesystem::path& path)
{
	try {
		SaveStringToFile(path, luaBuffer_);
		hasUnsavedChanges_ = false;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "[LuaScript] SaveBufferToFile failed: " << e.what() << std::endl;
		return false;
	}
}


/// <summary>
/// luaBuffer_の内容でスクリプトを再読み込み
/// </summary>
inline bool LuaScript::ReloadFromBuffer()
{
	sol::load_result result = lua_.load(luaBuffer_);
	if (!result.valid()) {
		sol::error err = result;
		std::cerr << "[Lua Error] Lua code compile error: " << err.what() << std::endl;
		return false;
	}

	result(); // 実行

	if (reloadCallback_) reloadCallback_();

	std::cout << "[Lua] Reloaded from buffer." << std::endl;
	return true;
}


/// <summary>
/// ファイルパス設定
/// </summary>
inline void LuaScript::SetCurrentFilePath(const std::filesystem::path& path)
{
	currentFilePath_ = path;
}


/// <summary>
/// ユーティリティ関数
/// </summary>
inline std::string LuaScript::LoadFileToString(const std::filesystem::path& path)
{
	std::ifstream file(path, std::ios::binary);
	if (!file) throw std::runtime_error("Failed to open file: " + path.string());
	std::ostringstream ss;
	ss << file.rdbuf();
	return ss.str();
}
inline void LuaScript::SaveStringToFile(const std::filesystem::path& path, const std::string& data)
{
	std::ofstream file(path, std::ios::binary);
	if (!file) throw std::runtime_error("Failed to open file for writing: " + path.string());
	file.write(data.data(), data.size());
}