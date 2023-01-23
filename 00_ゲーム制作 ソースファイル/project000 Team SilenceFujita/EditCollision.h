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
#include "object.h"

//列挙型定義 (ROTSTATE)
typedef enum
{
	ROTSTATE_0 = 0,						// 向き 0度 (360度)
	ROTSTATE_90,						// 向き 90度
	ROTSTATE_180,						// 向き 180度
	ROTSTATE_270,						// 向き 270度
	ROTSTATE_MAX,						// この列挙型の総数
} ROTSTATE;

//セットオブジェクトの情報
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	ROTSTATE    stateRot;				// 向き状態
	D3DXVECTOR3 scale;					// 拡大率
	Collision   collision;				// 当たり判定情報
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