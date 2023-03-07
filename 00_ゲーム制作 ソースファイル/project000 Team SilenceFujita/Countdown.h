//===========================================
//
//カウントダウンヘッダー[Countdown.h]
//Author 小原立暉
//
//===========================================
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include"main.h"

//===============================================
// 列挙型定義(CNTDOWNSTATE)
//===============================================
typedef enum
{
	CNTDOWNSTATE_NONE = 0,			// 無状態
	CNTDOWNSTATE_APPEAR,			// 出現状態
	CNTDOWNSTATE_TRANS,				// 透明化状態
	CNTDOWNSTATE_MAX				// この列挙型の総数
}CNTDOWNSTATE;

//===============================================
// 状態関係の構造体
//===============================================
typedef struct
{
	CNTDOWNSTATE state;				// 状態
	int			 nStateCounter;		// 状態のカウント
}CountDownState;

//===============================================
// カウントダウンの構造体
//===============================================
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		length;			// 長さ
	float			fTexU;			// テクスチャのU軸の値
	float			fAlpha;			// 透明度
	CountDownState	stateInfo;		// 状態関係の情報
	bool			bUse;			// 使用しているかどうか
}CountDown;

//プロトタイプ宣言
void InitCountDown(void);			// カウントダウンの初期化処理
void UninitCountDown(void);			// カウントダウンの終了処理
void UpdateCountDown(void);			// カウントダウンの更新処理
void DrawCountDown(void);			// カウントダウンの描画処理
void SetCountDown(void);			// カウントダウンの設定処理

#endif