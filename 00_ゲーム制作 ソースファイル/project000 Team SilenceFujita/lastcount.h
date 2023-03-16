//===========================================
//
// ラストカウントヘッダー[lastcount.h]
// Author 小原立暉
//
//===========================================
#ifndef _LASTCOUNT_H_
#define _LASTCOUNT_H_

#include"main.h"

//カウントダウンの構造体
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 size;				// 半径
	int nCount;						// カウント
	int nTime;						// 時間
	int nTimeOld;					// 前回の時間
	int nDigit;						// 桁数
	float fClear;					// 透明度の値
	bool bUse;						// 使用しているかどうか
}LASTCOUNT;

//プロトタイプ宣言
void InitLastCount(void);			// ラストカウントの初期化処理
void UninitLastCount(void);			// ラストカウントの終了処理
void UpdateLastCount(void);			// ラストカウントの更新処理
void DrawLastCount(void);			// ラストカウントの描画処理

#endif