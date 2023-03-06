//======================================================================================================================
//
//	ゲーム画面ヘッダー [game.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _GAME_H_			// このマクロ定義がされていない場合
#define _GAME_H_			// 二重インクルード防止のマクロを定義する

#include "curve.h"

//**********************************************************************************************************************
//　マクロ定義(Judge関係)
//**********************************************************************************************************************
#define JUDGE_FLASH			(0.005f)		// ジャッジの色の変化量
#define JUDGE_ORANGE		(D3DXCOLOR(1.0f, 0.5f, 0.1f, 0.1f))		//明転時の輝き
#define JUDGE_BLACK			(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.1f))		//暗転時の輝き

//**********************************************************************************************************************
//	列挙型定義 (GAMESTATE)
//**********************************************************************************************************************
typedef enum
{
	GAMESTATE_NORMAL = 0,	// 通常状態
	GAMESTATE_END,			// 終了状態
	GAMESTATE_MAX			// この列挙型の総数
} GAMESTATE;

//**********************************************************************************************************************
//	列挙型定義 (GAMEMODE)
//**********************************************************************************************************************
typedef enum
{
	GAMEMODE_EDIT = 0,		// エディットモード
	GAMEMODE_PLAY,			// プレイモード
	GAMEMODE_MAX,			// この列挙型の総数
}GAMEMODE;

//**********************************************************************************************************************
//	列挙型定義 (RESULTSTATE)
//**********************************************************************************************************************
typedef enum
{
	RESULTSTATE_NONE = 0,	// 初期化状態
	RESULTSTATE_CLEAR,		// クリア状態
	RESULTSTATE_TIMEOVER,	// タイムオーバー状態
	RESULTSTATE_LIFEOVER,	// ライフオーバー状態
	RESULTSTATE_MAX			// この列挙型の総数
}RESULTSTATE;

//**********************************************************************************************************************
//	列挙型定義(JUDGESTATE)
//**********************************************************************************************************************
typedef enum
{
	JUDGESTATE_JUSTICE = 0,	// 良い奴
	JUDGESTATE_EVIL,		// 悪い奴
	JUDGESTATE_MAX			// この列挙型の総数
}JUDGESTATE;

//**********************************************************************************************************************
//	列挙型定義(CHICA)
//**********************************************************************************************************************
typedef enum
{
	CHICASTATE_BLACKOUT = 0,	// 暗転状態
	CHICASTATE_WHITEOUT,		// 明転状態
	CHICASTATE_MAX				// この列挙型の総数
}CHICASTATE;

//**********************************************************************************************************************
//	構造体定義(Judge)
//**********************************************************************************************************************
typedef struct
{
	JUDGESTATE state;		// 善悪
	CHICASTATE ticatica;	// チカチカ状態
	D3DXCOLOR col;			// ピカピカの色
}Judge;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitGame(void);								// ゲーム画面の初期化処理
void UninitGame(void);								// ゲーム画面の終了処理
void UpdateGame(void);								// ゲーム画面の更新処理
void DrawGame(void);								// ゲーム画面の描画処理
void SetGameState(GAMESTATE state, int nCounter);	// ゲーム画面の設定処理
void SetEnablePause(bool bPause);					// ポーズ状態の設定処理
GAMESTATE GetGameState(void);						// ゲーム画面の状態取得処理
RESULTSTATE GetResultState(void);					// ゲーム状態の取得処理
bool GetPause(void);								// ポーズ状態の取得処理
int  GetGameMode(void);								// ゲームモードの取得処理

#endif