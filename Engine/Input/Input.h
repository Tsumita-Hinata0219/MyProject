#pragma once

#define DIRECTINPUT_VERSION 0x0800 // DirectInputnoバージョン指定
#include <dinput.h>
#include <Xinput.h>
#define XINPUT_GAMEPAD_MAX 16

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib, "dxguid.lib")

#include "Platform/WinApp/WinApp.h"
#include "Math/Struct.h"
#include "Math/MyMath.h"

#include <cassert>
#include <wrl.h>
#include <array>


/// <summary>
/// ゲームパッドのボタンデータ
/// </summary>
enum class PadData {
	UP = 0x0001,	
	DOWN = 0x0002,	
	LEFT = 0x0004,	
	RIGHT = 0x0008,
	START = 0x0010,			
	BACK = 0x0020,			
	LEFT_THUMB = 0x0040,	
	RIGHT_THUMB = 0x0080,	
	LEFT_SHOULDER = 0x0100,	
	RIGHT_SHOULDER = 0x0200,
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000,
};




// -------------------------------------------------------------------------
// 入力
// -------------------------------------------------------------------------
// KeyInput : キーボード
// -------------------------------------------------------------------------
class KeysInput {

public: // メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static KeysInput* GetInstance() {
		static KeysInput instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// 押されていない
	/// </summary>
	bool NoneKey(uint32_t keyNum) const;

	/// <summary>
	/// 押した瞬間
	/// </summary>
	bool TriggerKey(uint32_t keyNum) const;

	/// <summary>
	/// 押しっぱなし
	/// </summary>
	bool PressKeys(uint32_t keyNum) const;

	/// <summary>
	/// 離された瞬間
	/// </summary>
	bool ReleaseKeys(uint32_t keyNum) const;


private: // メンバ変数

	ComPtr<IDirectInput8> directInput_ = nullptr;
	ComPtr<IDirectInputDevice8> keyboard_ = nullptr;

	BYTE Keys[256] = {};
	BYTE preKeys[256] = {};
};



// -------------------------------------------------------------------------
// 入力
// -------------------------------------------------------------------------
// GamePadInput : ゲームパッド
// -------------------------------------------------------------------------
class GamePadInput {

public: // メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static GamePadInput* GetInstance() {
		static GamePadInput instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// パッドの更新
	/// </summary>
	bool GetJoyState();

	/// <summary>
	/// 各ボタンのトリガー状態の初期化処理
	/// </summary>
	void ResetButtonTriggers();

	/// <summary>
	/// ジョイコンの入力の取得
	/// </summary>
	bool GetJoyStickState();

	/// <summary>
	/// 押されていない
	/// </summary>
	bool NoneButton(PadData button) const;

	/// <summary>
	/// 押した瞬間
	/// </summary>
	bool TriggerButton(PadData button) const;

	/// <summary>
	/// 押しっぱなし
	/// </summary>
	bool PressButton(PadData button) const;

	/// <summary>
	/// 離された瞬間
	/// </summary>
	bool ReleaseButton(PadData button) const;

	/// <summary>
	/// Lスティック
	/// </summary>
	Vector2 GetLStick(const float& mode);

	/// <summary>
	/// Rスティック
	/// </summary>
	Vector2 GetRStick(const float& mode);


private: // メンバ変数

	XINPUT_STATE joyState_{};
	XINPUT_STATE preJoyState_{};

	WORD buttonTriggers[XINPUT_GAMEPAD_MAX]{};
};



// -------------------------------------------------------------------------
// 入力
// -------------------------------------------------------------------------
class Input {

private: // シングルトンデザインパターン

	Input() {};
	~Input() {};

public: // メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static Input* GetInstance() {
		static Input instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// 押されていない
	/// </summary>
	bool None(uint32_t keyNum);
	bool None(PadData button);

	/// <summary>
	/// 押した瞬間
	/// </summary>
	bool Trigger(uint32_t keyNum);
	bool Trigger(PadData button);

	/// <summary>
	/// 押しっぱなし
	/// </summary>
	bool Press(uint32_t keyNum);
	bool Press(PadData button);

	/// <summary>
	/// 離された瞬間
	/// </summary>
	bool Release(uint32_t keyNum);
	bool Release(PadData button);

	/// <summary>
	/// Lスティック
	/// </summary>
	Vector2 GetLStick(const float& mode = SHRT_MAX);

	/// <summary>
	/// Rスティック
	/// </summary>
	Vector2 GetRStick(const float& mode = SHRT_MAX);

	/// <summary>
	/// 倍部
	/// </summary>
	/// <param name="val1"></param>
	/// <param name="val2"></param>
	void Vibration(int val1, int val2);

private: // メンバ変数

	KeysInput* keysInput_ = nullptr;
	GamePadInput* gamePadInput_ = nullptr;

	XINPUT_VIBRATION vibration;
};
