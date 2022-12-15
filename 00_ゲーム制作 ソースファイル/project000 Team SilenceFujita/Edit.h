//===================================
//
//エディットヘッダー[Edit.h]
//Author 小原立暉
//
//===================================
#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"
#include "game.h"
#include "billboard.h"

//セットオブジェクトの情報
typedef struct
{
	D3DXVECTOR3 pos;									//位置
	D3DXVECTOR3 rot;									//向き
	D3DXVECTOR3 scale;									//拡大率
	bool bUse;											//オブジェクトが使用可能かどうか
	int nSetNumber;										//オブジェクトの番号
	int nType;											//種類
	D3DXMATRIX mtx;										//ワールドマトリックス
	Doll pDollData;										//モデルの基本情報
}EditObject;

//セットビルボードの情報
typedef struct
{
	D3DXVECTOR3 pos;									//位置
	D3DXVECTOR2 Radius;									//幅
	bool bUse;											//ビルボードが使用可能かどうか
	int nSetNumber;										//ビルボードの番号
	int nType;											//種類
	D3DXMATRIX mtx;										//ワールドマトリックス
	LPDIRECT3DTEXTURE9 Texture[BILLBOARD_MAX];			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;					//頂点バッファへのポインタ
}EditBillboard;

//オブジェクトの種類
typedef enum
{
	EDITSTYLE_OBJECT = 0,								//オブジェクト
	EDITSTYLE_BILLBOARD,								//ビルボード
	EDITSTYLE_MAX
}EDITSTYLE;

//プロトタイプ宣言
void InitEditObject(void);								//オブジェクトの初期化処理
void InitEditBillboard(void);							//ビルボードの初期化処理
void UninitEditObject(void);							//オブジェクトの終了処理
void UpdateEditObject(void);							//オブジェクトの更新処理
void DrawEditObject(void);								//オブジェクトの描画処理
void DrawEditBillboard(void);							//ビルボードの描画処理
EditObject GetEditObject(void);							//エディットオブジェクトの取得処理
EditBillboard GetEditBillboard(void);					//エディットビルボードの取得処理
int GetStyle(void);										//エディットスタイルの取得処理

#endif