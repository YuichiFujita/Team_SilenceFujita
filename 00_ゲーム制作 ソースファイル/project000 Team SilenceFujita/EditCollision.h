#ifdef _DEBUG	// デバッグ処理
//===================================
//
//	エディットヘッダー [EditCollision.h]
//	Author：藤田勇一
//
//===================================
#ifndef _EDITCOLLISION_H_
#define _EDITCOLLISION_H_

#include "main.h"
#include "model.h"

//セットオブジェクトの情報
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 vecPos;					// 位置ベクトル
	D3DXVECTOR3 rot;					// 向き
	D3DXVECTOR3 scale;					// 拡大率
	D3DXMATRIX  mtxWorld;				// ワールドマトリックス
	Model       modelData;				// モデル情報
	int         nType;					// オブジェクトの種類
}EditCollision;

//プロトタイプ宣言
void InitEditCollision(void);			// 当たり判定の初期化処理
void UninitEditCollision(void);			// 当たり判定の終了処理
void UpdateEditCollision(void);			// 当たり判定の更新処理
void DrawEditCollision(void);			// 当たり判定の描画処理
EditCollision *GetEditCollision(void);	// 当たり判定の取得処理

#endif

#endif