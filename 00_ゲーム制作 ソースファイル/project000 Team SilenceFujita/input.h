//======================================================================================================================
//
//	入力ヘッダー [input.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _INPUT_H_			// このマクロ定義がされていない場合
#define _INPUT_H_			// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"

//**********************************************************************************************************************
//	列挙型定義 (MOUKEY)
//**********************************************************************************************************************
typedef enum
{
	MOUKEY_LEFT = 0,		// マウス 右クリック
	MOUKEY_RIGHT,			// マウス 左クリック
	MOUKEY_WHEEL,			// マウスホイール 押し込み
	MOUKEY_MAX				// この列挙型の総数
}MOUKEY;

//**********************************************************************************************************************
//	列挙型定義 (JOYKEY)
//**********************************************************************************************************************
typedef enum
{
	JOYKEY_UP = 0,			// 方向キー 上
	JOYKEY_DOWN,			// 方向キー 下
	JOYKEY_LEFT,			// 方向キー 左
	JOYKEY_RIGHT,			// 方向キー 右
	JOYKEY_START,			// STARTボタン
	JOYKEY_BACK,			// BACKボタン
	JOYKEY_LSTICK_PUSH,		// 左スティック押し込み
	JOYKEY_RSTICK_PUSH,		// 右スティック押し込み
	JOYKEY_L1,				// LBボタン
	JOYKEY_R1,				// RBボタン
	JOYKEY_L2,				// LTボタン
	JOYKEY_R2,				// RTボタン
	JOYKEY_A,				// Aボタン
	JOYKEY_B,				// Bボタン
	JOYKEY_X,				// Xボタン
	JOYKEY_Y,				// Yボタン
	JOYKEY_LSTICK_X,		// 左スティック 左右
	JOYKEY_LSTICK_Y,		// 左スティック 上下
	JOYKEY_RSTICK_X,		// 右スティック 左右
	JOYKEY_RSTICK_Y,		// 右スティック 上下
	JOYKEY_MAX				// この列挙型の総数
}JOYKEY;

//**********************************************************************************************************************
//	列挙型定義 (VIBSTATE)
//**********************************************************************************************************************
typedef enum
{
	VIBSTATE_NONE = 0,		// 何もしていない状態
	VIBSTATE_DAMAGE,		// ダメージ状態
	VIBSTATE_BIG_DAMAGE,	// 大ダメージ状態
	VIBSTATE_DEATH,			// 死亡状態
	VIBSTATE_MAX			// この列挙型の総数
}VIBSTATE;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
// 入力デバイス
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);		// 入力デバイスの初期化処理
void UninitInput(void);									// 入力デバイスの終了処理
void UpdateInput(void);									// 入力デバイスの更新処理

// キーボード
bool GetKeyboardPress(int nKey);						// キーボードのプレスの取得処理
bool GetKeyboardTrigger(int nKey);						// キーボードのトリガーの取得処理
bool GetKeyboardRelease(int nKey);						// キーボードのリリースの取得処理

// マウス
bool GetMousePress(int nMouKey);						// マウスのプレスの取得処理
bool GetMouseTrigger(int nMouKey);						// マウスのトリガーの取得処理
bool GetMouseRelease(int nMouKey);						// マウスのリリースの取得処理
D3DXVECTOR3 GetMouseMove(void);							// マウスの移動量の取得処理

// パッド
void  SetVibration(VIBSTATE state, int nPlayer);		// パッドのバイブの設定処理
bool  GetJoyKeyPress(JOYKEY nJoyKey, int nPlayer);		// パッドのプレスの取得処理 (ボタン)
bool  GetJoyKeyL2Press(int nPlayer);					// パッドのプレスの取得処理 (L2)
bool  GetJoyKeyR2Press(int nPlayer);					// パッドのプレスの取得処理 (R2)
SHORT GetJoyStickPressLX(int nPlayer);					// パッドのプレスの取得処理 (Lスティック X)
SHORT GetJoyStickPressLY(int nPlayer);					// パッドのプレスの取得処理 (Lスティック Y)
SHORT GetJoyStickPressRX(int nPlayer);					// パッドのプレスの取得処理 (Lスティック X)
SHORT GetJoyStickPressRY(int nPlayer);					// パッドのプレスの取得処理 (Lスティック Y)

bool GetJoyKeyTrigger(JOYKEY nJoyKey, int nPlayer);		// パッドのトリガーの取得処理 (ボタン)
bool GetJoyKeyL2Trigger(int nPlayer);					// パッドのトリガーの取得処理 (L2)
bool GetJoyKeyR2Trigger(int nPlayer);					// パッドのトリガーの取得処理 (R2)
bool GetJoyKeyRelease(JOYKEY nJoyKey, int nPlayer);		// パッドのリリースの取得処理 (ボタン)

#endif