//======================================================================================================================
//
//	ゲーム画面ヘッダー [game.h]
//	Author：藤田勇一
//
//======================================================================================================================
#ifndef _GAME_H_			// このマクロ定義がされていない場合
#define _GAME_H_			// 二重インクルード防止のマクロを定義する

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
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitGame(void);								// ゲーム画面の初期化処理
void UninitGame(void);								// ゲーム画面の終了処理
void UpdateGame(void);								// ゲーム画面の更新処理
void DrawGame(void);								// ゲーム画面の描画処理
void SetGameState(GAMESTATE state, int nCounter);	// ゲーム画面の設定処理
void SetEnablePause(bool bPause);					// ポーズ状態の設定処理
GAMESTATE GetGameState(void);						// ゲーム画面の状態取得処理
bool GetPause(void);								// ポーズ状態の取得処理
int  GetGameMode(void);								// ゲームモードの取得処理

#endif