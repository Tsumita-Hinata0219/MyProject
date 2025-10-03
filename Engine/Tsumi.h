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
	Tsumi();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Tsumi() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// フレームの開始
	/// </summary>
	void BeginFlame();

	/// <summary>
	/// フレームの終了
	/// </summary>
	void EndFlame();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	bool ProcessMessage();

private:


	WinApp* winApp_ = nullptr;
	DirectXManager* dxMgr_ = nullptr;
	ShaderManager* shaderMgr_ = nullptr;
	PipeLineManager* pipeLineMgr_ = nullptr;
	TextureManager* texMgr_ = nullptr;
	ImGuiManager* imguiMgr_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DescriptorManager* descriptorMgr_ = nullptr;
	RTVManager* rtvMgr_ = nullptr;

};