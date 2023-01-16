#ifdef _DEBUG	// デバッグ処理
//===================================
//
//エディットヘッダー[Edit.h]
//Author 小原立暉
//
//===================================
#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"
#include "object.h"
#include "game.h"
#include "shadow.h"

//破壊関係の構造体
typedef struct
{
	char *pBreakMode[BREAKTYPE_MAX];					//破壊モードの表記
	int Breaktype;										//破壊の状態
}BREAK;

//影関係の構造体
typedef struct
{
	char *pShadowMode[SHADOWTYPE_MAX];					//影の種類の表記
	int Shadowtype;										//影の種類
}EDITSHADOW;

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
	Model modelData;									//モデルの基本情報
	int nCntMaterial;									//マテリアルの番号
	int nColorCount;									//色を変えるときのカウント
	D3DXMATERIAL MatCopy[MODELTYPE_OBJECT_MAX][MAX_MATERIAL];			//マテリアルのコピー
	D3DXMATERIAL EditMaterial[MODELTYPE_OBJECT_MAX][MAX_MATERIAL];		//カスタム用のマテリアル情報
	BREAK Break;										//破壊の変数
	EDITSHADOW Shadowtype;								//影の変数
}EditObject;

//プロトタイプ宣言
void InitEditObject(void);								//オブジェクトの初期化処理
void UninitEditObject(void);							//オブジェクトの終了処理
void UpdateEditObject(void);							//オブジェクトの更新処理
void DrawEditObject(void);								//オブジェクトの描画処理
EditObject *GetEditObject(void);						//エディットオブジェクトの取得処理

#endif

#endif