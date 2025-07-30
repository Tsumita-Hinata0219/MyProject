#include "ImGuiManager.h"
//#include <imgui_impl_dx12.cpp>

ImGuiManager* ImGuiManager::GetInstance() {

	static ImGuiManager instance;
	return &instance;
}


// 解放処理
void ImGuiManager::Release()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


void ImGuiManager::Initialize() {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();

	std::string vsSolutionPath = std::filesystem::current_path().string();
	//このままだとダブルスラッシュで出来ないので
	std::replace(vsSolutionPath.begin(), vsSolutionPath.end(), '\\', '/');
	std::string fontPath = vsSolutionPath + "/" + "Resources/font/GenShinGothic-P-Medium.ttf";
	//std::string fontPath = "Resources/font/GenShinGothic-P-Medium.ttf";
	ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	if (font == NULL) {
		assert(0);
	}

	ImGui_ImplWin32_Init(WinApp::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(
		DirectXManager::GetInstance()->GetDevice(),
		DirectXManager::GetInstance()->GetSwapChains().Desc.BufferCount,
		RTVManager::GetInstance()->GetDesc().Format,
		DirectXManager::GetInstance()->GetSrvDescriptorHeap(),
		DirectXManager::GetInstance()->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		DirectXManager::GetInstance()->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

	// フォントテクスチャの作成
	ImGui_ImplDX12_CreateDeviceObjects();
}


void ImGuiManager::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}


void ImGuiManager::EndFrame() {

	ImGui::Render();

	// 描画先のでDescriptorの設定
	ID3D12DescriptorHeap* descriptorHeap[] = { DirectXManager::GetInstance()->GetSrvDescriptorHeap() };
	CommandManager::GetInstance()->GetList()->SetDescriptorHeaps(1, descriptorHeap);

	//実際のCommandListのImGuiの描画コマンドを進む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CommandManager::GetInstance()->GetList());
}