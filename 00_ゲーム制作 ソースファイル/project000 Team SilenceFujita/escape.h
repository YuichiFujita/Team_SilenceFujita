//===========================================
//
//脱出通知ヘッダー[escape.h]
//Author 小原立暉
//
//===========================================
#ifndef _ESCAPE_H_
#define _ESCAPE_H_

#include"main.h"

//===============================================
// 列挙型定義(ESCAPESTATE)
//===============================================
typedef enum
{
	ESCAPESTATE_NONE = 0,			// 無状態
	ESCAPESTATE_DISP,				// 表示状態
	ESCAPESTATE_EMPHASIS,			// 強調状態
	ESCAPESTATE_MAX					// この列挙型の総数
}ESCAPESTATE;

//===============================================
// 列挙型定義(EMPHASISSTATE)
//===============================================
typedef enum
{
	EMPHASISSTATE_BIGGER = 0,		// 拡大中状態
	EMPHASISSTATE_SMALLER,			// 縮小中状態
	EMPHASISSTATE_MAX				// この列挙型の総数
}EMPHASISSTATE;

//===============================================
// 状態関係の構造体
//===============================================
typedef struct
{
	ESCAPESTATE	  escapeState;			// 状態
	EMPHASISSTATE emphaState;			// 強調状態
	int			  nStateCounter;		// 状態のカウント
}EscapeState;

//===============================================
// カウントダウンの構造体
//===============================================
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		length;			// 長さ
	EscapeState		stateInfo;		// 脱出状態関係
	D3DXCOLOR		col;			// 色
	bool			bUse;			// 使用しているかどうか
}EscapeInfo;

//プロトタイプ宣言
void InitEscape(void);				// カウントダウンの初期化処理
void UninitEscape(void);			// カウントダウンの終了処理
void UpdateEscape(void);			// カウントダウンの更新処理
void DrawEscape(void);				// カウントダウンの描画処理
void SetEscape(void);				// カウントダウンの設定処理

#endif