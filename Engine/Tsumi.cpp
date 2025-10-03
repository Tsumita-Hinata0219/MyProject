#include "Tsumi.h"


Tsumi::Tsumi()
{
	winApp_ = WinApp::GetInstance();
	dxMgr_ = DirectXManager::GetInstance();
	shaderMgr_ = ShaderManager::GetInstance();
	pipeLineMgr_ = PipeLineManager::GetInstance();
	texMgr_ = TextureManager::GetInstance();
	imguiMgr_ = ImGuiManager::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	descriptorMgr_ = DescriptorManager::GetInstance();
	rtvMgr_ = RTVManager::GetInstance();
	descriptorHeapMgr_ = DescriptorHeapManager::GetInstance();
}

void Tsumi::Initialize() {

	// WinAppの初期化処理
	winApp_->Initialize(L"Shape Conquest");

	// DirectXCommonの初期化処理
	dxMgr_->Initialize();

	// ShaderManagerの初期化処理
	shaderMgr_->Initialize();

	// PipeLineManagerの初期化処理
	pipeLineMgr_->CreatePipeLine();

	// TextureManagerの初期化処理
	texMgr_->Initialize();

	// ImGuiの初期化処理
	imguiMgr_->Initialize();

	// Inputの初期化処理
	input_->Initialize();

	// Audioの初期化処理
	audio_->Initialize();

	// DescriptorManagerの初期化処理
	descriptorMgr_->Init();

	// DescriptorHeapManagerの初期化処理
	descriptorHeapMgr_->Init();

	texMgr_->LoadTexture("Texture", "uvChecker.png");
}

void Tsumi::Finalize() {

	audio_->Finalize();
	imguiMgr_->Release();
	texMgr_->Finalize();
	descriptorMgr_->Clear();
}

void Tsumi::BeginFlame() {

	imguiMgr_->BeginFrame();
	input_->BeginFrame();
	descriptorMgr_->BeginFrame();
	descriptorHeapMgr_->BeginFrame();
	rtvMgr_->BeginFrame();
	pipeLineMgr_->PipeLineReset();
}

void Tsumi::EndFlame() {
	
	descriptorHeapMgr_->EndFrame();
	imguiMgr_->EndFrame();
}

bool Tsumi::ProcessMessage() {

	if (winApp_->ProcessMessage()) {
		return true;
	}
	else {

		return false;
	}
}