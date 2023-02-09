//======================================================================================================================
//
//	車ヘッダー [Car.h]
//	Author：小原立暉
//
//======================================================================================================================
#ifndef _CAR_H_						// このマクロ定義がされていない場合
#define _CAR_H_						// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "model.h"
#include "bomb.h"
#include "curve.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_CAR			(128)		// 使用する車の最大数
#define CAR_WIDTH		(45.0f)		// 車の縦幅
#define CAR_DEPTH		(45.0f)		// 車の奥行

//**********************************************************************************************************************
//	ぶつかったもののタイプ
//**********************************************************************************************************************
typedef enum
{
	COLLOBJECTTYPE_PLAYER = 0,			// プレイヤー
	COLLOBJECTTYPE_POLICE,				// 警察
	COLLOBJECTTYPE_CAR,					// 車
	COLLOBJECTTYPE_MAX					// この列挙型の総数
}COLLOBJECTTYPE;

//**********************************************************************************************************************
//	列挙型定義(CARTYPE)
//**********************************************************************************************************************
typedef enum
{
	CARTYPE_NORMAL = 0,					// 普通の車
	CARTYPE_MAX							// この列挙型の総数
}CARTYPE;

//**********************************************************************************************************************
//	列挙型定義(CARSTATE)
//**********************************************************************************************************************
typedef enum
{
	CARSTATE_NORMAL = 0,				// 通常状態
	CARSTATE_TRAFFIC,					// 交通渋滞状態
	CARSTATE_MAX						// この列挙型の総数
}CARSTATE;

//**********************************************************************************************************************
//	構造体定義 (Car)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3  pos;					// 現在の位置
	D3DXVECTOR3  posOld;				// 前回の位置
	D3DXVECTOR3  move;					// 移動量
	D3DXVECTOR3  rot;					// 向き
	D3DXMATRIX   mtxWorld;				// ワールドマトリックス
	Model        modelData;				// モデル情報
	BOMBSTATE    bombState;				// ボムの状態
	int			 nShadowID;				// 影のインデックス
	bool		 bJump;					// ジャンプしているかどうか
	bool		 bMove;					// 移動しているかどうか
	bool		 bUse;					// 使用しているか
	CARCURVE	 carCurveInfo;			// 曲がり角の情報
	int			 nTrafficCnt;			// 渋滞カウント
	CARSTATE	 state;					// 状態
}Car;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitCar(void);					// 車の初期化処理
void UninitCar(void);				// 車の終了処理
void UpdateCar(void);				// 車の更新処理
void DrawCar(void);					// 車の描画処理
void SetCar(D3DXVECTOR3 pos);		// 車の設定処理
Car *GetCarData(void);																		// 車の取得処理
void CollisionStopCar(D3DXVECTOR3 targetpos, D3DXVECTOR3 targetrot, D3DXVECTOR3 *move, float fTargetRadius, COLLOBJECTTYPE collObject,int *pTraCnt);	// 車の停止処理
void CollisionCarBody(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fWidth, float fDepth, COLLOBJECTTYPE collObject, int *pTraCnt);		// 車同士の当たり判定

//**********************************************************************************************************************
//	プロトタイプ宣言 (デバッグ用)
//**********************************************************************************************************************

#endif