//======================================================================================================================
//
//	人間ヘッダー [Human.h]
//	Author：小原立暉
//
//======================================================================================================================
#ifndef _HUMAN_H_					// このマクロ定義がされていない場合
#define _HUMAN_H_					// 二重インクルード防止のマクロを定義する

//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "game.h"
#include "model.h"
#include "curve.h"

//**********************************************************************************************************************
//	マクロ定義
//**********************************************************************************************************************
#define MAX_HUMAN		(256)		// 使用する人間の最大数
#define HUMAN_WIDTH		(10.0f)		// 人の縦幅
#define HUMAN_DEPTH		(10.0f)		// 人の奥行

//**********************************************************************************************************************
//	列挙型定義(HUMANSTATE)
//**********************************************************************************************************************
typedef enum
{
	HUMANSTATE_WALK = 0,				// 歩き状態
	HUMANSTATE_STOP,					// ストップ状態
	HUMANSTATE_FLY,						// 吹き飛ばし状態
	HUMANSTATE_OTOTO,					// すれ違い状態
	HUMANSTATE_MAX						// この列挙型の総数
}HUMANSTATE;

//**********************************************************************************************************************
//	構造体定義 (Human)
//**********************************************************************************************************************
typedef struct
{
	D3DXVECTOR3  pos;					// 現在の位置
	D3DXVECTOR3  posOld;				// 前回の位置
	D3DXVECTOR3  move;					// 移動量
	float		 fMaxMove;				// 移動量の最大数
	D3DXVECTOR3  rot;					// 向き
	D3DXMATRIX   mtxWorld;				// ワールドマトリックス
	Model        modelData;				// モデル情報
	int			 nShadowID;				// 影のインデックス
	bool		 bJump;					// ジャンプしているかどうか
	bool		 bMove;					// 移動しているかどうか
	bool		 bUse;					// 使用しているか
	HUMANSTATE	 state;					// 状態
	HUMANCURVE	 curveInfo;				// 曲がり角の情報
	Judge		 judge;					// ジャッジ
}Human;

//**********************************************************************************************************************
//	プロトタイプ宣言
//**********************************************************************************************************************
void InitHuman(void);					// 人間の初期化処理
void UninitHuman(void);					// 人間の終了処理
void UpdateHuman(void);					// 人間の更新処理
void DrawHuman(void);					// 人間の描画処理
void SetHuman(D3DXVECTOR3 pos);			// 人間の設定処理
Human *GetHumanData(void);				// 人間の取得処理

//**********************************************************************************************************************
//	プロトタイプ宣言 (デバッグ用)
//**********************************************************************************************************************

#endif