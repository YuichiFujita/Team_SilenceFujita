//===================================
//
//タイヤ痕ヘッダー[tiremark.h]
//Author 小原立暉
//
//===================================
#ifndef _TIREMARK_H_
#define _TIREMARK_H_

#include "main.h"

//マクロ定義
#define MAX_TIREMARK			(1024)	// セットマークの最大数

//タイヤ痕の構造体
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXMATRIX	mtx;					// ワールドマトリックス
	D3DXCOLOR	col;					// 色
	int			nClearCount;			// 消えるカウント
	float		fWidth;					// 横の広さ
	float		fDepth;					// 縦の広さ
	bool		bUse;					// 使用しているかどうか
}TireMark;

//プロトタイプ宣言
void InitTireMark(void);				// タイヤ痕の初期化処理
void UninitTireMark(void);				// タイヤ痕の終了処理
void UpdateTireMark(void);				// タイヤ痕の更新処理
void DrawTireMark(void);				// タイヤ痕の描画処理
void SetTireMark(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// タイヤ痕の設定処理
TireMark *GetTireMark(void);			// タイヤ痕の取得処理

#endif