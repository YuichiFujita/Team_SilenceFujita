#ifdef _DEBUG	// デバッグ処理
//===================================
//
//エディットヘッダー[Edit.h]
//Author 小原立暉
//
//===================================
#ifndef _EDITBILLBOARD_H_
#define _EDITBILLBOARD_H_

#include "main.h"
#include "game.h"
#include "billboard.h"

//セットビルボードの情報
typedef struct
{
	D3DXVECTOR3 pos;									//位置
	D3DXVECTOR2 Radius;									//幅
	D3DXCOLOR col;										//色
	bool bUse;											//ビルボードが使用可能かどうか
	int nSetNumber;										//ビルボードの番号
	int nType;											//種類
	D3DXMATRIX mtx;										//ワールドマトリックス
	LPDIRECT3DTEXTURE9 Texture[BILLBOARD_MAX];			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;					//頂点バッファへのポインタ
	int nColorCount;									//色を変えるときのカウント
	bool bAnimReplay;									//アニメーションリプレイするかどうか
	BillboardAnim EditAnim;								//アニメーション関係
	bool bShadow;										//影を付けるかどうか
}EditBillboard;

//プロトタイプ宣言
void InitEditBillboard(void);							//ビルボードの初期化処理
void UninitEditBillboard(void);							//ビルボードの終了処理
void UpdateEditBillboard(void);							//ビルボードの更新処理
void DrawEditBillboard(void);							//ビルボードの描画処理
EditBillboard *GetEditBillboard(void);					//エディットビルボードの取得処理

#endif

#endif