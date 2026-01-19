//=====================================================================
// 
// 入力処理 [input.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "input.h"
#include "util.h"
#include "DebugProc.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_JOYPAD (4)		//パッドの登録最大数

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;

BYTE g_aKeyState[NUM_KEY_MAX];				//キーボードの入力情報
BYTE g_aKeyTriggerState[NUM_KEY_MAX];		//キーボードのトリガー情報
BYTE g_aKeyReleaseState[NUM_KEY_MAX];		//キーボードのリリース情報
int g_aKeyRepeatState[NUM_KEY_MAX];			//キーボードの長押し情報

DIMOUSESTATE g_aMouseState;					//マウス情報
BYTE g_aMouseTriggerState[MOUSE_MAX];		//マウストリガー
BYTE g_aMouseReleaseState[MOUSE_MAX];		//マウスリリース

XINPUT_STATE g_joyKeyState[MAX_JOYPAD];				//パッド情報
XINPUT_STATE g_joyKeyTriggerState[MAX_JOYPAD];		//パッドトリガー
XINPUT_STATE g_joyKeyReleaseState[MAX_JOYPAD];		//パッドリリース
int g_aJoyKeyRepeatState[MAX_JOYPAD][JOYKEY_MAX];	//パッドリピート
int g_aJoystickState[MAX_JOYPAD][JOYSTICK_MAX];		//スティック情報
XINPUT_VIBRATION g_vibration[MAX_JOYPAD];			//バイブレーション
int g_nCountVibration[MAX_JOYPAD] = {};				//バイブレーションのカウンタ

int g_nPlayerNumber[MAX_JOYPAD];					//登録パッドのプレイヤー数を設定
													//配列の番号が内部、入った値がプレイヤー数
bool g_bRegistrationCheck[MAX_JOYPAD];

//=====================================================================
// 
// ***** キーボード *****
//
//=====================================================================
//=====================================================================
// 初期化処理
//=====================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	LPDIRECTINPUT8 pInput = GetInput();
	
	// 入力デバイスの生成
	if (FAILED(pInput->CreateDevice(
		GUID_SysKeyboard,
		&g_pDevKeyboard,
		NULL
	)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	// キーボードのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	for (int nCntpad = 0; nCntpad < MAX_JOYPAD; nCntpad++)
	{
		g_nPlayerNumber[nCntpad] = 0;
		g_bRegistrationCheck[nCntpad] = false;
	}

	return S_OK;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitKeyboard(void)
{
	// 入力デバイス（キーボード）破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyTriggerState[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeyReleaseState[nCntKey] = g_aKeyState[nCntKey] & (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]);
			g_aKeyState[nCntKey] = aKeyState[nCntKey];

			if (GetKeyboardPress(nCntKey))
			{
				g_aKeyRepeatState[nCntKey] += 1;
			}
			else
			{
				g_aKeyRepeatState[nCntKey] = 0;
			}
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	// キーボードのアクセス権利を再取得
	}
}

//=====================================================================
// キーボードのプレス情報を取得
//=====================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// キーボードのトリガー情報を取得
//=====================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyTriggerState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// キーボードのリリース情報を取得
//=====================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyReleaseState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// キーボードのリピート情報を取得
//=====================================================================
bool GetKeyboardRepeat(int nKey, int nInterval)
{
	return (g_aKeyRepeatState[nKey] == 1 || g_aKeyRepeatState[nKey] >= INPUT_REPEAT_START && g_aKeyRepeatState[nKey] % nInterval == 0) ? true : false;
}

//=====================================================================
// キーボードの状態を取得
//=====================================================================
BYTE* GetKeyState(void)
{
	return &g_aKeyState[0];
}

//=====================================================================
// 
// ***** マウス *****
//
//=====================================================================
//=====================================================================
// 初期化処理
//=====================================================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	LPDIRECTINPUT8 pInput = GetInput();

	// 入力デバイスの生成
	if (FAILED(pInput->CreateDevice(
		GUID_SysMouse,
		&g_pDevMouse,
		NULL
	)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevMouse->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	// マウスのアクセス権を獲得
	g_pDevMouse->Acquire();

	return S_OK;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitMouse(void)
{
	// 入力デバイス（マウス）破棄
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateMouse()
{
	DIMOUSESTATE aMouseState;	// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &aMouseState)))
	{
		for (int nCntKey = 0; nCntKey < MOUSE_MAX; nCntKey++)
		{
			g_aMouseTriggerState[nCntKey] = (g_aMouseState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey]) & aMouseState.rgbButtons[nCntKey];
			g_aMouseReleaseState[nCntKey] = g_aMouseState.rgbButtons[nCntKey] & (g_aMouseState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey]);
			g_aMouseState.rgbButtons[nCntKey] = aMouseState.rgbButtons[nCntKey];
		}
		g_aMouseState.lX = aMouseState.lX;
		g_aMouseState.lY = aMouseState.lY;
		g_aMouseState.lZ = aMouseState.lZ;
	}
	else
	{
		g_pDevMouse->Acquire();	// キーボードのアクセス権利を再取得
	}
}

//=====================================================================
// マウスのプレス情報を取得
//=====================================================================
bool GetMousePress(MOUSE key)
{
	return (g_aMouseState.rgbButtons[key] & 0x80) ? true : false;
}

//=====================================================================
// マウスのトリガー情報を取得
//=====================================================================
bool GetMouseTrigger(MOUSE key)
{
	return (g_aMouseTriggerState[key] & 0x80) ? true : false;
}

//=====================================================================
// マウスのリリース情報を取得
//=====================================================================
bool GetMouseRelease(MOUSE key)
{
	return (g_aMouseReleaseState[key] & 0x80) ? true : false;
}

//=====================================================================
// マウスの取得処理
//=====================================================================
DIMOUSESTATE GetMouse(void)
{
	return g_aMouseState;
}

//=====================================================================
// マウス位置の取得処理
//=====================================================================
D3DXVECTOR2 GetMousePos(void)
{
	HWND hWnd = GetMainWindow();
	RECT rectCurrent;
	POINT point;

	GetClientRect(hWnd, &rectCurrent);
	GetCursorPos(&point);

	ScreenToClient(hWnd, &point);

	point.x = Clamp((int)(SCREEN_WIDTH * ((float)point.x / (rectCurrent.right - rectCurrent.left))), 0, SCREEN_WIDTH - 1);
	point.y = Clamp((int)(SCREEN_HEIGHT * ((float)point.y / (rectCurrent.bottom - rectCurrent.top))), 0, SCREEN_HEIGHT - 1);

	return D3DXVECTOR2(point.x, point.y);
}

//=====================================================================
// 
// ***** ジョイパッド *****
//
//=====================================================================
//=====================================================================
// 初期化処理
//=====================================================================
HRESULT InitJoypad(void)
{
	for (int nCntPad = 0; nCntPad < MAX_JOYPAD; nCntPad++)
	{
		// メモリのクリア
		memset(&g_joyKeyState[g_nPlayerNumber[nCntPad]], 0, sizeof(XINPUT_STATE));
		memset(&g_vibration[g_nPlayerNumber[nCntPad]], 0, sizeof(XINPUT_VIBRATION));
		g_nCountVibration[g_nPlayerNumber[nCntPad]] = 0;

		// XINPUTのステートを有効にする
		XInputEnable(true);
	}

	return S_OK;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitJoypad(void)
{
	// XInputのステートを無効
	XInputEnable(false);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joyKeyState;
	XINPUT_GAMEPAD gamepad;

	for (int nCntpad = 0; nCntpad < MAX_JOYPAD; nCntpad++)
	{
		// ジョイパッドの状態
		if (XInputGetState(nCntpad, &joyKeyState) == ERROR_SUCCESS)
		{
			// ジョイキー
			gamepad = joyKeyState.Gamepad;

			g_joyKeyTriggerState[g_nPlayerNumber[nCntpad]].Gamepad.wButtons = (g_joyKeyState[g_nPlayerNumber[nCntpad]].Gamepad.wButtons ^ joyKeyState.Gamepad.wButtons)
				& joyKeyState.Gamepad.wButtons;
			g_joyKeyReleaseState[g_nPlayerNumber[nCntpad]].Gamepad.wButtons = g_joyKeyState[g_nPlayerNumber[nCntpad]].Gamepad.wButtons & (g_joyKeyState[g_nPlayerNumber[nCntpad]].Gamepad.wButtons
				^ joyKeyState.Gamepad.wButtons);
			g_joyKeyState[g_nPlayerNumber[nCntpad]] = joyKeyState;

			for (int nCntKey = 0; nCntKey < JOYKEY_MAX; nCntKey++)
			{
				if (GetJoypadPress((JOYKEY)nCntKey, nCntpad))
				{
					g_aJoyKeyRepeatState[g_nPlayerNumber[nCntpad]][nCntKey] += 1;
				}
				else
				{
					g_aJoyKeyRepeatState[g_nPlayerNumber[nCntpad]][nCntKey] = 0;
				}
			}

			// スティック
			if (gamepad.sThumbLX < -INPUT_DEADZONE)
			{// L左
				g_aJoystickState[nCntpad][JOYSTICK_L_LEFT]++;
			}
			else
			{
				g_aJoystickState[nCntpad][JOYSTICK_L_LEFT] = 0;
			}

			if (gamepad.sThumbLX > INPUT_DEADZONE)
			{// L右
				g_aJoystickState[nCntpad][JOYSTICK_L_RIGHT]++;
			}
			else
			{
				g_aJoystickState[nCntpad][JOYSTICK_L_RIGHT] = 0;
			}

			if (gamepad.sThumbLY < -INPUT_DEADZONE)
			{// L下
				g_aJoystickState[nCntpad][JOYSTICK_L_DOWN]++;
			}
			else
			{
				g_aJoystickState[nCntpad][JOYSTICK_L_DOWN] = 0;
			}

			if (gamepad.sThumbLY > INPUT_DEADZONE)
			{// L上
				g_aJoystickState[nCntpad][JOYSTICK_L_UP]++;
			}
			else
			{
				g_aJoystickState[nCntpad][JOYSTICK_L_UP] = 0;
			}

			if (gamepad.sThumbLY < -INPUT_DEADZONE)
			{// R左
				g_aJoystickState[nCntpad][JOYSTICK_R_LEFT]++;
			}
			else
			{
				g_aJoystickState[nCntpad][JOYSTICK_R_LEFT] = 0;
			}

			if (gamepad.sThumbLY > INPUT_DEADZONE)
			{// R右
				g_aJoystickState[nCntpad][JOYSTICK_R_RIGHT]++;
			}
			else
			{
				g_aJoystickState[nCntpad][JOYSTICK_R_RIGHT] = 0;
			}

			if (gamepad.sThumbLY < -INPUT_DEADZONE)
			{// R下
				g_aJoystickState[nCntpad][JOYSTICK_R_DOWN]++;
			}
			else
			{
				g_aJoystickState[nCntpad][JOYSTICK_R_DOWN] = 0;
			}

			if (gamepad.sThumbLY > INPUT_DEADZONE)
			{// R上
				g_aJoystickState[nCntpad][JOYSTICK_R_UP]++;
			}
			else
			{
				g_aJoystickState[nCntpad][JOYSTICK_R_UP] = 0;
			}
		}

		// 振動情報の更新
		if (g_nCountVibration[nCntpad] > -1) g_nCountVibration[nCntpad]--;
		if (g_nCountVibration[g_nPlayerNumber[nCntpad]] == 0)
		{
			g_vibration[nCntpad].wLeftMotorSpeed = 0;
			g_vibration[nCntpad].wRightMotorSpeed = 0;
			XInputSetState(nCntpad, &g_vibration[nCntpad]);
		}
	}
}

//=====================================================================
// ジョイパッドの取得
//=====================================================================
XINPUT_STATE* GetJoypad(void)
{
	return &g_joyKeyState[0];
}

//=====================================================================
// ジョイパッドのプレス情報を取得
//=====================================================================
bool GetJoypadPress(JOYKEY key, int nIdx)
{
	return (g_joyKeyState[g_nPlayerNumber[nIdx]].Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// ジョイパッドのトリガー情報を取得
//=====================================================================
bool GetJoypadTrigger(JOYKEY key, int nIdx)
{
	return (g_joyKeyTriggerState[g_nPlayerNumber[nIdx]].Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// ジョイパッドのリリース情報を取得
//=====================================================================
bool GetJoypadRelease(JOYKEY key, int nIdx)
{
	return (g_joyKeyReleaseState[g_nPlayerNumber[nIdx]].Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// ジョイパッドのリピート情報を取得
//=====================================================================
bool GetJoypadRepeat(JOYKEY key, int nIdx, int nInterval)
{
	return (
		g_aJoyKeyRepeatState[g_nPlayerNumber[nIdx]][key] == 1 ||
		g_aJoyKeyRepeatState[g_nPlayerNumber[nIdx]][key] >= INPUT_REPEAT_START &&
		g_aJoyKeyRepeatState[g_nPlayerNumber[nIdx]][key] % nInterval == 0
		) ? true : false;
}

//=====================================================================
// ジョイスティックのプレス情報を取得
//=====================================================================
bool GetJoystickPress(JOYSTICK stick, int nIdx)
{
	return g_aJoystickState[nIdx][stick] > 0;
}

//=====================================================================
// ジョイスティックのプレス情報を取得
//=====================================================================
bool GetJoystickTrigger(JOYSTICK stick, int nIdx)
{
	return g_aJoystickState[g_nPlayerNumber[nIdx]][stick] == 1;
}

//=====================================================================
// ジョイスティックのリピート情報を取得
//=====================================================================
bool GetJoystickRepeat(JOYSTICK stick, int nIdx, int nInterval)
{
	return (
		g_aJoystickState[g_nPlayerNumber[nIdx]][stick] == 1 ||
		g_aJoystickState[g_nPlayerNumber[nIdx]][stick] >= INPUT_REPEAT_START &&
		g_aJoystickState[g_nPlayerNumber[nIdx]][stick] % nInterval == 0
		) ? true : false;
}

//=====================================================================
// 振動情報の設定処理
//=====================================================================
void SetVibration(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, int nIdx, int nCountVibration)
{
	g_nCountVibration[g_nPlayerNumber[nIdx]] = nCountVibration;
	g_vibration[g_nPlayerNumber[nIdx]].wLeftMotorSpeed = wLeftMotorSpeed; // use any value between 0-65535 here
	g_vibration[g_nPlayerNumber[nIdx]].wRightMotorSpeed = wRightMotorSpeed; // use any value between 0-65535 here
	XInputSetState(0, &g_vibration[g_nPlayerNumber[nIdx]]);
}

//=====================================================================
// パッドのプレイヤー数を登録
//=====================================================================
void RegistrationJoypad(int nNumofPlayer)
{//プレイヤーの人数を引数に入力
	int nNum = 0;

	for (int nCntPad = 0; nCntPad < nNumofPlayer; nCntPad++)
		{
			if (GetJoypadPress(JOYKEY_A, nCntPad) == true && g_bRegistrationCheck[nCntPad] != true)
			{
				g_nPlayerNumber[nCntPad] = nNum;
				g_bRegistrationCheck[nCntPad] = true;
				nNum++;
			}
	}

	if (nNum == nNumofPlayer)
	{
		return;
	}
	
}