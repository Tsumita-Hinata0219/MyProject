#pragma once


#include <iostream>
#include <sol/sol.hpp>

#include "Math/MyMath.h"


// --- ヘルパー関数：Luaスタックへ値をプッシュ ---
// int, float, double, std::string, const char* に対応
inline void PushValue(lua_State* L, int value) {
	lua_pushinteger(L, value);
}

inline void PushValue(lua_State* L, float value) {
	lua_pushnumber(L, value);
}

inline void PushValue(lua_State* L, double value) {
	lua_pushnumber(L, value);
}

inline void PushValue(lua_State* L, const std::string& value) {
	lua_pushstring(L, value.c_str());
}

inline void PushValue(lua_State* L, const char* value) {
	lua_pushstring(L, value);
}

inline void PushValue(lua_State* L, const Vector2& value) {
	lua_newtable(L);                     // 新しいテーブルを作成
	lua_pushnumber(L, value.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, value.y);
	lua_setfield(L, -2, "y");
}
inline void PushValue(lua_State* L, const Vector3& value) {
	lua_newtable(L);                     // 新しいテーブルを作成
	lua_pushnumber(L, value.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, value.y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, value.z);
	lua_setfield(L, -2, "z");
}
inline void PushValue(lua_State* L, const Vector4& value) {
	lua_newtable(L);                     // 新しいテーブルを作成
	lua_pushnumber(L, value.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, value.y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, value.z);
	lua_setfield(L, -2, "z");
	lua_pushnumber(L, value.w);
	lua_setfield(L, -2, "w");
}


// --- Lua テーブル → Vector 変換 ---
inline Vector2 LuaTableToVector2(const sol::table& tbl) {
	return Vector2{
		tbl.get_or("x", tbl.get_or(1, 0.0f)),
		tbl.get_or("y", tbl.get_or(2, 0.0f))
	};
}

inline Vector3 LuaTableToVector3(const sol::table& tbl) {
	return Vector3{
		tbl.get_or("x", tbl.get_or(1, 0.0f)),
		tbl.get_or("y", tbl.get_or(2, 0.0f)),
		tbl.get_or("z", tbl.get_or(3, 0.0f))
	};
}

inline Vector4 LuaTableToVector4(const sol::table& tbl) {
	return Vector4{
		tbl.get_or("x", tbl.get_or(1, 0.0f)),
		tbl.get_or("y", tbl.get_or(2, 0.0f)),
		tbl.get_or("z", tbl.get_or(3, 0.0f)),
		tbl.get_or("w", tbl.get_or(4, 0.0f))
	};
}


// --- 可変引数を一括でプッシュするための再帰テンプレート関数 ---
// 最後の引数を処理する
template <typename T>
void PushAll(lua_State* L, T value) {
	PushValue(L, value);  // 引数が1つだけの場合、PushValueを呼び出す
}

// 引数が複数ある場合、再帰的にPushValueを呼び出す
template <typename T, typename... Args>
void PushAll(lua_State* L, T value, Args... args) {
	PushValue(L, value);          // 最初の引数を処理
	PushAll(L, args...);          // 残りの引数を再帰的に処理
}

// --- VectorN型編集ヘルパーの実装 (任意) ---
inline bool EditVector2InGui(const char* label, Vector2& vec) {
	bool changed = false;
	ImGui::TextUnformatted(label); // ラベル表示方法はお好みで
	ImGui::Indent(); // インデントすると見やすい
	// PushIDで各要素のIDをユニークにする
	ImGui::PushID((void*)&vec.x); changed |= ImGui::DragFloat("x", &vec.x, 0.01f); ImGui::PopID();
	ImGui::PushID((void*)&vec.y); changed |= ImGui::DragFloat("y", &vec.y, 0.01f); ImGui::PopID();
	// ImGui::Unindent();
	return changed;
}

inline bool EditVector3InGui(const char* label, Vector3& vec) {
	bool changed = false;
	ImGui::TextUnformatted(label); // ラベル表示方法はお好みで
	ImGui::Indent(); // インデントすると見やすい
	ImGui::PushID((void*)&vec.x); changed |= ImGui::DragFloat("x", &vec.x, 0.01f); ImGui::PopID();
	ImGui::PushID((void*)&vec.y); changed |= ImGui::DragFloat("y", &vec.y, 0.01f); ImGui::PopID();
	ImGui::PushID((void*)&vec.z); changed |= ImGui::DragFloat("z", &vec.z, 0.01f); ImGui::PopID();
	return changed;
}

inline bool EditVector4InGui(const char* label, Vector4& vec) {
	bool changed = false;
	ImGui::TextUnformatted(label); // ラベル表示方法はお好みで
	ImGui::Indent(); // インデントすると見やすい
	ImGui::PushID((void*)&vec.x); changed |= ImGui::DragFloat("x", &vec.x, 0.01f); ImGui::PopID();
	ImGui::PushID((void*)&vec.y); changed |= ImGui::DragFloat("y", &vec.y, 0.01f); ImGui::PopID();
	ImGui::PushID((void*)&vec.z); changed |= ImGui::DragFloat("z", &vec.z, 0.01f); ImGui::PopID();
	ImGui::PushID((void*)&vec.w); changed |= ImGui::DragFloat("w", &vec.w, 0.01f); ImGui::PopID();
	return changed;
}

inline std::string ReadFileToString(const std::string& path) {
	std::ifstream inFile(path);
	if (!inFile) {
		std::cerr << "[LuaHelpers] Failed to open file: " << path << std::endl;
		return "";
	}

	std::ostringstream ss;
	ss << inFile.rdbuf();
	return ss.str();
}