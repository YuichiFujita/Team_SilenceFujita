//======================================================================================================================
//
//	入力処理 [input.cpp]
//	Author：藤田勇一
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "effect.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
// キーボード用
#define NUM_KEY_MAX			(256)	// キーの最大数

// マウス用
#define NUM_MOUSEKEY_MAX	(4)		// マウスキーの最大数

// パッド用
#define MAX_PLAYER			(4)		// プレイヤーの最大数
#define VIB_TIME			(20)	// バイブの継続時間
#define VIB_TIME_DEATH		(100)	// 死亡時のバイブ継続時間

#define VIB_DAMAGE_LV		((short)(USHRT_MAX * 0.6f))		// ダメージ時の振動レベル
#define VIB_BIG_DAMAGE_LV	((short)(USHRT_MAX * 0.8f))		// 大ダメージ時の振動レベル
#define VIB_DEATH_LV		(USHRT_MAX)						// 死亡時の振動レベル

//**********************************************************************************************************************
//	構造体定義 (VIBRATION)
//**********************************************************************************************************************
typedef struct
{
	_XINPUT_VIBRATION vibration;	// パッドのバイブ情報
	VIBSTATE          state;		// バイブ使用状況
	int               nTime;		// バイブ使用時間
}VIBRATION;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
// キーボード
HRESULT InitKeyboard(HWND hWnd);	// キーボードの初期化処理
void UninitKeyboard(void);			// キーボードの終了処理
void UpdateKeyboard(void);			// キーボードの更新処理

// マウス
HRESULT InitMouse(HWND hWnd);		// マウスの初期化処理
void UninitMouse(void);				// マウスの終了処理
void UpdateMouse(void);				// マウスの更新処理

// パッド
HRESULT InitJoyPad(void);			// パッドの初期化処理
void UninitJoyPad(void);			// パッドの終了処理
void UpdateJoyPad(void);			// パッドの更新処理

//**********************************************************************************************************************
//	グローバル変数
//**********************************************************************************************************************
// 入力デバイス用
LPDIRECTINPUT8 g_pInput = NULL;								// DirectInput オブジェクトへのポインタ

// キーボード用
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;					// 入力デバイス (キーボード) へのポインタ
BYTE                 g_aKeyState[NUM_KEY_MAX];				// キーボードのプレス情報
BYTE                 g_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードのトリガー情報
BYTE                 g_aKeyStateRelease[NUM_KEY_MAX];		// キーボードのリリース情報

// マウス用
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;					// 入力デバイス (マウス) へのポインタ
DIMOUSESTATE         g_mouseState;							// マウスのプレス情報
DIMOUSESTATE         g_mouseStateTrigger;					// マウスのトリガー情報
DIMOUSESTATE         g_mouseStateRelease;					// マウスのリリース情報

// パッド用
VIBRATION    g_aVibration[MAX_PLAYER];						// パッドのバイブ情報
XINPUT_STATE g_aJoyKeyState[MAX_PLAYER];					// パッドのプレス情報
XINPUT_STATE g_aJoyKeyStateTrigger[MAX_PLAYER];				// パッドのトリガー情報
XINPUT_STATE g_aJoyKeyStateRelease[MAX_PLAYER];				// パッドのリリース情報

//**********************************************************************************************************************
//	入力デバイス処理
//**********************************************************************************************************************
//======================================================================================================================
//	入力デバイスの初期化処理
//======================================================================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput オブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{ // 生成に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// キーボードの初期化
	if (FAILED(InitKeyboard(hWnd)))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// マウスの初期化
	if (FAILED(InitMouse(hWnd)))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// パッドの初期化
	InitJoyPad();

	// 成功を返す
	return S_OK;
}

//======================================================================================================================
//	入力デバイスの終了処理
//======================================================================================================================
void UninitInput(void)
{
	// キーボードの終了
	UninitKeyboard();

	// マウスの終了
	UninitMouse();

	// パッドの終了
	UninitJoyPad();

	// DirectInput オブジェクトの破棄
	if (g_pInput != NULL)
	{ // DirectInput オブジェクトが NULLではない場合

		g_pInput->Release();
		g_pInput = NULL;
	}
}

//======================================================================================================================
//	入力デバイスの更新処理
//======================================================================================================================
void UpdateInput(void)
{
	// キーボードの更新
	UpdateKeyboard();

	// マウスの更新
	UpdateMouse();

	// パッドの更新
	UpdateJoyPad();
}

//**********************************************************************************************************************
//	キーボード処理
//**********************************************************************************************************************
//======================================================================================================================
//	キーボードの初期化処理
//======================================================================================================================
HRESULT InitKeyboard(HWND hWnd)
{
	// 入力デバイス (キーボード) の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{ // 生成に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{ // 設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{ // 設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	// 成功を返す
	return S_OK;
}

//======================================================================================================================
//	キーボードの終了処理
//======================================================================================================================
void UninitKeyboard(void)
{
	// 入力デバイス (キーボード) の破棄
	if (g_pDevKeyboard != NULL)
	{ // 入力デバイスが NULL でない場合

		g_pDevKeyboard->Unacquire();	// キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}

//======================================================================================================================
//	キーボードの更新処理
//======================================================================================================================
void UpdateKeyboard(void)
{
	// 変数配列を宣言
	BYTE aKeyState[NUM_KEY_MAX];		// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{ // 取得に成功した場合

		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{ // キーの最大数分繰り返す

			// キーボードのリリース情報を保存
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];

			// キーボードのトリガー情報を保存
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// キーボードのプレス情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{ // 取得に失敗した場合

		// キーボードへのアクセス権を再度獲得
		g_pDevKeyboard->Acquire();
	}
}

//======================================================================================================================
//	キーボードのプレスの取得処理
//======================================================================================================================
bool GetKeyboardPress(int nKey)
{
	// 真偽を返す (三項演算子)									// 条件式が真なら true 、偽なら false を返す
	return (g_aKeyState[nKey] & 0x80) ? true : false;			// ※ 条件式：() の中
}

//======================================================================================================================
//	キーボードのトリガーの取得処理
//======================================================================================================================
bool GetKeyboardTrigger(int nKey)
{
	// 真偽を返す (三項演算子)									// 条件式が真なら true 、偽なら false を返す
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;	// ※ 条件式：() の中
}

//======================================================================================================================
//	キーボードのリリースの取得処理
//======================================================================================================================
bool GetKeyboardRelease(int nKey)
{
	// 真偽を返す (三項演算子)									// 条件式が真なら true 、偽なら false を返す
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;	// ※ 条件式：() の中
}

//**********************************************************************************************************************
//	マウス処理
//**********************************************************************************************************************
//======================================================================================================================
//	マウスの初期化処理
//======================================================================================================================
HRESULT InitMouse(HWND hWnd)
{
	// 変数を宣言
	DIPROPDWORD diprop;		// デバイスの設定用

	// 入力デバイス (マウス) の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{ // 生成に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{ // 設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))))
	{ // 設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// デバイスの設定
	diprop.diph.dwSize       = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj        = 0;
	diprop.diph.dwHow        = DIPH_DEVICE;
	diprop.dwData            = DIPROPAXISMODE_REL;		// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(g_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{ // デバイスの設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得
	g_pDevMouse->Acquire();

	// 成功を返す
	return S_OK;
}

//======================================================================================================================
//	マウスの終了処理
//======================================================================================================================
void UninitMouse(void)
{
	// 入力デバイス (マウス) の破棄
	if (g_pDevMouse != NULL)
	{ // 入力デバイスが NULL でない場合

		g_pDevMouse->Unacquire();	// マウスへのアクセス権を放棄
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//======================================================================================================================
//	マウスの更新処理
//======================================================================================================================
void UpdateMouse(void)
{
	// 変数を宣言
	DIMOUSESTATE mouseState;		// マウスの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(mouseState), &mouseState)))
	{ // 取得に成功した場合

		for (int nCntMouKey = 0; nCntMouKey < NUM_MOUSEKEY_MAX; nCntMouKey++)
		{ // マウスキーの最大数分繰り返す

			// マウスのリリース情報を保存
			g_mouseStateRelease.rgbButtons[nCntMouKey] = (g_mouseState.rgbButtons[nCntMouKey] | mouseState.rgbButtons[nCntMouKey]) ^ mouseState.rgbButtons[nCntMouKey];

			// マウスのトリガー情報を保存
			g_mouseStateTrigger.rgbButtons[nCntMouKey] = (g_mouseState.rgbButtons[nCntMouKey] ^ mouseState.rgbButtons[nCntMouKey]) & mouseState.rgbButtons[nCntMouKey];
		}

		// マウスのプレス情報を保存
		g_mouseState = mouseState;
	}
	else
	{ // 取得に失敗した場合

		// マウスへのアクセス権を再度獲得
		g_pDevMouse->Acquire();
	}
}

//======================================================================================================================
//	マウスのプレスの取得処理
//======================================================================================================================
bool GetMousePress(int nMouKey)
{
	// 真偽を返す (三項演算子)												// 条件式が真なら true 、偽なら false を返す
	return (g_mouseState.rgbButtons[nMouKey] & 0x80) ? true : false;		// ※ 条件式：() の中
}

//======================================================================================================================
//	マウスのトリガーの取得処理
//======================================================================================================================
bool GetMouseTrigger(int nMouKey)
{
	// 真偽を返す (三項演算子)												// 条件式が真なら true 、偽なら false を返す
	return (g_mouseStateTrigger.rgbButtons[nMouKey] & 0x80) ? true : false;	// ※ 条件式：() の中
}

//======================================================================================================================
//	マウスのリリースの取得処理
//======================================================================================================================
bool GetMouseRelease(int nMouKey)
{
	// 真偽を返す (三項演算子)												// 条件式が真なら true 、偽なら false を返す
	return (g_mouseStateRelease.rgbButtons[nMouKey] & 0x80) ? true : false;	// ※ 条件式：() の中
}

//======================================================================================================================
//	マウスの移動量の取得処理
//======================================================================================================================
D3DXVECTOR3 GetMouseMove(void)
{
	// マウスの移動量を返す
	return D3DXVECTOR3((float)g_mouseState.lX, (float)g_mouseState.lY, (float)g_mouseState.lZ);
}

//**********************************************************************************************************************
//	パッド処理
//**********************************************************************************************************************
//======================================================================================================================
//	パッドの初期化処理
//======================================================================================================================
HRESULT InitJoyPad(void)
{
	// XInputのステートを有効化
	XInputEnable(true);

	for (int nCntJoyKey = 0; nCntJoyKey < MAX_PLAYER; nCntJoyKey++)
	{ // プレイヤーの最大人数分ループ

		// メモリの初期化
		memset(&g_aJoyKeyState[nCntJoyKey], 0, sizeof(XINPUT_STATE));
		memset(&g_aJoyKeyStateTrigger[nCntJoyKey], 0, sizeof(XINPUT_STATE));
		memset(&g_aVibration[nCntJoyKey].vibration, 0, sizeof(_XINPUT_VIBRATION));

		// 構造体の要素の初期化
		g_aVibration[nCntJoyKey].state = VIBSTATE_NONE;	// 何もしていない状態に初期化
		g_aVibration[nCntJoyKey].nTime = 0;				// タイマーを初期化
	}

	// 成功を返す
	return S_OK;
}

//======================================================================================================================
//	パッドの終了処理
//======================================================================================================================
void UninitJoyPad(void)
{
	// XInputのステートを無効化
	XInputEnable(false);
}

//======================================================================================================================
//	パッドの更新処理
//======================================================================================================================
void UpdateJoyPad(void)
{
	// 変数配列を宣言
	XINPUT_STATE aJoyKeyState[MAX_PLAYER];	// パッドの入力情報

	for (int nCntJoyKey = 0; nCntJoyKey < MAX_PLAYER; nCntJoyKey++)
	{ // プレイヤーの最大人数分ループ

		if (XInputGetState(nCntJoyKey, &aJoyKeyState[nCntJoyKey]) == ERROR_SUCCESS)
		{

			// パッドのリリース情報を保存
			g_aJoyKeyStateRelease[nCntJoyKey].Gamepad.wButtons =
			(g_aJoyKeyState[nCntJoyKey].Gamepad.wButtons
			|  aJoyKeyState[nCntJoyKey].Gamepad.wButtons)
			^  aJoyKeyState[nCntJoyKey].Gamepad.wButtons;

			// パッドのトリガー情報を保存 (ボタン)
			g_aJoyKeyStateTrigger[nCntJoyKey].Gamepad.wButtons =
			(g_aJoyKeyState[nCntJoyKey].Gamepad.wButtons
			^  aJoyKeyState[nCntJoyKey].Gamepad.wButtons)
			&  aJoyKeyState[nCntJoyKey].Gamepad.wButtons;

			// パッドのトリガー情報を保存 (L2)
			g_aJoyKeyStateTrigger[nCntJoyKey].Gamepad.bLeftTrigger =
			(g_aJoyKeyState[nCntJoyKey].Gamepad.bLeftTrigger
			^  aJoyKeyState[nCntJoyKey].Gamepad.bLeftTrigger)
			&  aJoyKeyState[nCntJoyKey].Gamepad.bLeftTrigger;

			// パッドのトリガー情報を保存 (R2)
			g_aJoyKeyStateTrigger[nCntJoyKey].Gamepad.bRightTrigger =
			(g_aJoyKeyState[nCntJoyKey].Gamepad.bRightTrigger
			^  aJoyKeyState[nCntJoyKey].Gamepad.bRightTrigger)
			&  aJoyKeyState[nCntJoyKey].Gamepad.bRightTrigger;

			// パッドのプレス情報を保存
			g_aJoyKeyState[nCntJoyKey] = aJoyKeyState[nCntJoyKey];
		}

		switch (g_aVibration[nCntJoyKey].state)
		{ // 振動の更新
		case VIBSTATE_DEATH:	// 死亡状態の場合

			// カウント処理
			if (g_aVibration[nCntJoyKey].nTime > 0)
			{ // バイブ構造体の要素 (nTime) が 0より大きい場合

				// バイブ構造体の要素 (nTime) を減算
				g_aVibration[nCntJoyKey].nTime--;

				// 振動レベルの設定
				g_aVibration[nCntJoyKey].vibration.wLeftMotorSpeed -= (short)(USHRT_MAX / VIB_TIME_DEATH);	// 左
				g_aVibration[nCntJoyKey].vibration.wRightMotorSpeed -= (short)(USHRT_MAX / VIB_TIME_DEATH);	// 右
			}
			else
			{ // バイブ構造体の要素 (nTime) が 0以下になった場合

				// メモリの初期化
				memset(&g_aVibration[nCntJoyKey].vibration, 0, sizeof(_XINPUT_VIBRATION));
			}

			// 処理を抜ける
			break;

		default:				// それ以外の場合

			// カウント処理
			if (g_aVibration[nCntJoyKey].nTime >= 0)
			{ // バイブ構造体の要素 (nTime) が 0より大きい場合

				// バイブ構造体の要素 (nTime) を減算
				g_aVibration[nCntJoyKey].nTime--;
			}
			else
			{ // バイブ構造体の要素 (nTime) が 0以下になった場合

				// 何もしない状態に設定
				g_aVibration[nCntJoyKey].state = VIBSTATE_NONE;

				// メモリの初期化
				memset(&g_aVibration[nCntJoyKey].vibration, 0, sizeof(_XINPUT_VIBRATION));
			}

			// 処理を抜ける
			break;
		}

		// モーターの補正
		if (g_aVibration[nCntJoyKey].vibration.wLeftMotorSpeed < 0)
		{ // 左の振動が 0より小さい場合

			// 左のモーターの補正
			g_aVibration[nCntJoyKey].vibration.wLeftMotorSpeed = 0;
		}
		if (g_aVibration[nCntJoyKey].vibration.wRightMotorSpeed < 0)
		{ // 右の振動が 0より小さい場合

			// 右のモーターの補正
			g_aVibration[nCntJoyKey].vibration.wRightMotorSpeed = 0;
		}

		// バイブ情報を設定
		XInputSetState(nCntJoyKey, &g_aVibration[nCntJoyKey].vibration);
	}
}

//======================================================================================================================
//	パッドのバイブの設定処理
//======================================================================================================================
void SetVibration(VIBSTATE state, int nPlayer)
{
	// バイブの状態を引数のものに設定
	g_aVibration[nPlayer].state = state;

	switch (g_aVibration[nPlayer].state)
	{ // バイブの種類
	case VIBSTATE_DAMAGE:		// ダメージ状態

		// 振動レベルの設定
		g_aVibration[nPlayer].vibration.wLeftMotorSpeed = VIB_DAMAGE_LV;		// 左
		g_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_DAMAGE_LV;		// 右

		// 振動時間の設定
		g_aVibration[nPlayer].nTime = VIB_TIME;

		// 処理を抜ける
		break;

	case VIBSTATE_BIG_DAMAGE:	// 大ダメージ状態

		// 振動レベルの設定
		g_aVibration[nPlayer].vibration.wLeftMotorSpeed = VIB_BIG_DAMAGE_LV;	// 左
		g_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_BIG_DAMAGE_LV;	// 右

		// 振動時間の設定
		g_aVibration[nPlayer].nTime = VIB_TIME;

		// 処理を抜ける
		break;

	case VIBSTATE_DEATH:		// 死亡状態

		// 振動レベルの設定
		g_aVibration[nPlayer].vibration.wLeftMotorSpeed = VIB_DEATH_LV;			// 左
		g_aVibration[nPlayer].vibration.wRightMotorSpeed = VIB_DEATH_LV;		// 右

		// 振動時間の設定
		g_aVibration[nPlayer].nTime = VIB_TIME_DEATH;

		// 処理を抜ける
		break;
	}

	// バイブ情報を設定
	XInputSetState(nPlayer, &g_aVibration[nPlayer].vibration);
}

//======================================================================================================================
//	パッドのプレスの取得処理 (ボタン)
//======================================================================================================================
bool GetJoyKeyPress(JOYKEY nJoyKey, int nPlayer)
{
	// 真偽を返す (三項演算子)															// 条件式が真なら true 、偽なら false を返す
	return (g_aJoyKeyState[nPlayer].Gamepad.wButtons & (1 << nJoyKey)) ? true : false;	// ※ 条件式：() の中
}

//======================================================================================================================
//	パッドのプレスの取得処理 (L2)
//======================================================================================================================
bool GetJoyKeyL2Press(int nPlayer)
{
	// 真偽を返す (三項演算子)															// 条件式が真なら true 、偽なら false を返す
	return (g_aJoyKeyState[nPlayer].Gamepad.bLeftTrigger & 'l') ? true : false;			// ※ 条件式：() の中
}

//======================================================================================================================
//	パッドのプレスの取得処理 (R2)
//======================================================================================================================
bool GetJoyKeyR2Press(int nPlayer)
{
	// 真偽を返す (三項演算子)															// 条件式が真なら true 、偽なら false を返す
	return (g_aJoyKeyState[nPlayer].Gamepad.bRightTrigger & 'r') ? true : false;		// ※ 条件式：() の中
}

//======================================================================================================================
//	パッドのプレスの取得処理 (LSTICK 右左)
//======================================================================================================================
SHORT GetJoyStickPressLX(int nPlayer)
{
	// スティックの傾きの値を返す (右左)
	return g_aJoyKeyState[nPlayer].Gamepad.sThumbLX;
}

//======================================================================================================================
//	パッドのプレスの取得処理 (LSTICK 上下)
//======================================================================================================================
SHORT GetJoyStickPressLY(int nPlayer)
{
	// スティックの傾きの値を返す (上下)
	return g_aJoyKeyState[nPlayer].Gamepad.sThumbLY;
}

//======================================================================================================================
//	パッドのプレスの取得処理 (RSTICK 右左)
//======================================================================================================================
SHORT GetJoyStickPressRX(int nPlayer)
{
	// スティックの傾きの値を返す (右左)
	return g_aJoyKeyState[nPlayer].Gamepad.sThumbRX;
}

//======================================================================================================================
//	パッドのプレスの取得処理 (RSTICK 上下)
//======================================================================================================================
SHORT GetJoyStickPressRY(int nPlayer)
{
	// スティックの傾きの値を返す (上下)
	return g_aJoyKeyState[nPlayer].Gamepad.sThumbRY;
}

//======================================================================================================================
//	パッドのトリガーの取得処理 (ボタン)
//======================================================================================================================
bool GetJoyKeyTrigger(JOYKEY nJoyKey, int nPlayer)
{
	// 真偽を返す (三項演算子)																	// 条件式が真なら true 、偽なら false を返す
	return (g_aJoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (1 << nJoyKey)) ? true : false;	// ※ 条件式：() の中
}

//======================================================================================================================
//	パッドのトリガーの取得処理 (L2)
//======================================================================================================================
bool GetJoyKeyL2Trigger(int nPlayer)
{
	// 真偽を返す (三項演算子)																	// 条件式が真なら true 、偽なら false を返す
	return (g_aJoyKeyStateTrigger[nPlayer].Gamepad.bLeftTrigger & 'l') ? true : false;			// ※ 条件式：() の中
}

//======================================================================================================================
//	パッドのトリガーの取得処理 (R2)
//======================================================================================================================
bool GetJoyKeyR2Trigger(int nPlayer)
{
	// 真偽を返す (三項演算子)																	// 条件式が真なら true 、偽なら false を返す
	return (g_aJoyKeyStateTrigger[nPlayer].Gamepad.bRightTrigger & 'r') ? true : false;			// ※ 条件式：() の中
}

//======================================================================================================================
//	パッドのリリースの取得処理 (ボタン)
//======================================================================================================================
bool GetJoyKeyRelease(JOYKEY nJoyKey, int nPlayer)
{
	// 真偽を返す (三項演算子)																	// 条件式が真なら true 、偽なら false を返す
	return (g_aJoyKeyStateRelease[nPlayer].Gamepad.wButtons & (1 << nJoyKey)) ? true : false;	// ※ 条件式：() の中
}