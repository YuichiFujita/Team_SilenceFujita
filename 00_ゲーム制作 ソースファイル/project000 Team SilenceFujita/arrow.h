//===================================
//
// 矢印ヘッダー[arrow.h]
// Author 小原立暉
//
//===================================
#ifndef _ARROW_H_
#define _ARROW_H_

#include "main.h"

//マクロ定義

//ビルボードの構造体
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 方向(発射の方向設定に必要)
	D3DXMATRIX	mtx;			// ワールドマトリックス
	bool		bUse;			// 使用状況
}Arrow;

//プロトタイプ宣言
void InitArrow(void);			// 矢印の初期化処理
void UninitArrow(void);			// 矢印の終了処理
void UpdateArrow(void);			// 矢印の更新処理
void DrawArrow(void);			// 矢印の描画処理
void SetArrow(bool bUse);		// 矢印の設定処理

#endif