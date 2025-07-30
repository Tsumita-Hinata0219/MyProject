#pragma once

#include <filesystem>

#include "Platform/WinApp/WinApp.h"
#include "Core/DirectXManager/DirectXManager.h"
#include "Graphics/CommandManager/CommandManager.h"
#include "Resource/View/RTVManager/RTVManager.h"
#include "JapaneseGlyphArray.h"

class ImGuiManager {

private:

	// コンストラクタ、デストラクタ
	ImGuiManager() = default;
	~ImGuiManager() = default;

public:

	// インスタンス取得
	static ImGuiManager* GetInstance();

	// 解放処理
	static  void Release();

	// 初期化
	static void Initialize();

	// フレーム開始
	static void BeginFrame();

	// フレーム終了
	static void EndFrame();
};