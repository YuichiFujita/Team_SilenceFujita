//======================================================================================================================
//
//	警察ヘッダー [Police.h]
//	Author：小原立暉
//
//======================================================================================================================
#ifndef _POLICE_H_						// このマクロ定義がされていない場合
#define _POLICE_H_						// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "model.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_POLICE			(128)		// 使用する警察の最大数

//**********************************************************************************************************************
//	警察車両の状態
//**********************************************************************************************************************
typedef enum
{
	POLICESTATE_SPAWN = 0,				// 出現
	POLICESTATE_PATROL,					// パトロール
	POLICESTATE_CHASE,					// 追跡処理
	POLICESTATE_PATBACK,				// パトロールに戻るときの処理
	POLICESTATE_STOP,					// 足止め処理
	POLICESTATE_MAX						// この列挙型の総数
}POLICESTATE;

//**********************************************************************************************************************
//	警察車両の向かう先
//**********************************************************************************************************************
typedef enum
{
	POLICEDESTINATION_NEAR = 0,			//近くの壁
	POLICEDESTINATION_FAR,				//奥の壁
	POLICEDESTINATION_RIGHT,			//右の壁
	POLICEDESTINATION_LEFT,				//左の壁
	POLICEDESTINATION_MAX				//この列挙型の総数
}POLICEDEST;

//**********************************************************************************************************************
//	警察車両の曲がり角
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 Near;					//手前の曲がり角
	D3DXVECTOR3 Far;					//奥の曲がり角
	D3DXVECTOR3 Right;					//右の曲がり角
	D3DXVECTOR3 Left;					//左の曲がり角
}POLICECURVE;

//**********************************************************************************************************************
//	構造体定義 (Police)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3  pos;					// 現在の位置
	D3DXVECTOR3  posOld;				// 前回の位置
	D3DXVECTOR3  move;					// 移動量
	D3DXVECTOR3  rot;					// 向き
	D3DXVECTOR3  rotDest;				// 目標の向き
	D3DXMATRIX   mtxWorld;				// ワールドマトリックス
	Model        modelData;				// モデル情報
	POLICESTATE  State;					// 警察車両の状態
	int			 nShadowID;				// 影のインデックス
	int			 nLife;					// 寿命
	bool		 bMove;					// 移動しているかどうか
	bool		 bUse;					// 使用しているか
	POLICEDEST   nPoliDest;				// 警察の行先
	POLICECURVE  nPoliCurve;			//警察のカーブ位置
}Police;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitPolice(void);					// 警察の初期化処理
void UninitPolice(void);				// 警察の終了処理
void UpdatePolice(void);				// 警察の更新処理
void DrawPolice(void);					// 警察の描画処理
void SetPolice(D3DXVECTOR3 pos, D3DXVECTOR3 rot, POLICEDEST PoliDest);		// 警察の設定処理
void HitPolice(Police *pPolice, int nDamage);												// 警察のダメージ判定
void CollisionPolice(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pOldPos, float fWidth, float fDepth);	// 警察との当たり判定
Police *GetPoliceData(void);																// 警察の取得処理

//**********************************************************************************************************************
//	プロトタイプ宣言 (デバッグ用)
//**********************************************************************************************************************
int GetNumPolice(void);					// オブジェクトの総数取得処理

#endif