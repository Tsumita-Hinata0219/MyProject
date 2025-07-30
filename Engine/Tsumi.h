#pragma once

#include "Platform/WinApp/WinApp.h"
#include "Core/DirectXManager/DirectXManager.h"
#include "Graphics/ShaderManager/ShaderManager.h"
#include "Graphics/PipeLineManager/PipeLineManager.h"
#include "Utilities/ImGuiManager/ImGuiManager.h"
#include "Resource/TextureManager/TextureManager.h"
#include "Input/Input.h"
#include "Audio/Audio.h"
#include "Resource/DescriptorManager/DescriptorManager.h"
#include "Resource/View/RTVManager/RTVManager.h"
#include "Resource/View/SRVManager/SRVManager.h"


/* Tsumiクラス */
class Tsumi {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Tsumi() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Tsumi() {};

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static Tsumi* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// フレームの開始
	/// </summary>
	static void BeginFlame();

	/// <summary>
	/// フレームの終了
	/// </summary>
	static void EndFlame();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	static bool ProcessMessage();

private:


};