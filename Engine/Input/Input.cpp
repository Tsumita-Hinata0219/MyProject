#include "Input.h"



// -------------------------------------------------------------------------
// KeyInput : キーボード
// -------------------------------------------------------------------------

// 初期化処理
void KeysInput::Initialize() 
{
	// DirectInputのインスタンス生成
	HRESULT result = DirectInput8Create(
		WinApp::GetWc().hInstance, DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイス生成
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(
		WinApp::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

// 更新処理
void KeysInput::BeginFrame() 
{
	// メモリコピー
	memcpy(preKeys, Keys, 256);

	// キーボード情報の取得開始
	keyboard_->Acquire();

	// 全キーの入力状態を取得する
	keyboard_->GetDeviceState(sizeof(Keys), Keys);
}

// 押されていない
bool KeysInput::NoneKey(uint32_t keyNum) const 
{
	if (preKeys[keyNum] == 0x00 && Keys[keyNum] == 0x00) {
		return true;
	}
	return false;
}

// 押した瞬間
bool KeysInput::TriggerKey(uint32_t keyNum) const 
{
	if (preKeys[keyNum] == 0x00 && Keys[keyNum] == 0x80) {
		return true;
	}
	return false;
}

// 押しっぱなし
bool KeysInput::PressKeys(uint32_t keyNum) const 
{
	if (preKeys[keyNum] == 0x80 && Keys[keyNum] == 0x80) {
		return true;
	}
	return false;
}

// 離された瞬間
bool KeysInput::ReleaseKeys(uint32_t keyNum) const 
{
	if (preKeys[keyNum] == 0x80 && Keys[keyNum] == 0x00) {
		return true;
	}
	return false;
}

// -------------------------------------------------------------------------
// KeyInput : キーボード
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
// GamePadInput : ゲームパッド
// -------------------------------------------------------------------------

// 初期化処理
void GamePadInput::Initialize() 
{
	// 各ボタンのトリガー状態の初期化処理
	ResetButtonTriggers();
}

// 更新処理
void GamePadInput::BeginFrame() 
{
	// メモリコピー
	preJoyState_ = joyState_;

	GetJoyState();

	// ジョイスティックの状態をポーリング
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i) {
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
	}
}

// パッドの状態更新
bool GamePadInput::GetJoyState()
{
	DWORD dwResult = XInputGetState(0, &joyState_);
	if (dwResult == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

// 各ボタンのトリガー状態の初期化処理
void GamePadInput::ResetButtonTriggers()
{
	ZeroMemory(buttonTriggers, sizeof(buttonTriggers));
}

// ジョイコンの入力の取得
bool GamePadInput::GetJoyStickState()
{
	DWORD dwresult = XInputGetState(0, &joyState_);

	if (dwresult == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

// 押されていない
bool GamePadInput::NoneButton(PadData button) const
{
	button;
	return false;
}

// 押した瞬間
bool GamePadInput::TriggerButton(PadData button) const
{
	bool preFlag = false;

	if (preJoyState_.Gamepad.wButtons & (WORD)button)
	{
		preFlag = true;
	}

	if (!preFlag && joyState_.Gamepad.wButtons & (WORD)button)
	{
		return true;
	}

	return false;
}

// 押しっぱなし
bool GamePadInput::PressButton(PadData button) const
{
	if (joyState_.Gamepad.wButtons & (WORD)button)
	{
		return true;
	}
	return false;
}

// 離された瞬間
bool GamePadInput::ReleaseButton(PadData button) const
{
	bool preFlag = false;

	// 前回の状態でボタンが押されていたかを確認
	if (preJoyState_.Gamepad.wButtons & (WORD)button)
	{
		preFlag = true;
	}

	// 現在の状態でボタンが押されていないことを確認
	if (preFlag && !(joyState_.Gamepad.wButtons & (WORD)button))
	{
		return true;
	}

	return false;
}

// Lスティック
Vector2 GamePadInput::GetLStick(const float& mode)
{
	Vector2 result = {
		static_cast<float>(joyState_.Gamepad.sThumbLX) / mode * 1.0f,
		static_cast<float>(joyState_.Gamepad.sThumbLY) / mode * 1.0f,
	};

	result.x = max(-1.0f, min(1.0f, result.x));
	result.y = max(-1.0f, min(1.0f, result.y));

	return result;
}

// Rスティック
Vector2 GamePadInput::GetRStick(const float& mode)
{
	Vector2 result = {
		static_cast<float>(joyState_.Gamepad.sThumbRX) / mode * 1.0f,
		static_cast<float>(joyState_.Gamepad.sThumbRY) / mode * 1.0f,
	};

	result.x = max(-1.0f, min(1.0f, result.x));
	result.y = max(-1.0f, min(1.0f, result.y));

	return result;
}

// -------------------------------------------------------------------------
// GamePadInput : ゲームパッド
// -------------------------------------------------------------------------





// -------------------------------------------------------------------------
// Input : インプット
// -------------------------------------------------------------------------

/// <summary>
/// 初期化処理
/// </summary>
void Input::Initialize()
{
	keysInput_ = KeysInput::GetInstance();
	keysInput_->Initialize();
	gamePadInput_ = GamePadInput::GetInstance();
	gamePadInput_->Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void Input::BeginFrame()
{
	keysInput_->BeginFrame();
	gamePadInput_->BeginFrame();
}

/// <summary>
/// 押されていない
/// </summary>
bool Input::None(uint32_t keyNum)
{
	return keysInput_->NoneKey(keyNum);
}
bool Input::None(PadData button)
{
	return gamePadInput_->NoneButton(button);
}

/// <summary>
/// 押した瞬間
/// </summary>
bool Input::Trigger(uint32_t keyNum)
{
	return keysInput_->TriggerKey(keyNum);
}
bool Input::Trigger(PadData button)
{
	return gamePadInput_->TriggerButton(button);
}

/// <summary>
/// 押しっぱなし
/// </summary>
bool Input::Press(uint32_t keyNum)
{
	return keysInput_->PressKeys(keyNum);
}
bool Input::Press(PadData button)
{
	return gamePadInput_->PressButton(button);
}

/// <summary>
/// 離された瞬間
/// </summary>
bool Input::Release(uint32_t keyNum)
{
	return keysInput_->ReleaseKeys(keyNum);
}
bool Input::Release(PadData button)
{
	return gamePadInput_->ReleaseButton(button);
}

/// <summary>
/// Lスティック
/// </summary>
Vector2 Input::GetLStick(const float& mode)
{
	return gamePadInput_->GetLStick(mode);
}

/// <summary>
/// Rスティック
/// </summary>
Vector2 Input::GetRStick(const float& mode)
{
	return gamePadInput_->GetRStick(mode);
}


void Input::Vibration(int val1, int val2)
{
	std::memset(&Input::GetInstance()->vibration, 0, sizeof(XINPUT_VIBRATION));

	// Set the vibration levels
	Input::GetInstance()->vibration.wLeftMotorSpeed = WORD(val1);  // Left motor vibration (0-65535)
	Input::GetInstance()->vibration.wRightMotorSpeed = WORD(val2); // Right motor vibration (0-65535)

	// Apply the vibration to the controller
	XInputSetState(0, &Input::GetInstance()->vibration);
}


// -------------------------------------------------------------------------
// Input : インプット
// -------------------------------------------------------------------------