//===================================
//
//ビルボードヘッダー[billboard.h]
//Author 小原立暉
//
//===================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"

//マクロ定義
#define MAX_BILLBOARD			(256)				//ビルボードの最大数

//ビルボードアニメーション関係
typedef struct
{
	bool bAnim;											//アニメーションするかどうか
	int nAnimCounter;									//アニメーションカウンター
	int nReplayCount;									//再生カウント
	int nAnimPattern;									//アニメーションパターン
	int nReplayPattern;									//再生パターン
	float TexPattern;									//テクスチャのパターン
}BillboardAnim;

//ビルボードの構造体
typedef struct
{
	D3DXVECTOR3 pos;								//位置
	D3DXVECTOR3 rot;								//方向(発射の方向設定に必要)
	D3DXCOLOR col;									//色
	D3DXCOLOR colCopy;								//色のコピー
	D3DXMATRIX mtx;									//ワールドマトリックス
	D3DXVECTOR2 Radius;								//大きさ
	int State;										//状態
	int Type;										//種類
	int nLife;										//寿命
	int nAnimCounter;								//アニメカウンター
	int nAnimPattern;								//アニメパターンNo.
	int nShadow;									//影のインデックス(影の番号)
	bool bShadow;									//影の使用状況
	bool bUse;										//使用しているかどうか
	BillboardAnim Anim;								//アニメーション関係
}Billboard;

//ビルボードの種類
typedef enum
{
	BILLBOARD_EXPL = 0,								//爆発
	BILLBOARD_BULLET,								//弾
	BILLBOARD_MAX
}Billtype;

//オブジェクトの状態
typedef enum
{
	BILLBOARDSTATE_NONE = 0,							//オブジェクト不使用状態
	BILLBOARDSTATE_USE,									//オブジェクト使用中
	BILLBOARDSTATE_DELETETARGET,						//オブジェクト削除判定中
	BILLBOARDSTATE_MAX
}BILLBOARDSTATE;

//プロトタイプ宣言
void InitBillboard(void);							//ビルボードの初期化処理
void UninitBillboard(void);							//ビルボードの終了処理
void UpdateBillboard(void);							//ビルボードの更新処理
void DrawBillboard(void);							//ビルボードの描画処理
void SetBillboard(D3DXVECTOR3 rot, D3DXVECTOR3 pos, int nType, D3DXVECTOR2 Radius, D3DXCOLOR col, int AnimCnt, int AnimPtrn, bool bAnim, bool bShadow);	//ビルボードの設定処理
Billboard *GetBillboard(void);						//ビルボードの取得処理

#endif