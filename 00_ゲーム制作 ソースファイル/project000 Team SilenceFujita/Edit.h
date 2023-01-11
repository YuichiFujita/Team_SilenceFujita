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
	int nCntMaterial;									//マテリアルの番号
	int nColorCount;									//色を変えるときのカウント
}EditObject;

//エディットマテリアルの情報
typedef struct
{
	D3DXMATERIAL EditMaterial[MAX_MATERIAL] = {};		//カスタム用のマテリアル情報
}EditMaterial;

//プロトタイプ宣言
void InitEditObject(void);								//オブジェクトの初期化処理
void UninitEditObject(void);							//オブジェクトの終了処理
void UpdateEditObject(void);							//オブジェクトの更新処理
void DrawEditObject(void);								//オブジェクトの描画処理
EditObject *GetEditObject(void);						//エディットオブジェクトの取得処理
EditMaterial GetCustomMaterial(void);					//マテリアルの情報取得処理

#endif