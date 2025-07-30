#pragma once

#include <iostream>
#include <lua.hpp>

int Demo() {
    // Lua状態を新しく作成
    lua_State* L = luaL_newstate();
    if (!L) {
        std::cerr << "LuaJITの初期化に失敗しました！" << std::endl;
        return -1;  // 初期化に失敗した場合
    }

    // 標準ライブラリをロード
    luaL_openlibs(L);

    // Luaが正常に動作しているか確認
    if (luaL_dostring(L, "print('LuaJITが正常にインクルードされました！')") != LUA_OK) {
        std::cerr << "Lua Error: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return -1;
    }

    // 正常に終了したら、Lua状態をクローズ
    lua_close(L);

    std::cout << "LuaJITが正常に動作しています！" << std::endl;
    return 0;
}