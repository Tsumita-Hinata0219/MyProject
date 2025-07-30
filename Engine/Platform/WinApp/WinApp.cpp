#include "WinApp.h"
#pragma comment(lib, "winmm.lib")


/// <summary>
/// WinAppのインスタンスの取得
/// </summary>
WinApp* WinApp::GetInstance() {

	static WinApp instance;
	return &instance;
}



/// <summary>
/// 初期化
/// </summary>
void WinApp::Initialize(const wchar_t* title) {

	WinApp::GetInstance()->title_ = title;
	timeBeginPeriod(1);
	CreateGameWindow();
}



/* ----- ゲームウィンドウの作成 -----*/

void WinApp::CreateGameWindow() {

	/* --- ウィンドウクラスを登録する --- */

	// ウィンドウプロシージャ
	WinApp::GetInstance()->wc_.lpfnWndProc = WindowProc;
	// ウィンドウクラス名(なんでも良い)
	WinApp::GetInstance()->wc_.lpszClassName = L"%s", WinApp::GetInstance()->title_;
	// インスタンスハンドル
	WinApp::GetInstance()->wc_.hInstance = GetModuleHandle(nullptr);
	// カーソル
	WinApp::GetInstance()->wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	// ウィンドウクラスを登録する
	RegisterClass(&WinApp::GetInstance()->wc_);


	/* --- ウィンドウサイズを決める --- */

	// クライアント領域のサイズ
	RECT wrc = { 0, 0, WinApp::GetInstance()->kWindowWidth, WinApp::GetInstance()->kWindowHeight };
	// クライアント領域をもとに実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);


	/* --- ウィンドウを生成して表示 --- */

	//ウィンドウの生成
	WinApp::GetInstance()->hwnd_ = CreateWindow(

		WinApp::GetInstance()->wc_.lpszClassName,  //利用するクラス名
		WinApp::GetInstance()->title_,             //タイトルバーの文字(なんでも良い)
		WS_OVERLAPPEDWINDOW,                       //よく見るウィンドウスタイル
		CW_USEDEFAULT,		                       //表示X座標(Windowsに任せる)
		CW_USEDEFAULT,		                       //表示X座標(WindowsOSに任せる)
		wrc.right - wrc.left,                      //ウィンドウ横幅
		wrc.bottom - wrc.top,                      //ウィンドウ縦幅
		nullptr,			                       //親ウィンドウハンドル
		nullptr,			                       //メニューハンドル
		WinApp::GetInstance()->wc_.hInstance,	   //インスタンスハンドル
		nullptr);			                       //オプション


	//ウィンドウを表示する
	ShowWindow(WinApp::GetInstance()->hwnd_, SW_SHOW);
}



/* ----- メッセージの処理 -----*/

bool WinApp::ProcessMessage() {

	if (PeekMessage(&WinApp::GetInstance()->msg_, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&WinApp::GetInstance()->msg_); // キー入力メッセージの処理
		DispatchMessage(&WinApp::GetInstance()->msg_);  // ウィンドウプロシージャにメッセージを送る
	}

	if (WinApp::GetInstance()->msg_.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}



/// <summary>
/// ウィンドウプロシージャ
/// </summary>
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg,
	WPARAM wparam, LPARAM lparam) {


	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	// メッセージが来てたら最優先で処理させる
	switch (msg) {

		// ウィンドウが破棄された
	case WM_DESTROY:

		// OSに対して、アプリ終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準メッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}